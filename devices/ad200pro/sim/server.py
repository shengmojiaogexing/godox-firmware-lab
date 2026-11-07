"""HTTP JSON API + static file server for the AD200 Pro debugging UI.

Usage:
    python -m sim.server [firmware.bin] [--port 8765] [--host 127.0.0.1]
Then open http://127.0.0.1:8765
"""
import argparse
import json
import os
import sys
import threading
import traceback
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs, unquote

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
WEBUI = os.path.join(ROOT, 'webui')
from .machine import Machine, StopRun

MIME = {'.html': 'text/html; charset=utf-8', '.js': 'application/javascript; charset=utf-8',
        '.css': 'text/css; charset=utf-8', '.json': 'application/json; charset=utf-8',
        '.svg': 'image/svg+xml', '.png': 'image/png', '.ico': 'image/x-icon'}


class State:
    def __init__(self, machine):
        self.m = machine
        self.lock = threading.Lock()


def make_handler(state: State):
    class Handler(BaseHTTPRequestHandler):
        server_version = 'AD200Pro-Sim/0.1'
        protocol_version = 'HTTP/1.1'

        # ------------------------------------------------------------ utils
        def log_message(self, fmt, *args):
            if os.environ.get('SIM_HTTP_LOG'):
                sys.stderr.write('%s - %s\n' % (self.address_string(), fmt % args))

        def _cors(self):
            self.send_header('Access-Control-Allow-Origin', '*')
            self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
            self.send_header('Access-Control-Allow-Headers', 'Content-Type')

        def _json(self, obj, code=200):
            data = json.dumps(obj, ensure_ascii=False, default=str).encode('utf-8')
            self.send_response(code)
            self.send_header('Content-Type', 'application/json; charset=utf-8')
            self.send_header('Content-Length', str(len(data)))
            self._cors()
            self.end_headers()
            self.wfile.write(data)

        def _err(self, code, msg):
            self._json({'error': msg}, code)

        def _body(self):
            try:
                n = int(self.headers.get('Content-Length') or 0)
            except ValueError:
                n = 0
            raw = self.rfile.read(n) if n else b''
            if not raw:
                return {}
            try:
                return json.loads(raw.decode('utf-8'))
            except Exception:
                return {}

        def _static(self, path):
            if path in ('/', ''):
                path = '/index.html'
            path = unquote(path.lstrip('/'))
            full = os.path.normpath(os.path.join(WEBUI, path))
            if not full.startswith(WEBUI) or not os.path.isfile(full):
                return False
            ext = os.path.splitext(full)[1].lower()
            data = open(full, 'rb').read()
            self.send_response(200)
            self.send_header('Content-Type', MIME.get(ext, 'application/octet-stream'))
            self.send_header('Content-Length', str(len(data)))
            self.send_header('Cache-Control', 'no-store')
            self._cors()
            self.end_headers()
            self.wfile.write(data)
            return True

        # ------------------------------------------------------------ verbs
        def do_OPTIONS(self):
            self.send_response(204)
            self._cors()
            self.send_header('Content-Length', '0')
            self.end_headers()

        def do_GET(self):
            u = urlparse(self.path)
            q = parse_qs(u.query)
            try:
                if not u.path.startswith('/api/'):
                    if self._static(u.path):
                        return
                    return self._err(404, 'not found')
                with state.lock:
                    m = state.m
                    if u.path == '/api/health':
                        return self._json({'ok': True, 'version': '0.1'})
                    if u.path == '/api/status':
                        return self._json(m.status())
                    if u.path == '/api/symbols':
                        return self._json({'symbols': {'0x%08x' % a: n for a, n in m.fw.symbols.items()},
                                           'vectors': [{'idx': i, 'name': nm, 'value': v}
                                                       for i, (v, nm) in enumerate(zip(
                                                           m.fw.vector_table, m.fw.vector_names + [''] * 48))],
                                           'base': m.fw.base, 'size': m.fw.size})
                    if u.path == '/api/mem':
                        addr = int(q.get('addr', ['0'])[0], 0)
                        n = int(q.get('len', ['256'])[0], 0)
                        data = bytes(m.read(addr, n))
                        return self._json({'addr': addr, 'bytes': list(data),
                                           'hex': data.hex()})
                    if u.path == '/api/disasm':
                        addr = int(q.get('addr', [str(m.regs()['pc'])])[0], 0)
                        n = int(q.get('n', ['20'])[0], 0)
                        return self._json({'addr': addr, 'insns': m.disasm(addr, n)})
                    if u.path == '/api/trace':
                        since = int(q.get('since', ['0'])[0], 0)
                        limit = int(q.get('limit', ['500'])[0], 0)
                        return self._json({'seq': m.tracer.seq, 'events': m.trace(since, limit)})
                    if u.path == '/api/breakpoints':
                        return self._json({'breakpoints': sorted(m.breakpoints)})
                    if u.path == '/api/device':
                        return self._json(m.device_state())
                    if u.path == '/api/backtrace':
                        return self._json({'frames': m.backtrace()})
                    if u.path == '/api/mmio':
                        addr = q.get('addr')
                        ev = [e for e in m.trace(0, 5000) if e['type'].startswith('mmio')]
                        if addr:
                            a = int(addr[0], 0)
                            ev = [e for e in ev if e['addr'] == a]
                        return self._json({'events': ev[-200:]})
                    if u.path == '/api/console':
                        return self._json({'text': ''})
                return self._err(404, 'unknown endpoint')
            except Exception as e:
                traceback.print_exc()
                return self._err(500, '%s: %s' % (type(e).__name__, e))

        def do_POST(self):
            u = urlparse(self.path)
            try:
                body = self._body()
                if not u.path.startswith('/api/'):
                    return self._err(404, 'not found')
                with state.lock:
                    m = state.m
                    if u.path == '/api/reset':
                        m.reset()
                        return self._json(m.status())
                    if u.path == '/api/run':
                        n = int(body.get('max_insns') or 200000)
                        reason = m.run(max_insns=min(n, 2000000))
                        st = m.status()
                        st['stop_reason_detail'] = reason
                        return self._json(st)
                    if u.path == '/api/pause':
                        m.state = 'paused'
                        return self._json(m.status())
                    if u.path == '/api/step':
                        n = int(body.get('n') or 1)
                        m.step(min(n, 10000))
                        return self._json(m.status())
                    if u.path == '/api/breakpoints':
                        for a in body.get('add', []):
                            m.set_breakpoint(int(a, 0) if isinstance(a, str) else int(a))
                        for a in body.get('remove', []):
                            m.clear_breakpoint(int(a, 0) if isinstance(a, str) else int(a))
                        if 'clear' in body:
                            m.breakpoints.clear()
                        return self._json({'breakpoints': sorted(m.breakpoints)})
                    if u.path == '/api/mem':
                        addr = int(body['addr'], 0) if isinstance(body['addr'], str) else int(body['addr'])
                        data = bytes(body['bytes'] if isinstance(body['bytes'], list)
                                     else bytes.fromhex(body['bytes']))
                        m.write(addr, data)
                        return self._json({'ok': True, 'addr': addr, 'len': len(data)})
                    if u.path == '/api/device':
                        kw = {}
                        for k in ('battery', 'battery_mv', 'cap_voltage', 'temp', 'temp_c',
                                  'auto_charge', 'charge_rate'):
                            if k in body:
                                kw[k] = body[k]
                        return self._json(m.device_set(**kw))
                    if u.path == '/api/device/event':
                        kind = body.get('kind') or body.get('type')
                        payload = body.get('payload') or body
                        if kind == 'button':
                            name = payload.get('name') or payload.get('button')
                            if name == 'MENU':
                                name = 'MODE'
                            return self._json(m.device_event('button',
                                                             {'name': name,
                                                              'pressed': payload.get('pressed', True)}))
                        if kind in ('rf_rx', 'rf'):
                            data = payload.get('bytes') or payload.get('data') or []
                            if isinstance(data, str):
                                data = bytes.fromhex(data.replace(' ', ''))
                            return self._json(m.device_event('rf_rx',
                                                             {'channel': payload.get('channel', 0),
                                                              'bytes': list(data)}))
                        return self._json(m.device_event(kind, payload))
                    if u.path == '/api/trace/reset':
                        m.reset_trace()
                        return self._json({'ok': True})
                return self._err(404, 'unknown endpoint')
            except StopRun as e:
                return self._json({'error': str(e), 'status': state.m.status()}, 500)
            except Exception as e:
                traceback.print_exc()
                return self._err(500, '%s: %s' % (type(e).__name__, e))

    return Handler


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument('bin', nargs='?', default=None)
    ap.add_argument('--port', type=int, default=8765)
    ap.add_argument('--host', default='127.0.0.1')
    args = ap.parse_args(argv)
    print('loading firmware...')
    m = Machine(args.bin)
    st = State(m)
    httpd = ThreadingHTTPServer((args.host, args.port), make_handler(st))
    print('AD200 Pro simulator: http://%s:%d  (webui: %s)' % (args.host, args.port, WEBUI))
    print('firmware image base 0x%08x, reset 0x%08x' % (m.fw.base, m.fw.reset_pc))
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print('\nbye')


if __name__ == '__main__':
    main()
