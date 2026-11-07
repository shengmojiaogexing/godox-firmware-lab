#!/usr/bin/env node
/**
 * studio/tests/test_ui.mjs — 可视化密钥工坊的 UI 回归（无头 Edge + CDP）。
 *
 * 用法：
 *   node studio/tests/test_ui.mjs --url http://127.0.0.1:18780/
 *   node studio/tests/test_ui.mjs --url http://127.0.0.1:18903/?mock=1 --out %TEMP%\studio.png
 *
 * 说明：
 *  - 自己拉起 headless Edge（唯一 --user-data-dir / 唯一调试端口），结束只清理自己的进程树；
 *  - 通过 CDP 断言：页面标题、设备卡片、密钥校验、生成结果、下载链接、控制台无错误；
 *  - 截图保存到 --out（默认 %TEMP%/studio_ui_e2e.png）。
 */
import { spawn, spawnSync } from 'node:child_process';
import fs from 'node:fs';
import net from 'node:net';
import os from 'node:os';
import path from 'node:path';

const EDGE = process.env.EDGE_PATH
  || 'C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe';

function parseArgs(argv) {
  const out = { url: 'http://127.0.0.1:18780/', out: path.join(os.tmpdir(), 'studio_ui_e2e.png'), timeout: 90000, all: false };
  for (let i = 0; i < argv.length; i += 1) {
    const a = argv[i];
    if (a === '--url') out.url = argv[++i];
    else if (a === '--out') out.out = argv[++i];
    else if (a === '--timeout') out.timeout = Number(argv[++i]);
    else if (a === '--all') out.all = true;
  }
  return out;
}

const opts = parseArgs(process.argv.slice(2));
const checks = [];
const consoleErrors = [];
let ws = null;
let edge = null;
let userDataDir = null;
let msgId = 0;
const pending = new Map();

function freePort() {
  return new Promise((resolve, reject) => {
    const srv = net.createServer();
    srv.listen(0, '127.0.0.1', () => {
      const port = srv.address().port;
      srv.close(() => resolve(port));
    });
    srv.on('error', reject);
  });
}

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

function cleanup() {
  try { if (ws && ws.readyState === 1) ws.close(); } catch { /* ignore */ }
  if (edge && edge.pid) {
    try { spawnSync('taskkill', ['/PID', String(edge.pid), '/T', '/F'], { stdio: 'ignore' }); } catch { /* ignore */ }
  }
  if (userDataDir) {
    try { fs.rmSync(userDataDir, { recursive: true, force: true }); } catch { /* ignore */ }
  }
}

function check(name, ok, detail = '') {
  checks.push({ name, ok: !!ok, detail });
  if (!ok) console.error(`[FAIL] ${name}${detail ? ' — ' + detail : ''}`);
  else console.log(`[PASS] ${name}`);
  return !!ok;
}

async function cdp(method, params = {}) {
  const id = ++msgId;
  return new Promise((resolve, reject) => {
    const timer = setTimeout(() => {
      pending.delete(id);
      reject(new Error(`CDP timeout: ${method}`));
    }, 30000);
    pending.set(id, { resolve: (v) => { clearTimeout(timer); resolve(v); }, reject });
    ws.send(JSON.stringify({ id, method, params }));
  });
}

async function evalJs(expression) {
  const res = await cdp('Runtime.evaluate', {
    expression,
    returnByValue: true,
    awaitPromise: true,
    userGesture: true,
  });
  if (res.exceptionDetails) {
    throw new Error('JS exception: ' + (res.exceptionDetails.text || JSON.stringify(res.exceptionDetails)));
  }
  return res.result ? res.result.value : undefined;
}

async function waitFor(name, expression, timeoutMs = opts.timeout) {
  const start = Date.now();
  let lastErr = '';
  while (Date.now() - start < timeoutMs) {
    try {
      const v = await evalJs(expression);
      if (v) { check(name, true); return v; }
    } catch (e) { lastErr = String(e); }
    await sleep(250);
  }
  check(name, false, `等待超时: ${expression}${lastErr ? ' / ' + lastErr : ''}`);
  return false;
}

async function httpJson(url, init) {
  const res = await fetch(url, init);
  const text = await res.text();
  try { return { status: res.status, json: JSON.parse(text) }; } catch { return { status: res.status, json: null }; }
}

async function main() {
  if (!fs.existsSync(EDGE)) throw new Error('找不到 Edge: ' + EDGE);
  console.log('[info] Edge:', EDGE);
  console.log('[info] URL :', opts.url);

  // 0) 目标可访问性（后端或静态服务）
  try {
    const res = await fetch(opts.url, { method: 'GET' });
    await res.text(); // 必须消费响应体，否则 Node 24/undici 在连接关闭时会断言失败
    if (!res.ok) throw new Error('HTTP ' + res.status);
    console.log('[info] 目标页面可访问');
  } catch (e) {
    throw new Error('目标不可访问: ' + opts.url + ' — ' + e.message);
  }

  // 1) 启动 headless Edge（唯一 user-data-dir + 唯一调试端口）
  const debugPort = await freePort();
  userDataDir = fs.mkdtempSync(path.join(os.tmpdir(), 'studio-e2e-'));
  edge = spawn(EDGE, [
    '--headless=new',
    '--disable-gpu',
    '--no-first-run',
    '--no-default-browser-check',
    '--disable-extensions',
    '--hide-scrollbars',
    '--remote-allow-origins=*',
    '--window-size=1440,1400',
    `--user-data-dir=${userDataDir}`,
    `--remote-debugging-port=${debugPort}`,
    'about:blank',
  ], { stdio: 'ignore', windowsHide: true });
  console.log('[info] Edge PID:', edge.pid, 'debug port:', debugPort);

  // 2) 等调试端点
  let targets = null;
  for (let i = 0; i < 80; i += 1) {
    await sleep(250);
    try {
      const res = await fetch(`http://127.0.0.1:${debugPort}/json/list`);
      targets = await res.json();
      if (Array.isArray(targets) && targets.length) break;
    } catch { /* retry */ }
  }
  if (!targets || !targets.length) throw new Error('Edge CDP 未就绪');
  const page = targets.find((t) => t.type === 'page') || targets[0];
  console.log('[info] 页面 target:', page.id);

  // 3) 连接 CDP
  ws = new WebSocket(page.webSocketDebuggerUrl);
  await new Promise((resolve, reject) => {
    ws.addEventListener('open', resolve, { once: true });
    ws.addEventListener('error', () => reject(new Error('WebSocket 连接失败')), { once: true });
  });
  ws.addEventListener('message', (ev) => {
    let msg;
    try { msg = JSON.parse(ev.data); } catch { return; }
    if (msg.id && pending.has(msg.id)) {
      const { resolve, reject } = pending.get(msg.id);
      pending.delete(msg.id);
      if (msg.error) reject(new Error('CDP error: ' + JSON.stringify(msg.error)));
      else resolve(msg.result || {});
    }
    if (msg.method === 'Runtime.exceptionThrown') {
      const d = msg.params.exceptionDetails || {};
      consoleErrors.push('exception: ' + (d.exception && (d.exception.description || d.exception.value) || d.text));
    }
    if (msg.method === 'Runtime.consoleAPICalled' && msg.params.type === 'error') {
      consoleErrors.push('console.error: ' + (msg.params.args || []).map((a) => a.value ?? a.description ?? '').join(' '));
    }
    if (msg.method === 'Log.entryAdded' && msg.params.entry && msg.params.entry.level === 'error') {
      consoleErrors.push('log: ' + msg.params.entry.text);
    }
  });

  await cdp('Page.enable');
  await cdp('Runtime.enable');
  await cdp('Log.enable');
  await cdp('Page.navigate', { url: opts.url });
  await sleep(1200);

  // 4) 断言
  const title = await evalJs('document.title + " | " + (document.querySelector("h1")?.textContent || "")');
  check('页面标题包含品牌名', String(title || '').includes('神牛密钥工坊'), String(title));

  await waitFor('后端/模拟数据就绪（设备卡片渲染）',
    'document.querySelectorAll("[data-testid=\\"device-card\\"]").length >= 1');
  const cards = await evalJs('document.querySelectorAll("[data-testid=\\"device-card\\"]").length');
  check('设备卡片数量 >= 3', cards >= 3, '实际 ' + cards);

  // 选择设备（默认选第一台；--all 时全选）
  if (opts.all) {
    await evalJs(`(() => {
      document.querySelectorAll('[data-testid="device-select"]').forEach((box) => {
        if (!box.checked) box.click();
      });
      return true;
    })()`);
  } else {
    await evalJs(`(() => {
      const box = document.querySelector('[data-testid="device-select"]');
      if (box) { if (!box.checked) box.click(); else box.checked = true; }
      return !!(box && box.checked);
    })()`);
  }
  await sleep(300);
  const selected = await evalJs('document.querySelectorAll("[data-testid=\\"device-select\\"]:checked").length');
  check('可以勾选设备', selected >= 1, '已选 ' + selected);

  // 输入密钥（走原生 setter + input 事件）
  await evalJs(`(() => {
    const el = document.querySelector('[data-testid="key-input"]');
    if (!el) return false;
    const setter = Object.getOwnPropertyDescriptor(HTMLInputElement.prototype, 'value').set;
    setter.call(el, 'D798');
    el.dispatchEvent(new Event('input', { bubbles: true }));
    el.dispatchEvent(new Event('change', { bubbles: true }));
    return true;
  })()`);
  await sleep(800);
  const keyValue = await evalJs('document.querySelector("[data-testid=\\"key-input\\"]")?.value');
  check('密钥输入可用', String(keyValue || '').includes('D798'), String(keyValue));
  const syncText = await evalJs('document.body.innerText');
  check('页面显示 SYNC 信息', /D7/.test(syncText) && /SYNC/i.test(syncText));

  // 生成
  const genClicked = await evalJs(`(() => {
    const btn = document.querySelector('[data-testid="generate-btn"]');
    if (!btn) return false;
    btn.click();
    return true;
  })()`);
  check('生成按钮存在且可点击', genClicked);
  await waitFor('生成结果出现',
    'document.querySelectorAll("[data-testid=\\"result-file\\"]").length >= 1', opts.timeout);

  const resultInfo = await evalJs(`(() => {
    const nodes = [...document.querySelectorAll('[data-testid="result-file"]')];
    return {
      count: nodes.length,
      text: nodes.map(n => n.innerText).join('\\n').slice(0, 4000),
      links: nodes.map(n => [...n.querySelectorAll('a')].map(a => a.getAttribute('href') || '')),
    };
  })()`);
  check('结果卡片包含文件名/描述', /keyD798|\.bin|固件/i.test(resultInfo.text), (resultInfo.text || '').slice(0, 200));
  check('结果卡片包含 sha256', /[0-9a-f]{64}/i.test(resultInfo.text), '');
  const hasDownload = resultInfo.links.flat().some((h) => h && (h.includes('/api/builds/') || h.endsWith('.bin') || h.endsWith('.zip')));
  check('结果包含下载链接', hasDownload || /下载/.test(resultInfo.text));

  // 截图（先回到顶部，避免 sticky header 与内容重叠）
  await evalJs('window.scrollTo(0, 0)');
  await sleep(400);
  const shot = await cdp('Page.captureScreenshot', { format: 'png', captureBeyondViewport: true });
  fs.mkdirSync(path.dirname(opts.out), { recursive: true });
  fs.writeFileSync(opts.out, Buffer.from(shot.data, 'base64'));
  console.log('[info] 截图:', opts.out);

  // 控制台错误（mock 模式允许没有后端，但不应有 JS 异常）
  const relevantErrors = consoleErrors.filter((e) => !/Failed to load resource.*40[0-9]/.test(e));
  check('无 JS 控制台错误', relevantErrors.length === 0, relevantErrors.slice(0, 3).join(' | '));
}

main()
  .then(() => {
    const failed = checks.filter((c) => !c.ok);
    console.log('\n================ UI 回归结果 ================');
    console.log(`通过 ${checks.length - failed.length}/${checks.length}`);
    if (failed.length) {
      failed.forEach((f) => console.log('  FAIL:', f.name, f.detail));
      cleanup();
      process.exit(1);
    }
    cleanup();
    console.log('ALL PASS');
    process.exit(0);
  })
  .catch((err) => {
    console.error('[ERROR]', err.message || err);
    if (process.env.STUDIO_E2E_DEBUG) console.error(err.stack);
    cleanup();
    process.exit(2);
  });
