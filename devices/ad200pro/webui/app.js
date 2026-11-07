/* ============================================================================
 * GODOX AD200 Pro 固件调试器 —— Web 前端
 * ----------------------------------------------------------------------------
 * 纯静态、无构建步骤、无外部依赖；对接 docs/SIM-SPEC.md §6 的 HTTP JSON API。
 * 数据来源：同源 `/api/*`（真实后端 sim/server.py 或自测 mock_server.py）。
 * 所有后端数据都经过宽松归一化，以兼容 §6 契约下的不同字段形状。
 * ========================================================================== */
(function () {
  'use strict';

  /* ------------------------------------------------------------------------
   * 常量与配置
   * ---------------------------------------------------------------------- */

  const POLL_STATUS_MS = 300;     // /api/status 轮询
  const POLL_TRACE_MS = 300;      // /api/trace 轮询
  const POLL_DEVICE_MS = 1000;    // /api/device 轮询（GPIO/ADC 反馈）
  const POLL_CONSOLE_MS = 1200;   // /api/console 轮询
  const DISASM_COUNT = 40;        // 反汇编一次抓取条数
  const MEM_LEN = 256;            // 内存视图一页字节数（16 行 × 16 字节）
  const TRACE_BUFFER_MAX = 2500;  // 内存中保留的 trace 事件上限
  const TRACE_DOM_MAX = 1200;     // DOM 中最多渲染的 trace 行数
  const LS_API_BASE = 'ad200.debugger.apiBase';
  const LS_FILTERS = 'ad200.debugger.filters';

  const CODE_START = 0x08003000;
  const RESET_PC = 0x080030D4;
  const MAIN_LOOP = 0x0800CDB0;
  const RAM_START = 0x20000000;

  const PORT_NAMES = ['A', 'B', 'C', 'D', 'F'];
  const PORT_BASES = { A: 0x48000000, B: 0x48000400, C: 0x48000800, D: 0x48000C00, F: 0x48001400 };

  /** STM32F0 NVIC IRQ 名称（悬停提示用）。SysTick 记为 -1。 */
  const IRQ_NAMES = {
    '-1': 'SysTick', 0: 'WWDG', 1: 'PVD', 2: 'RTC', 3: 'FLASH', 4: 'RCC',
    5: 'EXTI0_1', 6: 'EXTI2_3', 7: 'EXTI4_15', 8: 'TSC', 9: 'DMA1_CH1',
    10: 'DMA1_CH2_3', 11: 'DMA1_CH4_7', 12: 'ADC_COMP', 13: 'TIM1_BRK_UP',
    14: 'TIM1_CC', 15: 'TIM2', 16: 'TIM3', 17: 'TIM6', 18: 'TIM7', 19: 'TIM14',
    20: 'TIM15', 21: 'TIM16', 22: 'TIM17', 23: 'I2C1', 24: 'I2C2', 25: 'SPI1',
    26: 'SPI2', 27: 'USART1', 28: 'USART2', 29: 'USART3_6', 30: 'CEC_CAN', 31: 'USB',
  };

  /** Trace 过滤分组：任务要求的 call/mmio/irq/gpio_change/unimpl/fault + 其他。 */
  const TRACE_GROUPS = [
    { key: 'call', label: '调用', cls: 'f-call', types: ['call', 'ret'] },
    { key: 'mmio', label: 'MMIO', cls: 'f-mmio', types: ['mmio_read', 'mmio_write'] },
    { key: 'irq', label: '中断', cls: 'f-irq', types: ['irq_enter', 'irq_exit', 'exception'] },
    { key: 'gpio', label: 'GPIO', cls: 'f-gpio', types: ['gpio_change'] },
    { key: 'unimpl', label: '未实现', cls: 'f-unimpl', types: ['unimpl'] },
    { key: 'fault', label: '故障', cls: 'f-fault', types: ['fault'] },
    { key: 'other', label: '其他', cls: 'f-other', types: ['uart_tx', 'uart_rx', 'note'] },
  ];
  const ALL_GROUP_KEYS = TRACE_GROUPS.map((g) => g.key);
  const TYPE_TO_GROUP = (function () {
    const map = {};
    TRACE_GROUPS.forEach((g) => g.types.forEach((t) => { map[t] = g.key; }));
    return map;
  })();

  /** trace 类型 -> 徽标文字 + CSS 类。 */
  const TYPE_BADGE = {
    call: ['调用', 't-call'], ret: ['返回', 't-call'],
    mmio_read: ['MMIO 读', 't-mmio'], mmio_write: ['MMIO 写', 't-mmio'],
    irq_enter: ['中断入', 't-irq'], irq_exit: ['中断出', 't-irq'], exception: ['异常', 't-irq'],
    gpio_change: ['GPIO', 't-gpio'],
    uart_tx: ['串口TX', 't-other'], uart_rx: ['串口RX', 't-other'],
    unimpl: ['未实现', 't-unimpl'], fault: ['故障', 't-fault'], note: ['备注', 't-other'],
    adc: ['ADC', 't-other'], dma: ['DMA', 't-other'], flash: ['FLASH', 't-other'],
    rf_tx: ['RF TX', 't-other'], rf_rx: ['RF RX', 't-other'], systick: ['SysTick', 't-other'],
  };

  const STATE_LABELS = {
    running: '运行中', paused: '暂停', stalled: '卡死', fault: '故障',
    ready: '就绪', idle: '空闲', reset: '复位', unknown: '未知',
  };
  const STOP_REASON_LABELS = {
    breakpoint: '命中断点', insn_limit: '达到指令上限', until_pc: '到达目标 PC',
    stall: '外设卡死', fault: '固件故障', pause: '用户暂停',
  };

  const BUTTON_DEFS = [
    { name: 'POWER', label: 'POWER' },
    { name: 'TEST', label: 'TEST' },
    { name: 'MENU', label: 'MENU' },
    { name: 'SET', label: 'SET' },
    { name: 'UP', label: '▲ UP' },
    { name: 'DOWN', label: '▼ DOWN' },
  ];

  /* ------------------------------------------------------------------------
   * 全局状态
   * ---------------------------------------------------------------------- */

  const state = {
    apiBase: localStorage.getItem(LS_API_BASE) || '',
    connected: false,
    everConnected: false,
    failCount: 0,
    lastStatus: null,
    lastPc: null,
    symbols: {},
    symbolList: [],
    vectors: [],
    breakpoints: new Set(),
    regCells: null,
    regPrevValues: {},
    lastIrqKey: '',
    disasmInsns: [],
    disasmBase: null,
    disasmToken: 0,
    disasmRows: {},
    followPc: true,
    lastHighlightPc: null,
    memAddr: RAM_START,
    memBytes: [],
    memCellMap: {},
    memEditing: null,
    memToken: 0,
    device: null,
    deviceLoaded: false,
    sliderHold: { battery: 0, cap: 0, temp: 0 },
    deviceTimers: {},
    traceSeq: 0,
    traceBuffer: [],
    traceFilters: new Set(ALL_GROUP_KEYS),
    traceInFlight: false,
    traceAutoScroll: true,
    consoleSince: 0,
    consoleLastText: '',
    consoleSuppressed: '',
    consoleInFlight: false,
    runningRequest: false,
    statusInFlight: false,
    deviceInFlight: false,
    gpioCells: null,
    padButtons: {},
    lastRfInjected: '',
    timers: { status: null, trace: null, device: null, console: null },
    toastTimer: null,
    maxInsns: 2000000,
  };

  /* ------------------------------------------------------------------------
   * 基础工具
   * ---------------------------------------------------------------------- */

  function $(id) { return document.getElementById(id); }

  function esc(value) {
    return String(value === null || value === undefined ? '' : value)
      .replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;').replace(/'/g, '&#39;');
  }

  function hx(value) {
    const n = Number(value);
    if (!isFinite(n)) return '—';
    return '0x' + (n >>> 0).toString(16).toUpperCase().padStart(8, '0');
  }

  function hx16(value) {
    const n = Number(value);
    if (!isFinite(n)) return '—';
    return '0x' + (n & 0xFFFF).toString(16).toUpperCase().padStart(4, '0');
  }

  function fmtNum(value) {
    const n = Number(value);
    if (!isFinite(n)) return '0';
    return Math.trunc(n).toLocaleString('en-US');
  }

  function toInt(value, fallback) {
    const fb = fallback === undefined ? 0 : fallback;
    if (value === undefined || value === null || value === '') return fb;
    if (typeof value === 'number') return isFinite(value) ? Math.trunc(value) : fb;
    if (typeof value === 'boolean') return value ? 1 : 0;
    const s = String(value).trim();
    if (!s) return fb;
    let v;
    if (/^0x/i.test(s)) v = parseInt(s.slice(2), 16);
    else if (/^-0x/i.test(s)) v = -parseInt(s.slice(3), 16);
    else v = parseInt(s, 10);
    return isFinite(v) ? v : fb;
  }

  function toNum(value, fallback) {
    const fb = fallback === undefined ? NaN : fallback;
    if (value === undefined || value === null || value === '') return fb;
    if (typeof value === 'number') return isFinite(value) ? value : fb;
    const s = String(value).trim();
    const v = /^0x/i.test(s) ? parseInt(s.slice(2), 16) : parseFloat(s);
    return isFinite(v) ? v : fb;
  }

  /** 用户输入/JSON 键地址解析：0x 前缀为十六进制；纯数字短串按十进制，长串按十六进制。 */
  function parseAddr(value) {
    if (typeof value === 'number') return value >>> 0;
    if (typeof value !== 'string') return NaN;
    let s = value.trim().replace(/[_\s,]/g, '');
    if (!s) return NaN;
    let neg = false;
    if (s[0] === '-') { neg = true; s = s.slice(1); }
    let v;
    if (/^0x/i.test(s)) v = parseInt(s.slice(2), 16);
    else if (/[a-f]/i.test(s)) v = parseInt(s, 16);
    else if (s.length >= 6) v = parseInt(s, 16);
    else v = parseInt(s, 10);
    if (!isFinite(v)) return NaN;
    return (neg ? -v : v) >>> 0;
  }

  /** 符号表键的地址解析：兼容 0x 开头、十进制与十六进制字符串。 */
  function parseSymbolAddr(key) {
    const s = String(key).trim();
    if (/^0x/i.test(s)) {
      const v = parseInt(s.slice(2), 16);
      return isFinite(v) ? v : NaN;
    }
    if (!/^[0-9a-fA-F]+$/.test(s)) return NaN;
    const dec = /^[0-9]+$/.test(s) ? parseInt(s, 10) : NaN;
    const hex = parseInt(s, 16);
    const valid = (v) => isFinite(v) && ((v >= 0x06000000 && v <= 0x0C000000) || (v >= 0x20000000 && v <= 0x20010000));
    if (valid(dec)) return dec;
    if (valid(hex)) return hex;
    if (s.length === 8 && isFinite(hex)) return hex;
    return isFinite(dec) ? dec : hex;
  }

  function hexParam(addr) { return '0x' + (addr >>> 0).toString(16); }

  function now() { return Date.now(); }

  function debounce(fn, wait) {
    let timer = null;
    return function () {
      const args = arguments;
      const self = this;
      clearTimeout(timer);
      timer = setTimeout(function () { fn.apply(self, args); }, wait);
    };
  }

  function toast(message, kind) {
    const el = $('toast');
    if (!el) return;
    el.textContent = message;
    el.className = 'toast ' + (kind || '') + (el.classList.contains('hidden') ? ' hidden' : '');
    el.classList.remove('hidden');
    clearTimeout(state.toastTimer);
    state.toastTimer = setTimeout(function () { el.classList.add('hidden'); }, kind === 'err' ? 4500 : 2800);
  }

  /* ------------------------------------------------------------------------
   * HTTP API 封装
   * ---------------------------------------------------------------------- */

  function ApiError(message, status, data) {
    const err = new Error(message);
    err.name = 'ApiError';
    err.status = status || 0;
    err.data = data || null;
    return err;
  }

  function apiUrl(path) {
    const base = state.apiBase ? state.apiBase.replace(/\/+$/, '') : '';
    return base + path;
  }

  async function api(path, options) {
    const opts = options || {};
    const timeoutMs = opts.timeout || 8000;
    const controller = (typeof AbortController !== 'undefined') ? new AbortController() : null;
    const timer = controller ? setTimeout(function () { controller.abort(); }, timeoutMs) : null;
    const init = {
      method: opts.method || 'GET',
      cache: 'no-store',
      headers: {},
    };
    if (controller) init.signal = controller.signal;
    if (opts.body !== undefined) {
      init.headers['Content-Type'] = 'application/json';
      init.body = JSON.stringify(opts.body);
    }
    try {
      const res = await fetch(apiUrl(path), init);
      const text = await res.text();
      let data = null;
      if (text) {
        try { data = JSON.parse(text); } catch (e) { data = text; }
      }
      if (!res.ok) {
        const msg = (data && typeof data === 'object' && data.error) ? data.error : ('HTTP ' + res.status);
        throw ApiError(msg, res.status, data);
      }
      return data;
    } catch (err) {
      if (err && err.name === 'ApiError') throw err;
      if (err && err.name === 'AbortError') throw ApiError('请求超时（' + timeoutMs + ' ms）', 0, null);
      throw ApiError('无法连接后端（' + (err && err.message ? err.message : String(err)) + '）', 0, null);
    } finally {
      if (timer) clearTimeout(timer);
    }
  }

  /* ------------------------------------------------------------------------
   * 数据归一化
   * ---------------------------------------------------------------------- */

  function unwrapStatus(raw) {
    if (raw && typeof raw === 'object' && raw.status && typeof raw.status === 'object' && raw.status.regs) return raw.status;
    return raw || {};
  }

  /** 响应里是否真的包含状态数据（用于兼容只返回 {ok:true} 的后端）。 */
  function hasStatusData(raw) {
    const r = unwrapStatus(raw);
    if (!r || typeof r !== 'object') return false;
    return !!(r.regs || r.state || r.stats || r.pc !== undefined);
  }

  function applyActionStatus(raw) {
    if (hasStatusData(raw)) applyStatus(raw);
    else pollStatus();
  }

  function hasDeviceData(raw) {
    if (!raw || typeof raw !== 'object') return false;
    if (raw.device && typeof raw.device === 'object') return true;
    return ['battery', 'battery_mv', 'cap_voltage', 'temp', 'temp_c', 'gpio',
      'gpio_inputs', 'adc', 'buttons', 'power', 'charging'].some(function (k) {
      return raw[k] !== undefined;
    });
  }

  function applyDeviceResponse(raw) {
    if (hasDeviceData(raw)) applyDevice(raw);
    else loadDevice();
  }

  function normalizeAddrList(value) {
    if (!Array.isArray(value)) return null;
    const out = [];
    value.forEach(function (v) {
      const a = typeof v === 'string' ? parseAddr(v) : toInt(v, NaN);
      if (isFinite(a)) out.push(a);
    });
    out.sort(function (a, b) { return a - b; });
    return out;
  }

  function normalizeIrqList(value) {
    const out = [];
    if (Array.isArray(value)) {
      value.forEach(function (v) { const n = toInt(v, NaN); if (isFinite(n)) out.push(n); });
    } else if (typeof value === 'number') {
      for (let bit = 0; bit < 32; bit += 1) if (value & (1 << bit)) out.push(bit);
    } else if (typeof value === 'string') {
      const n = toInt(value, NaN);
      if (isFinite(n)) for (let bit = 0; bit < 32; bit += 1) if (n & (1 << bit)) out.push(bit);
    } else if (value && typeof value === 'object') {
      Object.keys(value).forEach(function (k) {
        const v = value[k];
        if (v) { const n = toInt(k, NaN); if (isFinite(n)) out.push(n); }
      });
    }
    const uniq = {};
    out.forEach(function (n) { uniq[n] = true; });
    return Object.keys(uniq).map(Number).sort(function (a, b) { return a - b; });
  }

  function normalizeStatus(raw) {
    raw = unwrapStatus(raw);
    const regsIn = (raw && raw.regs) || {};
    const regs = {};
    for (let i = 0; i < 16; i += 1) {
      let v = regsIn['r' + i];
      if (v === undefined || v === null) v = regsIn['R' + i];
      if (v === undefined || v === null) {
        if (i === 13) v = regsIn.sp !== undefined ? regsIn.sp : regsIn.SP;
        else if (i === 14) v = regsIn.lr !== undefined ? regsIn.lr : regsIn.LR;
        else if (i === 15) v = regsIn.pc !== undefined ? regsIn.pc : regsIn.PC;
      }
      if (v !== undefined && v !== null) regs['r' + i] = toInt(v, 0) >>> 0;
    }
    regs.xpsr = toInt(regsIn.xpsr !== undefined ? regsIn.xpsr : regsIn.XPSR, 0) >>> 0;
    regs.primask = toInt(regsIn.primask !== undefined ? regsIn.primask : regsIn.PRIMASK, 0) >>> 0;
    const stats = raw.stats || {};
    const irq = raw.irq || {};
    return {
      raw: raw,
      state: String(raw.state || raw.status || 'unknown').toLowerCase(),
      pc: toInt(raw.pc !== undefined ? raw.pc : (regsIn.pc !== undefined ? regsIn.pc : regs.r15), 0) >>> 0,
      pcName: raw.pc_name || raw.pcName || null,
      regs: regs,
      stats: {
        insn_count: toInt(stats.insn_count !== undefined ? stats.insn_count
          : (stats.insns !== undefined ? stats.insns : stats.instructions), 0),
        cycle_estimate: toNum(stats.cycle_estimate !== undefined ? stats.cycle_estimate : stats.cycles, null),
        uptime_ms: toNum(stats.uptime_ms, 0),
      },
      breakpoints: normalizeAddrList(raw.breakpoints),
      irq: {
        enabled: normalizeIrqList(irq.enabled !== undefined ? irq.enabled : irq.enable),
        pending: normalizeIrqList(irq.pending !== undefined ? irq.pending : irq.pend),
      },
      stop_reason: raw.stop_reason || raw.stopReason || raw.reason
        || raw.stop_reason_detail || raw.stopReasonDetail || null,
      stop_reason_detail: raw.stop_reason_detail || raw.stopReasonDetail || null,
    };
  }

  function normalizeInsns(raw) {
    let list = [];
    if (raw && Array.isArray(raw.insns)) list = raw.insns;
    else if (Array.isArray(raw)) list = raw;
    else if (raw && Array.isArray(raw.disasm)) list = raw.disasm;
    return list.map(function (item) {
      let addr;
      let bytes;
      let text;
      if (Array.isArray(item)) {
        addr = toInt(item[0], NaN);
        bytes = item[1];
        text = item[2];
      } else if (item && typeof item === 'object') {
        addr = toInt(item.addr !== undefined ? item.addr : item.address, NaN);
        bytes = item.bytes !== undefined ? item.bytes : item.data;
        text = item.text !== undefined ? item.text : (item.asm !== undefined ? item.asm : item.disasm);
      } else {
        return null;
      }
      if (!isFinite(addr)) return null;
      return { addr: addr >>> 0, bytes: normalizeBytes(bytes), text: String(text === undefined || text === null ? '' : text) };
    }).filter(Boolean);
  }

  function normalizeBytes(value) {
    if (value === undefined || value === null) return [];
    if (typeof value === 'string') {
      const clean = value.replace(/0x/gi, '').replace(/[^0-9a-fA-F]/g, '');
      const pairs = clean.match(/.{1,2}/g) || [];
      return pairs.map(function (p) { return parseInt(p, 16) & 0xFF; });
    }
    if (Array.isArray(value)) return value.map(function (b) { return toInt(b, 0) & 0xFF; });
    if (value && typeof value === 'object') {
      const out = [];
      Object.keys(value).sort(function (a, b) { return toInt(a) - toInt(b); }).forEach(function (k) {
        out.push(toInt(value[k], 0) & 0xFF);
      });
      return out;
    }
    return [];
  }

  function normalizeEvent(ev) {
    ev = ev || {};
    const type = String(ev.type || ev.etype || 'note');
    let pc = ev.pc;
    if (pc === undefined || pc === null) pc = 0;
    let addr = ev.addr;
    if (addr === undefined) addr = null;
    return {
      seq: toInt(ev.seq !== undefined ? ev.seq : ev.id, 0),
      type: type,
      pc: toInt(pc, 0) >>> 0,
      addr: addr === null || addr === undefined ? null : (toInt(addr, 0) >>> 0),
      name: ev.name !== undefined && ev.name !== null ? String(ev.name) : (ev.symbol !== undefined ? String(ev.symbol) : null),
      data: ev.data !== undefined ? ev.data : (ev.value !== undefined ? ev.value : null),
      raw: ev,
    };
  }

  /** 设备状态归一化：兼容 SIM-SPEC 与常见替身字段名。 */
  function normalizeDevice(raw) {
    if (raw && typeof raw === 'object' && raw.device && typeof raw.device === 'object') {
      const d = raw.device;
      if (d.battery !== undefined || d.battery_mv !== undefined || d.gpio || d.gpio_inputs || d.adc) raw = d;
    }
    if (raw && typeof raw === 'object' && raw.state && typeof raw.state === 'object'
      && (raw.state.battery_mv !== undefined || raw.state.cap_voltage !== undefined
        || raw.state.gpio_inputs !== undefined || raw.state.adc !== undefined)) {
      raw = raw.state;
    }
    raw = raw || {};
    const dev = { raw: raw };

    // 电池（mV / V）
    let mv = NaN;
    if (raw.battery_mv !== undefined) mv = toNum(raw.battery_mv, NaN);
    if (!isFinite(mv) && raw.battery !== undefined) {
      const b = toNum(raw.battery, NaN);
      if (isFinite(b)) mv = b > 20 ? b : b * 1000;
    }
    if (!isFinite(mv) && raw.vbat !== undefined) {
      const b = toNum(raw.vbat, NaN);
      if (isFinite(b)) mv = b > 20 ? b : b * 1000;
    }
    dev.batteryMv = isFinite(mv) ? Math.round(mv) : 3700;

    // 电容电压（V）
    let cap = NaN;
    if (raw.cap_voltage !== undefined) cap = toNum(raw.cap_voltage, NaN);
    else if (raw.cap !== undefined) cap = toNum(raw.cap, NaN);
    else if (raw.cap_v !== undefined) cap = toNum(raw.cap_v, NaN);
    else if (raw.cap_mv !== undefined) cap = toNum(raw.cap_mv, NaN) / 1000;
    dev.capV = isFinite(cap) ? cap : null;

    // 温度
    let temp = NaN;
    if (raw.temp !== undefined) temp = toNum(raw.temp, NaN);
    else if (raw.temp_c !== undefined) temp = toNum(raw.temp_c, NaN);
    else if (raw.temperature !== undefined) temp = toNum(raw.temperature, NaN);
    dev.temp = isFinite(temp) ? temp : null;

    dev.power = raw.power !== undefined ? !!raw.power : (raw.powered !== undefined ? !!raw.powered : null);
    dev.buttons = normalizeButtons(raw);
    dev.rf = normalizeRf(raw);
    dev.gpio = normalizeGpio(raw);
    dev.adc = normalizeAdc(raw);
    dev.charging = raw.charging !== undefined ? !!raw.charging : null;
    dev.flashCount = toInt(raw.flash_count, 0);
    dev.capMax = toNum(raw.cap_max, null);
    return dev;
  }

  function normalizeButtons(raw) {
    const out = {};
    let src = raw.buttons !== undefined ? raw.buttons : raw.keys;
    if (Array.isArray(src)) {
      src.forEach(function (k) {
        if (typeof k === 'string') out[k.toUpperCase()] = true;
        else if (k && typeof k === 'object') out[String(k.name || '').toUpperCase()] = !!k.pressed;
      });
    } else if (src && typeof src === 'object') {
      Object.keys(src).forEach(function (k) {
        const v = src[k];
        out[k.toUpperCase()] = typeof v === 'object' && v !== null ? !!v.pressed : !!v;
      });
    }
    if (raw.button !== undefined && raw.button !== null) {
      if (typeof raw.button === 'string') out[raw.button.toUpperCase()] = true;
      else if (typeof raw.button === 'object') {
        out[String(raw.button.name || '').toUpperCase()] = !!raw.button.pressed;
      }
    }
    return out;
  }

  function normalizeRf(raw) {
    let src = raw.rf !== undefined ? raw.rf : raw.radio;
    if (src && typeof src === 'object' && src.rf !== undefined) src = src.rf;
    src = (src && typeof src === 'object') ? src : {};
    // 兼容 sim/device.py 的 Device.state()：rf_rx_pending / rf_tx_count
    const pending = raw.rf_rx_pending !== undefined ? raw.rf_rx_pending : raw.rf_rx_count;
    const txCount = raw.rf_tx_count !== undefined ? raw.rf_tx_count : raw.rf_tx;
    let rxCount = 0;
    if (src.rx_count !== undefined) rxCount = toInt(src.rx_count, 0);
    else if (src.rx !== undefined) rxCount = toInt(src.rx, 0);
    else if (pending !== undefined) rxCount = toInt(pending, 0);
    return {
      rx_count: rxCount,
      tx_count: src.tx_count !== undefined ? toInt(src.tx_count, 0) : (txCount !== undefined ? toInt(txCount, 0) : 0),
      last_rx: String(src.last_rx !== undefined ? src.last_rx : (src.rx_last !== undefined ? src.rx_last : (raw.rf_rx_last || ''))),
      last_tx: String(src.last_tx !== undefined ? src.last_tx : (src.tx_last !== undefined ? src.tx_last : (raw.rf_tx_last || ''))),
    };
  }

  function maskFrom(value, bits) {
    const maskMax = bits === 2 ? 0xFF : 0xFF; // 16 个引脚
    if (value === undefined || value === null) return 0;
    if (typeof value === 'number') return (value & 0xFFFFFFFF) >>> 0;
    if (typeof value === 'string') {
      const v = parseAddr(value);
      return isFinite(v) ? v : 0;
    }
    if (Array.isArray(value)) {
      let mask = 0;
      for (let i = 0; i < value.length && i < 16; i += 1) {
        let v = toInt(value[i], 0);
        if (bits === 2) v &= 3; else v = v ? 1 : 0;
        mask |= v << (i * bits);
      }
      return mask >>> 0;
    }
    if (typeof value === 'object') {
      let mask = 0;
      Object.keys(value).forEach(function (k) {
        const i = toInt(k, -1);
        if (i < 0 || i >= 16) return;
        let v = toInt(value[k], 0);
        if (bits === 2) v &= 3; else v = v ? 1 : 0;
        mask |= v << (i * bits);
      });
      return mask >>> 0;
    }
    return 0;
  }

  function normalizeGpio(raw) {
    const ports = {};
    PORT_NAMES.forEach(function (p) { ports[p] = { moder: 0, odr: 0, idr: 0, known: false }; });
    if (!raw || typeof raw !== 'object') return { ports: ports, known: false };
    let src = raw.gpio !== undefined ? raw.gpio : (raw.GPIO !== undefined ? raw.GPIO : raw.pins);
    if (!src) {
      // 兼容 sim/device.py 的 Device.state()：
      // {gpio_inputs:{port:{pin:level}}, gpio_outputs:{port:mask}, gpio_dirs:{port:mask}}
      if (raw.gpio_inputs || raw.gpio_outputs || raw.gpio_dirs) {
        const outs = raw.gpio_outputs || {};
        const dirs = raw.gpio_dirs || {};
        const ins = raw.gpio_inputs || {};
        PORT_NAMES.forEach(function (p) {
          const prt = ports[p];
          prt.odr = toInt(outs[p], 0) & 0xFFFF;
          const dirMask = toInt(dirs[p], 0);
          if (dirMask > 0xFFFF) {
            // 32 位 MODER 风格（2 bit/引脚）
            prt.moder = dirMask & 0xFFFFFFFF;
          } else if (dirMask) {
            // 输出位掩码：展开为 MODER 的输出（01）
            for (let i = 0; i < 16; i += 1) if (dirMask & (1 << i)) prt.moder |= 1 << (i * 2);
          } else {
            // 没有方向信息时用 ODR 推断输出
            for (let i = 0; i < 16; i += 1) if (prt.odr & (1 << i)) prt.moder |= 1 << (i * 2);
          }
          const pinMap = ins[p];
          if (pinMap && typeof pinMap === 'object' && !Array.isArray(pinMap)) {
            Object.keys(pinMap).forEach(function (pinKey) {
              const pin = toInt(pinKey, -1);
              if (pin < 0 || pin > 15) return;
              if (toInt(pinMap[pinKey], 0)) prt.idr |= 1 << pin;
            });
          } else if (Array.isArray(pinMap)) {
            pinMap.forEach(function (lv, pin) { if (toInt(lv, 0)) prt.idr |= 1 << pin; });
          }
          prt.known = true;
        });
        return { ports: ports, known: true };
      }
      return { ports: ports, known: false };
    }

    function applyArray(pinList, defaultPort) {
      pinList.forEach(function (item) {
        if (!item || typeof item !== 'object') return;
        const name = String(item.port !== undefined ? item.port : (item.portName !== undefined ? item.portName : (item.bank !== undefined ? item.bank : (defaultPort || '')))).toUpperCase();
        const prt = ports[name];
        if (!prt) return;
        const pin = toInt(item.pin !== undefined ? item.pin : (item.index !== undefined ? item.index : -1), -1);
        if (pin < 0 || pin > 15) return;
        const dirRaw = String(item.direction !== undefined ? item.direction : (item.dir !== undefined ? item.dir : (item.mode !== undefined ? item.mode : ''))).toLowerCase();
        let level = 0;
        if (item.level !== undefined) level = item.level ? 1 : 0;
        else if (item.value !== undefined) level = item.value ? 1 : 0;
        else if (item.idr !== undefined) level = toInt(item.idr, 0) & 1;
        if (dirRaw === 'output' || dirRaw === 'out') prt.moder |= 1 << (pin * 2);
        else if (dirRaw === 'af' || dirRaw === 'alternate') prt.moder |= 2 << (pin * 2);
        else if (dirRaw === 'analog' || dirRaw === 'ana') prt.moder |= 3 << (pin * 2);
        if (dirRaw === 'output' || dirRaw === 'out') { if (level) prt.odr |= 1 << pin; }
        else { if (level) prt.idr |= 1 << pin; }
        prt.known = true;
      });
    }

    if (Array.isArray(src)) {
      applyArray(src, '');
      return { ports: ports, known: true };
    }
    Object.keys(src).forEach(function (key) {
      const name = String(key).toUpperCase();
      const val = src[key];
      const prt = ports[name];
      if (!prt || val === null || val === undefined) return;
      if (Array.isArray(val)) {
        // 若数组元素是对象则按引脚列表处理；否则视为电平数组
        if (val.some(function (x) { return x && typeof x === 'object'; })) {
          val.forEach(function (item) {
            if (!item || typeof item !== 'object') return;
            const o = Object.assign({}, item, { port: name });
            applyArray([o], name);
          });
        } else {
          for (let i = 0; i < val.length && i < 16; i += 1) if (toInt(val[i], 0) & 1) prt.idr |= 1 << i;
        }
        prt.known = true;
        return;
      }
      if (typeof val === 'object') {
        const o = val;
        prt.moder = maskFrom(o.moder !== undefined ? o.moder : o.mode, 2);
        prt.odr = maskFrom(o.odr !== undefined ? o.odr : o.output, 1);
        prt.idr = maskFrom(o.idr !== undefined ? o.idr : o.input, 1);
        if (o.pins) applyArray(Array.isArray(o.pins) ? o.pins : [], name);
        prt.known = true;
        return;
      }
      prt.idr = toInt(val, 0) & 0xFFFF;
      prt.known = true;
    });
    return { ports: ports, known: true };
  }

  function normalizeAdc(raw) {
    if (!raw || typeof raw !== 'object') return [];
    let src = raw.adc !== undefined ? raw.adc : (raw.adc_channels !== undefined ? raw.adc_channels : raw.channels);
    if (!src) return [];
    let list = [];
    if (Array.isArray(src)) list = src;
    else if (Array.isArray(src.channels)) list = src.channels;
    else if (typeof src === 'object') {
      list = Object.keys(src).map(function (k) { return { ch: k, value: src[k] }; });
    }
    const names = { 0: '电池电压', 1: '电容电压', 2: '温度', 3: '内部参考', 10: 'VREFINT' };
    return list.map(function (item, idx) {
      if (typeof item === 'number' || typeof item === 'string') {
        return { ch: idx, name: names[idx] || ('CH' + idx), raw: toInt(item, 0), mv: null };
      }
      item = item || {};
      const ch = toInt(item.ch !== undefined ? item.ch : (item.channel !== undefined ? item.channel : idx), idx);
      let rawVal = NaN;
      if (item.value !== undefined) rawVal = toNum(item.value, NaN);
      else if (item.raw !== undefined) rawVal = toNum(item.raw, NaN);
      else if (item.code !== undefined) rawVal = toNum(item.code, NaN);
      let mvVal = NaN;
      if (item.mv !== undefined) mvVal = toNum(item.mv, NaN);
      else if (item.millivolts !== undefined) mvVal = toNum(item.millivolts, NaN);
      else if (item.volts !== undefined) mvVal = toNum(item.volts, NaN) * 1000;
      return {
        ch: ch,
        name: String(item.name || names[ch] || ('CH' + ch)),
        raw: isFinite(rawVal) ? Math.round(rawVal) : null,
        mv: isFinite(mvVal) ? Math.round(mvVal) : null,
      };
    }).sort(function (a, b) { return a.ch - b.ch; });
  }

  function normalizeBacktrace(raw) {
    let frames = [];
    if (Array.isArray(raw)) frames = raw;
    else if (raw && Array.isArray(raw.frames)) frames = raw.frames;
    else if (raw && Array.isArray(raw.backtrace)) frames = raw.backtrace;
    else if (raw && Array.isArray(raw.stack)) frames = raw.stack;
    return frames.map(function (f, idx) {
      if (typeof f === 'number') return { index: idx, pc: f >>> 0, symbol: null, offset: 0 };
      f = f || {};
      const pc = toInt(f.pc !== undefined ? f.pc : (f.addr !== undefined ? f.addr : f.return_addr), 0) >>> 0;
      return {
        index: f.index !== undefined ? toInt(f.index, idx) : idx,
        pc: pc,
        symbol: f.symbol || f.name || f.func || null,
        offset: toInt(f.offset !== undefined ? f.offset : f.off, 0),
      };
    });
  }

  function normalizeConsole(raw) {
    if (typeof raw === 'string') return raw;
    if (raw && typeof raw.text === 'string') return raw.text;
    if (raw && Array.isArray(raw.lines)) return raw.lines.join('\n');
    if (raw && Array.isArray(raw.console)) return raw.console.join('\n');
    return '';
  }

  /* ------------------------------------------------------------------------
   * 连接状态 / 控件状态
   * ---------------------------------------------------------------------- */

  function setConnUI(on, err) {
    const dot = $('connDot');
    const text = $('connText');
    const banner = $('banner');
    if (on) {
      dot.className = 'dot on';
      text.textContent = '模拟器已连接' + (state.apiBase ? ' · ' + state.apiBase : '');
      banner.classList.add('hidden');
    } else {
      dot.className = 'dot off';
      text.textContent = '模拟器未连接';
      banner.classList.remove('hidden');
      const target = state.apiBase || '当前源';
      $('bannerText').innerHTML =
        '模拟器未连接：无法连接后端（' + esc(target) + '）。请先启动：' +
        '<code>python run_sim.py sim.server firmware/AD200pro_V1.60.bin --port 8765</code> ' +
        '或 <code>python webui/mock_server.py --port 8766</code>' +
        (err && err.message ? '。原因：' + esc(err.message) : '');
    }
    updateControlState();
  }

  function updateControlState() {
    const running = !!((state.lastStatus && state.lastStatus.state === 'running') || state.runningRequest);
    const disconnected = !state.connected;
    $('btnRun').disabled = disconnected || running;
    $('btnPause').disabled = disconnected || !running;
    $('btnStep').disabled = disconnected || running;
    $('btnStepN').disabled = disconnected || running;
    $('btnReset').disabled = disconnected;
    $('btnRefreshAll').disabled = false;
  }

  function markConnected() {
    state.failCount = 0;
    if (!state.connected) {
      state.connected = true;
      setConnUI(true);
      afterConnect();
    }
  }

  function handleConnFailure(err) {
    if (err && err.status > 0) {
      // 服务器可达，只是请求/业务错误
      markConnected();
      return;
    }
    // 运行请求进行中（或上次状态为 running）时，后端可能正忙于执行，
    // 状态轮询超时属于正常现象，不要误报“未连接”。
    if (state.runningRequest || (state.lastStatus && state.lastStatus.state === 'running')) {
      state.failCount += 1;
      return;
    }
    state.failCount += 1;
    if (state.failCount >= 2 || !state.everConnected) {
      state.connected = false;
      setConnUI(false, err);
    }
  }

  async function afterConnect() {
    const first = !state.everConnected;
    state.everConnected = true;
    await loadSymbols();
    await Promise.all([
      loadDevice(),
      loadDisasm(initialDisasmAddr()),
      loadMem(state.memAddr),
      refreshBacktrace(),
    ]);
    pollTrace();
    pollConsole();
    if (first) toast('模拟器已连接', 'ok');
  }

  function initialDisasmAddr() {
    if (state.lastStatus && isFinite(state.lastStatus.pc)) {
      return Math.max(CODE_START, state.lastStatus.pc - 0x10);
    }
    return MAIN_LOOP;
  }

  /* ------------------------------------------------------------------------
   * 轮询
   * ---------------------------------------------------------------------- */

  function startPolling() {
    if (state.timers.status === null) {
      state.timers.status = setInterval(pollStatus, POLL_STATUS_MS);
      state.timers.trace = setInterval(pollTrace, POLL_TRACE_MS);
      state.timers.device = setInterval(pollDevice, POLL_DEVICE_MS);
      state.timers.console = setInterval(pollConsole, POLL_CONSOLE_MS);
    }
  }

  async function pollStatus() {
    if (state.statusInFlight || document.hidden) return;
    state.statusInFlight = true;
    try {
      const raw = await api('/api/status', { timeout: 3000 });
      markConnected();
      applyStatus(raw);
    } catch (err) {
      handleConnFailure(err);
    } finally {
      state.statusInFlight = false;
    }
  }

  async function pollDevice() {
    if (state.deviceInFlight || document.hidden || !state.connected) return;
    state.deviceInFlight = true;
    try {
      const raw = await api('/api/device', { timeout: 4000 });
      markConnected();
      applyDevice(raw);
    } catch (err) {
      // 设备轮询失败不改变连接状态，避免噪音
    } finally {
      state.deviceInFlight = false;
    }
  }

  async function pollTrace() {
    if (state.traceInFlight || document.hidden) return;
    state.traceInFlight = true;
    try {
      const raw = await api('/api/trace?since=' + state.traceSeq + '&limit=400', { timeout: 4000 });
      markConnected();
      const events = (raw && Array.isArray(raw.events)) ? raw.events : (Array.isArray(raw) ? raw : []);
      const serverSeq = raw && typeof raw.seq === 'number' ? raw.seq : state.traceSeq;
      if (serverSeq < state.traceSeq) {
        // 后端复位：清空视图重新收集
        state.traceSeq = 0;
        state.traceBuffer = [];
        clearTraceDom();
      }
      const fresh = [];
      events.forEach(function (ev) {
        const e = normalizeEvent(ev);
        if (e.seq > state.traceSeq) {
          fresh.push(e);
          state.traceSeq = Math.max(state.traceSeq, e.seq);
        }
      });
      if (serverSeq > state.traceSeq) state.traceSeq = serverSeq;
      if (fresh.length) {
        fresh.forEach(flashFromEvent);
        state.traceBuffer = state.traceBuffer.concat(fresh);
        if (state.traceBuffer.length > TRACE_BUFFER_MAX) state.traceBuffer = state.traceBuffer.slice(-TRACE_BUFFER_MAX);
        appendTraceRows(fresh);
      }
    } catch (err) {
      handleConnFailure(err);
    } finally {
      state.traceInFlight = false;
    }
  }

  async function pollConsole() {
    if (state.consoleInFlight || document.hidden) return;
    state.consoleInFlight = true;
    try {
      const raw = await api('/api/console?since=' + state.consoleSince, { timeout: 4000 });
      markConnected();
      if (raw && typeof raw.since === 'number') state.consoleSince = raw.since;
      const text = normalizeConsole(raw);
      if (text && text !== state.consoleLastText) {
        state.consoleLastText = text;
        if (state.consoleSuppressed !== text) {
          $('consoleText').textContent = text;
          const box = $('consoleScroll');
          if (box) box.scrollTop = box.scrollHeight;
          state.consoleSuppressed = '';
        }
      }
    } catch (err) {
      // 控制台不可用时静默
    } finally {
      state.consoleInFlight = false;
    }
  }

  /* ------------------------------------------------------------------------
   * 状态应用与顶部工具条
   * ---------------------------------------------------------------------- */

  function applyStatus(raw) {
    const st = normalizeStatus(raw);
    const prev = state.lastStatus;
    const prevState = prev ? prev.state : null;
    const prevPc = prev ? prev.pc : null;

    state.lastStatus = st;
    state.lastPc = st.pc;
    if (st.breakpoints) state.breakpoints = new Set(st.breakpoints);

    renderTop(st);
    renderRegs(st.regs);
    renderNvic(st.irq);
    renderBreakpoints();

    if (st.state !== 'running') {
      if (!prev || prevPc !== st.pc || !state.disasmInsns.length || !disasmHasPc(st.pc)) {
        if (state.followPc) loadDisasm(Math.max(CODE_START, st.pc - 0x10));
      }
    }
    updateDisasmCurrent(false);
    updateMemPcHighlight();

    if (prevState !== st.state) {
      updateControlState();
      if (st.state === 'running') {
        // 运行中把焦点留在顶部
      } else if (prevState === 'running' || prevState === null) {
        refreshBacktrace();
        loadDevice();
        if (state.followPc) loadDisasm(Math.max(CODE_START, st.pc - 0x10));
      }
      if ((st.state === 'stalled' || st.state === 'fault') && prevState !== st.state) {
        toast('固件状态：' + (STATE_LABELS[st.state] || st.state) + (st.stop_reason ? '（' + stopReasonLabel(st.stop_reason) + '）' : ''), 'warn');
      }
    }
  }

  function stopReasonLabel(reason) {
    if (!reason) return '—';
    if (typeof reason === 'object') {
      const inner = reason.reason || reason.detail || reason.name || reason.text;
      if (inner) return stopReasonLabel(inner);
      try { return JSON.stringify(reason); } catch (e) { return '—'; }
    }
    const text = String(reason);
    if (STOP_REASON_LABELS[text]) return STOP_REASON_LABELS[text];
    if (/^breakpoint/i.test(text)) return '命中断点';
    if (/^stall/i.test(text)) return '外设卡死';
    if (/^fault/i.test(text)) return '固件故障';
    if (/^until_pc|^until/i.test(text)) return '到达目标 PC';
    if (/insn_limit|max_insns/i.test(text)) return '达到指令上限';
    if (/^pause/i.test(text)) return '用户暂停';
    if (/^exc_return/i.test(text)) return '中断返回';
    return text;
  }

  function renderTop(st) {
    const label = STATE_LABELS[st.state] || st.state || '未知';
    const pill = $('runState');
    pill.textContent = label;
    if (['running', 'paused', 'stalled', 'fault'].indexOf(st.state) >= 0) {
      pill.className = 'v state-pill s-' + st.state;
    } else {
      pill.className = 'v state-pill s-unknown';
    }
    const sym = resolveSymbol(st.pc);
    const pcText = $('pcText');
    if (sym) {
      pcText.textContent = sym.name + (sym.offset ? '+0x' + sym.offset.toString(16).toUpperCase() : '') + ' @ ' + hx(st.pc);
    } else if (st.pcName) {
      pcText.textContent = st.pcName + ' @ ' + hx(st.pc);
    } else {
      pcText.textContent = hx(st.pc);
    }
    pcText.title = hx(st.pc);
    $('insnText').textContent = fmtNum(st.stats.insn_count);
    $('uptimeText').textContent = fmtNum(st.stats.uptime_ms) + ' ms';
    $('stopReasonText').textContent = st.stop_reason ? '· ' + stopReasonLabel(st.stop_reason) : '';
    updateControlState();
  }

  function resolveSymbol(addr) {
    if (!isFinite(addr)) return null;
    const exact = state.symbols[addr];
    if (exact) return { name: exact, offset: 0 };
    const list = state.symbolList;
    if (!list.length) return null;
    let lo = 0;
    let hi = list.length - 1;
    let found = -1;
    while (lo <= hi) {
      const mid = (lo + hi) >> 1;
      if (list[mid].addr <= addr) { found = mid; lo = mid + 1; } else hi = mid - 1;
    }
    if (found >= 0) {
      const s = list[found];
      const off = addr - s.addr;
      if (off >= 0 && off <= 0x1000) return { name: s.name, offset: off };
    }
    return null;
  }

  /* ------------------------------------------------------------------------
   * 寄存器 / NVIC / 断点
   * ---------------------------------------------------------------------- */

  function initRegTable() {
    const body = $('regBody');
    body.innerHTML = '';
    const rows = [];
    for (let i = 0; i <= 12; i += 1) rows.push({ key: 'r' + i, label: 'R' + i });
    rows.push({ key: 'r13', label: 'SP (R13)' });
    rows.push({ key: 'r14', label: 'LR (R14)' });
    rows.push({ key: 'r15', label: 'PC (R15)' });
    rows.push({ key: 'xpsr', label: 'xPSR' });
    rows.push({ key: 'primask', label: 'PRIMASK' });
    state.regCells = {};
    rows.forEach(function (row) {
      const tr = document.createElement('tr');
      if (row.key === 'r15') tr.className = 'pc';
      const nameTd = document.createElement('td');
      nameTd.className = 'rn';
      nameTd.textContent = row.label;
      const valTd = document.createElement('td');
      valTd.className = 'rv';
      valTd.textContent = '—';
      tr.appendChild(nameTd);
      tr.appendChild(valTd);
      body.appendChild(tr);
      state.regCells[row.key] = { tr: tr, td: valTd };
    });
  }

  function renderRegs(regs) {
    if (!state.regCells) return;
    Object.keys(state.regCells).forEach(function (key) {
      const cell = state.regCells[key];
      const v = regs ? regs[key] : undefined;
      const text = (v === undefined || v === null) ? '—' : hx(v);
      if (cell.td.textContent !== text) {
        cell.td.textContent = text;
        const prev = state.regPrevValues[key];
        if (prev !== undefined && prev !== v && text !== '—') {
          cell.tr.classList.remove('changed');
          void cell.tr.offsetWidth;
          cell.tr.classList.add('changed');
        }
      }
      state.regPrevValues[key] = v;
    });
  }

  function renderNvic(irq) {
    const enabled = irq.enabled || [];
    const pending = irq.pending || [];
    const key = enabled.join(',') + '|' + pending.join(',');
    if (key === state.lastIrqKey) return;
    state.lastIrqKey = key;
    $('irqEnabledCount').textContent = String(enabled.length);
    $('irqPendingCount').textContent = String(pending.length);
    renderIrqChips($('irqEnabledList'), enabled, 'en');
    renderIrqChips($('irqPendingList'), pending, 'pend');
  }

  function renderIrqChips(container, list, cls) {
    if (!list.length) {
      container.innerHTML = '<span class="empty">无</span>';
      return;
    }
    container.innerHTML = list.map(function (irq) {
      const name = IRQ_NAMES[String(irq)] || ('IRQ' + irq);
      return '<span class="chip ' + cls + '" title="' + esc('IRQ ' + irq + ' · ' + name) + '">' +
        (irq === -1 ? 'SysTick' : irq) + '</span>';
    }).join('');
  }

  function renderBreakpoints() {
    const list = $('bpList');
    const bps = Array.from(state.breakpoints).sort(function (a, b) { return a - b; });
    $('bpCount').textContent = bps.length + ' 个';
    if (!bps.length) {
      list.innerHTML = '<li class="empty">暂无断点（在反汇编中点击指令行添加）</li>';
      return;
    }
    list.innerHTML = bps.map(function (addr) {
      const sym = resolveSymbol(addr);
      const label = sym ? (sym.name + (sym.offset ? '+0x' + sym.offset.toString(16).toUpperCase() : '')) : '';
      return '<li data-addr="' + addr + '" title="点击删除断点">' +
        '<span class="bp-mark">●</span>' +
        '<span class="bp-name">' + esc(label || '断点') + '</span>' +
        '<span class="bp-addr">' + hx(addr) + '</span>' +
        '<span class="bp-del">✕</span></li>';
    }).join('');
  }

  async function toggleBreakpoint(addr) {
    const has = state.breakpoints.has(addr);
    try {
      const res = await api('/api/breakpoints', {
        method: 'POST',
        body: has ? { remove: [addr] } : { add: [addr] },
        timeout: 5000,
      });
      const list = normalizeAddrList(res && res.breakpoints);
      if (list) state.breakpoints = new Set(list);
      else if (has) state.breakpoints.delete(addr); else state.breakpoints.add(addr);
      renderBreakpoints();
      updateDisasmBpMarks();
      toast((has ? '已删除断点 ' : '已设置断点 ') + hx(addr), 'ok');
    } catch (err) {
      toast('断点操作失败：' + err.message, 'err');
    }
  }

  /* ------------------------------------------------------------------------
   * 符号表
   * ---------------------------------------------------------------------- */

  async function loadSymbols() {
    try {
      const raw = await api('/api/symbols', { timeout: 6000 });
      const src = (raw && raw.symbols) || (raw && raw.names) || {};
      const symbols = {};
      Object.keys(src).forEach(function (key) {
        const addr = parseSymbolAddr(key);
        if (isFinite(addr)) symbols[addr] = String(src[key]);
      });
      state.symbols = symbols;
      state.symbolList = Object.keys(symbols).map(function (k) { return { addr: Number(k), name: symbols[k] }; })
        .sort(function (a, b) { return a.addr - b.addr; });
      if (raw && Array.isArray(raw.vectors)) state.vectors = raw.vectors;
      if (state.disasmInsns.length) renderDisasm();
      renderBreakpoints();
    } catch (err) {
      // 符号表缺失时前端仍可工作
    }
  }

  /* ------------------------------------------------------------------------
   * 反汇编视图
   * ---------------------------------------------------------------------- */

  async function loadDisasm(addr) {
    const a = parseAddr(typeof addr === 'number' ? addr : addr);
    if (!isFinite(a)) return;
    const token = ++state.disasmToken;
    try {
      const raw = await api('/api/disasm?addr=' + hexParam(a) + '&n=' + DISASM_COUNT, { timeout: 5000 });
      if (token !== state.disasmToken) return;
      const insns = normalizeInsns(raw);
      if (!insns.length) {
        toast('该地址没有可显示的反汇编', 'warn');
        return;
      }
      state.disasmInsns = insns;
      state.disasmBase = insns[0].addr;
      renderDisasm();
      updateDisasmCurrent(false);
      if (state.followPc) $('disasmAddr').value = hx(insns[0].addr);
    } catch (err) {
      if (token === state.disasmToken) toast('读取反汇编失败：' + err.message, 'err');
    }
  }

  function renderDisasm() {
    const body = $('disasmBody');
    const insns = state.disasmInsns;
    state.disasmRows = {};
    if (!insns.length) {
      body.innerHTML = '<tr><td colspan="4" class="t-empty">（无数据）</td></tr>';
      return;
    }
    const html = insns.map(function (insn) {
      const bp = state.breakpoints.has(insn.addr);
      const symName = state.symbols[insn.addr];
      const bytesText = insn.bytes.length
        ? insn.bytes.map(function (b) { return b.toString(16).toUpperCase().padStart(2, '0'); }).join(' ')
        : '';
      let textHtml = esc(insn.text);
      if (symName) textHtml = '<span class="symname">' + esc(symName) + ':</span> ' + textHtml;
      return '<tr data-addr="' + insn.addr + '" class="' + (bp ? 'has-bp' : '') + '">' +
        '<td class="c-bp">' + (bp ? '●' : '') + '</td>' +
        '<td class="c-addr">' + hx(insn.addr) + '</td>' +
        '<td class="c-bytes">' + esc(bytesText) + '</td>' +
        '<td class="c-text">' + textHtml + '</td></tr>';
    }).join('');
    body.innerHTML = html;
    Array.prototype.forEach.call(body.querySelectorAll('tr[data-addr]'), function (tr) {
      state.disasmRows[Number(tr.getAttribute('data-addr'))] = tr;
    });
    updateDisasmCurrent(false);
  }

  function disasmHasPc(pc) {
    return state.disasmInsns.some(function (i) { return i.addr === pc; });
  }

  function updateDisasmCurrent(scroll) {
    if (!state.lastStatus) return;
    const pc = state.lastStatus.pc;
    const rows = state.disasmRows;
    Object.keys(rows).forEach(function (addr) {
      const tr = rows[addr];
      if (Number(addr) === pc) tr.classList.add('current');
      else tr.classList.remove('current');
    });
    if (scroll && state.followPc && state.lastHighlightPc !== pc && rows[pc]) {
      rows[pc].scrollIntoView({ block: 'center' });
    }
    if (state.lastHighlightPc !== pc) state.lastHighlightPc = pc;
  }

  function updateDisasmBpMarks() {
    Object.keys(state.disasmRows).forEach(function (addr) {
      const tr = state.disasmRows[addr];
      const bp = state.breakpoints.has(Number(addr));
      tr.classList.toggle('has-bp', bp);
      const cell = tr.querySelector('.c-bp');
      if (cell) cell.textContent = bp ? '●' : '';
    });
  }

  /* ------------------------------------------------------------------------
   * 内存视图
   * ---------------------------------------------------------------------- */

  async function loadMem(addr) {
    const a = parseAddr(typeof addr === 'number' ? addr : addr);
    if (!isFinite(a)) return;
    const token = ++state.memToken;
    try {
      const raw = await api('/api/mem?addr=' + hexParam(a) + '&len=' + MEM_LEN, { timeout: 5000 });
      if (token !== state.memToken) return;
      const base = raw && raw.addr !== undefined ? (typeof raw.addr === 'string' ? parseAddr(raw.addr) : toInt(raw.addr, a)) : a;
      const bytes = normalizeBytes(raw && raw.bytes !== undefined ? raw.bytes : raw);
      if (!bytes.length) {
        toast('该地址没有可显示的内存数据', 'warn');
        return;
      }
      state.memAddr = base >>> 0;
      state.memBytes = bytes;
      renderMem();
      $('memAddr').value = hx(state.memAddr);
    } catch (err) {
      if (token === state.memToken) toast('读取内存失败：' + err.message, 'err');
    }
  }

  function renderMem() {
    const body = $('memBody');
    const bytes = state.memBytes;
    state.memCellMap = {};
    const rows = Math.ceil(bytes.length / 16);
    let html = '';
    for (let r = 0; r < rows; r += 1) {
      const rowAddr = state.memAddr + r * 16;
      html += '<tr data-rowaddr="' + rowAddr + '"><td class="c-addr">' + hx(rowAddr) + '</td>';
      let ascii = '';
      for (let c = 0; c < 16; c += 1) {
        const idx = r * 16 + c;
        if (idx >= bytes.length) {
          html += '<td class="c-hex"></td>';
          ascii += ' ';
          continue;
        }
        const b = bytes[idx];
        const addr = rowAddr + c;
        html += '<td class="c-hex' + (b === 0 ? ' zero' : '') + '" data-addr="' + addr + '" title="' + hx(addr) + '">' +
          b.toString(16).toUpperCase().padStart(2, '0') + '</td>';
        ascii += (b >= 32 && b < 127) ? String.fromCharCode(b) : '.';
      }
      html += '<td class="c-ascii">' + esc(ascii) + '</td></tr>';
    }
    body.innerHTML = html;
    Array.prototype.forEach.call(body.querySelectorAll('td.c-hex[data-addr]'), function (td) {
      state.memCellMap[Number(td.getAttribute('data-addr'))] = td;
    });
    updateMemPcHighlight();
  }

  function updateMemPcHighlight() {
    if (!state.lastStatus) return;
    const pc = state.lastStatus.pc;
    const old = state.memCellMap;
    const inRange = pc >= state.memAddr && pc < state.memAddr + state.memBytes.length;
    if (state.memPcCell && state.memPcCell.classList) state.memPcCell.classList.remove('pc-hit');
    if (inRange && old[pc]) {
      old[pc].classList.add('pc-hit');
      state.memPcCell = old[pc];
    } else {
      state.memPcCell = null;
    }
  }

  function beginMemEdit(td) {
    if (!td) return;
    if (state.memEditing) return;
    const addr = Number(td.getAttribute('data-addr'));
    if (!isFinite(addr)) return;
    const original = td.textContent;
    td.classList.add('editing');
    const input = document.createElement('input');
    input.className = 'mem-editor';
    input.type = 'text';
    input.maxLength = 2;
    input.spellcheck = false;
    input.value = original;
    td.textContent = '';
    td.appendChild(input);
    input.focus();
    input.select();
    state.memEditing = { td: td, input: input, addr: addr, original: original, done: false };
    function finish(commit) {
      const editing = state.memEditing;
      if (!editing || editing.done) return;
      editing.done = true;
      state.memEditing = null;
      const text = input.value.trim();
      const value = /^[0-9a-fA-F]{1,2}$/.test(text) ? parseInt(text, 16) : NaN;
      td.classList.remove('editing');
      td.textContent = original;
      if (commit && isFinite(value)) {
        writeMemByte(addr, value);
      }
    }
    input.addEventListener('keydown', function (e) {
      if (e.key === 'Enter') { e.preventDefault(); finish(true); }
      else if (e.key === 'Escape') { e.preventDefault(); finish(false); }
    });
    input.addEventListener('blur', function () { finish(true); });
  }

  async function writeMemByte(addr, value) {
    try {
      await api('/api/mem', { method: 'POST', body: { addr: addr, bytes: [value] }, timeout: 5000 });
      const idx = addr - state.memAddr;
      if (idx >= 0 && idx < state.memBytes.length) {
        state.memBytes[idx] = value;
        renderMem();
      }
      toast('已写入 ' + hx(addr) + ' = 0x' + value.toString(16).toUpperCase().padStart(2, '0'), 'ok');
    } catch (err) {
      toast('写入内存失败：' + err.message, 'err');
      loadMem(state.memAddr);
    }
  }

  /* ------------------------------------------------------------------------
   * 虚拟整机面板
   * ---------------------------------------------------------------------- */

  function applyDevice(raw) {
    state.device = normalizeDevice(raw);
    state.deviceLoaded = true;
    renderDevice();
  }

  async function loadDevice() {
    try {
      const raw = await api('/api/device', { timeout: 5000 });
      applyDevice(raw);
    } catch (err) {
      // 设备端点不可用时静默
    }
  }

  function renderDevice() {
    const dev = state.device;
    if (!dev) return;
    const t = now();
    if (t - state.sliderHold.battery > 1200) {
      const mv = Math.round(dev.batteryMv);
      const el = $('batterySlider');
      if (Number(el.value) !== mv) el.value = String(mv);
    }
    if (t - state.sliderHold.cap > 1200 && dev.capV !== null) {
      const el = $('capSlider');
      const v = Math.round(dev.capV);
      if (Number(el.value) !== v) el.value = String(v);
    }
    if (t - state.sliderHold.temp > 1200 && dev.temp !== null) {
      const el = $('tempSlider');
      const v = Math.round(dev.temp * 2) / 2;
      if (Number(el.value) !== v) el.value = String(v);
    }
    updateSliderLabels();
    const powerEl = $('powerState');
    let powerText;
    if (dev.power === null && dev.charging) powerText = '充电中';
    else if (dev.power === null) powerText = '电源 —';
    else powerText = '电源 ' + (dev.power ? '开' : '关') + (dev.charging ? ' · 充电中' : '');
    powerEl.textContent = powerText;
    powerEl.style.color = dev.charging ? 'var(--green)' : (dev.power === null ? '' : (dev.power ? 'var(--green)' : 'var(--red)'));

    const rf = dev.rf;
    const lastRx = rf.last_rx || state.lastRfInjected || '—';
    let rfLine = 'RX ×' + rf.rx_count + ' · ' + lastRx + '\n' +
      'TX ×' + rf.tx_count + ' · ' + (rf.last_tx || '—');
    if (dev.flashCount) rfLine += '\n闪光 ×' + dev.flashCount;
    $('rfInfo').textContent = rfLine;

    // 后端设备模型里的按键状态（例如外部注入或固件侧状态）同步到按钮外观
    if (state.padButtons) {
      BUTTON_DEFS.forEach(function (def) {
        const btn = state.padButtons[def.name];
        if (!btn) return;
        let down = !!dev.buttons[def.name];
        // 真实后端的 Device.buttons 使用 MODE；界面按钮叫 MENU
        if (!down && def.name === 'MENU' && dev.buttons.MODE) down = true;
        btn.classList.toggle('held', down);
      });
    }

    renderGpio(dev.gpio);
    renderAdc(dev.adc);
  }

  function updateSliderLabels() {
    const mv = toInt($('batterySlider').value, 3700);
    $('batteryVal').textContent = mv + ' mV';
    $('batteryVal').title = (mv / 1000).toFixed(3) + ' V';
    const cap = toInt($('capSlider').value, 0);
    $('capVal').textContent = cap + ' V';
    const temp = toNum($('tempSlider').value, 25);
    $('tempVal').textContent = temp.toFixed(1) + ' °C';
  }

  function scheduleDevicePost(key, bodyFactory) {
    clearTimeout(state.deviceTimers[key]);
    state.deviceTimers[key] = setTimeout(async function () {
      try {
        const raw = await api('/api/device', { method: 'POST', body: bodyFactory(), timeout: 4000 });
        applyDeviceResponse(raw);
      } catch (err) {
        // 滑条连续拖动时静默失败，避免刷屏
      }
    }, 200);
  }

  function initSliders() {
    $('batterySlider').addEventListener('input', function () {
      state.sliderHold.battery = now();
      updateSliderLabels();
      const mv = toInt($('batterySlider').value, 3700);
      // 同时携带 V 与 mV 两种字段名，兼容不同后端的 device 设置接口
      scheduleDevicePost('battery', function () { return { battery: mv / 1000, battery_mv: mv }; });
    });
    $('capSlider').addEventListener('input', function () {
      state.sliderHold.cap = now();
      updateSliderLabels();
      const v = toNum($('capSlider').value, 0);
      scheduleDevicePost('cap', function () { return { cap_voltage: v }; });
    });
    $('tempSlider').addEventListener('input', function () {
      state.sliderHold.temp = now();
      updateSliderLabels();
      const v = toNum($('tempSlider').value, 25);
      scheduleDevicePost('temp', function () { return { temp: v, temp_c: v }; });
    });
    updateSliderLabels();
  }

  function initButtonPad() {
    const pad = $('buttonPad');
    pad.innerHTML = '';
    state.padButtons = {};
    BUTTON_DEFS.forEach(function (def) {
      const btn = document.createElement('button');
      btn.type = 'button';
      btn.className = 'pad-btn';
      btn.textContent = def.label;
      btn.title = '注入按键 ' + def.name + '（按住产生按下事件）';
      state.padButtons[def.name] = btn;
      let pressed = false;
      function press() {
        if (pressed) return;
        pressed = true;
        btn.classList.add('pressed');
        sendDeviceButton(def.name, true);
      }
      function release() {
        if (!pressed) return;
        pressed = false;
        btn.classList.remove('pressed');
        sendDeviceButton(def.name, false);
      }
      btn.addEventListener('pointerdown', function (e) { e.preventDefault(); press(); });
      btn.addEventListener('pointerup', function (e) { e.preventDefault(); release(); });
      btn.addEventListener('pointerleave', release);
      btn.addEventListener('pointercancel', release);
      btn.addEventListener('keydown', function (e) {
        if (e.key === ' ' || e.key === 'Enter') { e.preventDefault(); press(); }
      });
      btn.addEventListener('keyup', function (e) {
        if (e.key === ' ' || e.key === 'Enter') { e.preventDefault(); release(); }
      });
      pad.appendChild(btn);
    });
  }

  async function sendDeviceButton(name, pressed) {
    try {
      const raw = await api('/api/device/event', {
        method: 'POST',
        body: { kind: 'button', payload: { name: name, pressed: pressed } },
        timeout: 4000,
      });
      applyDeviceResponse(raw);
    } catch (err) {
      toast('按键事件发送失败：' + err.message, 'err');
    }
  }

  function parseHexText(text) {
    const clean = String(text || '').replace(/0x/gi, ' ').replace(/[^0-9a-fA-F]/g, '');
    if (!clean.length) return { bytes: [], hex: '' };
    if (clean.length % 2 !== 0) return null;
    const bytes = [];
    for (let i = 0; i < clean.length; i += 2) bytes.push(parseInt(clean.slice(i, i + 2), 16));
    return { bytes: bytes, hex: bytes.map(function (b) { return b.toString(16).toUpperCase().padStart(2, '0'); }).join(' ') };
  }

  async function sendRf() {
    const parsed = parseHexText($('rfHex').value);
    if (!parsed) {
      toast('RF 数据不是合法的十六进制（每字节两位，如 01 23 45 67）', 'warn');
      return;
    }
    if (!parsed.bytes.length) {
      toast('请输入要注入的 RF 接收数据', 'warn');
      return;
    }
    try {
      const raw = await api('/api/device/event', {
        method: 'POST',
        body: { kind: 'rf_rx', payload: { bytes: parsed.bytes, hex: parsed.hex } },
        timeout: 5000,
      });
      applyDeviceResponse(raw);
      state.lastRfInjected = parsed.hex;
      renderDevice();
      toast('已注入 RF 接收数据：' + parsed.hex, 'ok');
    } catch (err) {
      toast('RF 发送失败：' + err.message, 'err');
    }
  }

  /* ------------------------------------------------------------------------
   * GPIO / ADC 渲染
   * ---------------------------------------------------------------------- */

  function initGpioTable() {
    const container = $('gpioTable');
    container.innerHTML = '';
    state.gpioCells = {};
    PORT_NAMES.forEach(function (p) {
      const block = document.createElement('div');
      block.className = 'gpio-port';
      const head = document.createElement('div');
      head.className = 'gpio-head';
      head.innerHTML = '<span class="port-name">P' + p + '</span>' +
        '<span class="regs" id="gpioRegs' + p + '">MODER — · ODR — · IDR —</span>';
      const pins = document.createElement('div');
      pins.className = 'gpio-pins';
      for (let i = 15; i >= 0; i -= 1) {
        const cell = document.createElement('div');
        cell.className = 'gpio-pin';
        cell.setAttribute('data-port', p);
        cell.setAttribute('data-pin', String(i));
        cell.innerHTML = '<span class="pin-n">' + i + '</span>';
        state.gpioCells[p + i] = cell;
        pins.appendChild(cell);
      }
      block.appendChild(head);
      block.appendChild(pins);
      container.appendChild(block);
    });
  }

  function renderGpio(gpio) {
    if (!state.gpioCells) return;
    const ports = gpio && gpio.ports ? gpio.ports : null;
    const known = !!(gpio && gpio.known);
    PORT_NAMES.forEach(function (p) {
      const prt = ports ? ports[p] : null;
      const regsEl = $('gpioRegs' + p);
      if (regsEl) {
        regsEl.textContent = (known && prt && prt.known)
          ? 'MODER ' + hx16(prt.moder) + ' · ODR ' + hx16(prt.odr) + ' · IDR ' + hx16(prt.idr)
          : 'MODER — · ODR — · IDR —';
      }
      for (let i = 15; i >= 0; i -= 1) {
        const cell = state.gpioCells[p + i];
        if (!cell) continue;
        let dirCode = 0;
        let level = 0;
        const has = !!(known && prt && prt.known);
        if (has) {
          dirCode = (prt.moder >>> (i * 2)) & 3;
          if (dirCode === 1) level = (prt.odr >>> i) & 1;
          else if (dirCode === 0) level = (prt.idr >>> i) & 1;
        }
        const dirClass = dirCode === 1 ? 'dir-out' : dirCode === 2 ? 'dir-af' : dirCode === 3 ? 'dir-ana' : 'dir-in';
        cell.className = 'gpio-pin ' + dirClass + (level ? ' hi' : '') + (has ? '' : ' unknown');
        const dirText = dirCode === 1 ? '输出' : dirCode === 2 ? '复用' : dirCode === 3 ? '模拟' : '输入';
        cell.title = 'P' + p + i + ' · ' + dirText + ' · ' + (level ? '高电平' : '低电平') + (has ? '' : ' · 后端无数据');
      }
    });
  }

  function flashFromEvent(ev) {
    if (ev.type !== 'gpio_change') return;
    const data = ev.data;
    let port = null;
    let pin = null;
    if (data && typeof data === 'object') {
      port = data.port !== undefined ? String(data.port).toUpperCase() : null;
      pin = data.pin !== undefined ? toInt(data.pin, -1) : null;
    }
    if ((port === null || pin === null)) {
      // 无结构化数据时尝试从地址推断端口
      if (ev.addr !== null && ev.addr !== undefined) {
        const off = ev.addr - 0x48000000;
        const banks = [['A', 0x000], ['B', 0x400], ['C', 0x800], ['D', 0xC00], ['F', 0x1400]];
        for (let i = 0; i < banks.length; i += 1) {
          if (off >= banks[i][1] && off < banks[i][1] + 0x400) { port = banks[i][0]; break; }
        }
        if (pin === null && data && typeof data === 'object' && data.mask !== undefined) {
          const mask = toInt(data.mask, 0);
          if (mask) pin = Math.floor(Math.log(mask & -mask) / Math.LN2);
        }
      }
    }
    if (port === null || pin === null || pin < 0 || pin > 15) return;
    const cell = state.gpioCells && state.gpioCells[port + pin];
    if (!cell) return;
    cell.classList.remove('flash');
    void cell.offsetWidth;
    cell.classList.add('flash');
    setTimeout(function () { cell.classList.remove('flash'); }, 750);
  }

  function renderAdc(channels) {
    const list = $('adcList');
    if (!channels || !channels.length) {
      list.innerHTML = '<div class="bt-empty">后端未提供 ADC 通道数据</div>';
      $('adcHint').textContent = '—';
      return;
    }
    $('adcHint').textContent = channels.length + ' 通道 · 原始值 / 电压';
    list.innerHTML = channels.map(function (ch) {
      const raw = ch.raw === null ? 0 : ch.raw;
      const pct = Math.max(0, Math.min(100, (raw / 4095) * 100));
      let mvText = '—';
      if (ch.mv !== null) mvText = ch.mv + ' mV';
      else if (ch.raw !== null) mvText = Math.round((raw / 4095) * 3300) + ' mV';
      return '<div class="adc-row">' +
        '<span class="adc-ch">CH' + ch.ch + '</span>' +
        '<div class="adc-main">' +
        '<div class="adc-line"><span class="adc-name" title="' + esc(ch.name) + '">' + esc(ch.name) + '</span>' +
        '<span class="adc-val">' + (ch.raw === null ? '—' : ch.raw) + '<small>' + esc(mvText) + '</small></span></div>' +
        '<div class="adc-bar"><i style="width:' + pct.toFixed(1) + '%"></i></div></div>' +
        '</div>';
    }).join('');
  }

  /* ------------------------------------------------------------------------
   * Trace 日志
   * ---------------------------------------------------------------------- */

  function groupOfType(type) {
    return TYPE_TO_GROUP[type] || 'other';
  }

  function passTraceFilter(ev) {
    return state.traceFilters.has(groupOfType(ev.type));
  }

  function initTraceFilters() {
    const box = $('traceFilters');
    box.innerHTML = TRACE_GROUPS.map(function (g) {
      const on = state.traceFilters.has(g.key);
      return '<label class="filter-chip ' + g.cls + (on ? '' : ' off') + '" data-key="' + g.key + '" title="显示/隐藏 ' + g.label + ' 事件">' + g.label + '</label>';
    }).join('');
    box.addEventListener('click', function (e) {
      const chip = e.target.closest ? e.target.closest('.filter-chip') : null;
      if (!chip) return;
      const key = chip.getAttribute('data-key');
      if (state.traceFilters.has(key)) state.traceFilters.delete(key);
      else state.traceFilters.add(key);
      chip.classList.toggle('off', !state.traceFilters.has(key));
      saveFilters();
      renderTraceAll();
    });
  }

  function saveFilters() {
    try { localStorage.setItem(LS_FILTERS, JSON.stringify(Array.from(state.traceFilters))); } catch (e) { /* ignore */ }
  }

  function restoreFilters() {
    try {
      const raw = localStorage.getItem(LS_FILTERS);
      if (!raw) return;
      const arr = JSON.parse(raw);
      if (Array.isArray(arr)) {
        const valid = arr.filter(function (k) { return ALL_GROUP_KEYS.indexOf(k) >= 0; });
        state.traceFilters = new Set(valid);
      }
    } catch (e) { /* ignore */ }
  }

  function clearTraceDom() {
    $('traceLog').innerHTML = '<div class="t-empty">（暂无 trace 事件）</div>';
  }

  function traceRowHtml(ev) {
    const badge = TYPE_BADGE[ev.type] || [ev.type, 't-other'];
    const desc = describeEvent(ev);
    const sym = resolveSymbol(ev.pc);
    const pcLabel = sym ? (sym.name + (sym.offset ? '+0x' + sym.offset.toString(16).toUpperCase() : '')) : hx(ev.pc);
    return '<div class="trace-row" data-seq="' + ev.seq + '">' +
      '<span class="t-seq">#' + ev.seq + '</span>' +
      '<span class="t-badge ' + badge[1] + '">' + esc(badge[0]) + '</span>' +
      '<span class="t-pc" title="' + esc(hx(ev.pc)) + '">' + esc(pcLabel) + '</span>' +
      '<span class="t-main" title="' + esc(desc.plain) + '">' + desc.html + '</span>' +
      '</div>';
  }

  function appendTraceRows(events) {
    const log = $('traceLog');
    const empty = log.querySelector('.t-empty');
    if (empty) empty.remove();
    const html = events.filter(passTraceFilter).map(traceRowHtml).join('');
    if (html) log.insertAdjacentHTML('beforeend', html);
    while (log.children.length > TRACE_DOM_MAX) log.removeChild(log.firstChild);
    if (state.traceAutoScroll) {
      const box = $('traceScroll');
      if (box) box.scrollTop = box.scrollHeight;
    }
  }

  function renderTraceAll() {
    const log = $('traceLog');
    log.innerHTML = '';
    const events = state.traceBuffer.filter(passTraceFilter);
    if (!events.length) {
      log.innerHTML = '<div class="t-empty">（当前过滤条件下暂无 trace 事件）</div>';
      return;
    }
    log.innerHTML = events.slice(-TRACE_DOM_MAX).map(traceRowHtml).join('');
    if (state.traceAutoScroll) {
      const box = $('traceScroll');
      if (box) box.scrollTop = box.scrollHeight;
    }
  }

  function describeEvent(ev) {
    const nameHtml = ev.name ? '<span class="sym">' + esc(ev.name) + '</span>' : '';
    const addrHtml = (ev.addr !== null && ev.addr !== undefined) ? '<span class="reg">' + hx(ev.addr) + '</span>' : '';
    const data = ev.data;

    function dataHtml() {
      if (data === null || data === undefined) return '';
      if (typeof data === 'number') return '<span class="val">' + hx(data) + '</span>';
      if (typeof data === 'string') return esc(data);
      if (Array.isArray(data)) {
        return '<span class="val">' + esc(data.map(function (b) {
          return typeof b === 'number' ? ('0x' + (b & 0xFF).toString(16).toUpperCase().padStart(2, '0')) : String(b);
        }).join(' ')) + '</span>';
      }
      if (typeof data === 'object') {
        if (data.irq !== undefined) {
          return 'IRQ ' + esc(String(data.irq)) + ' (' + esc(IRQ_NAMES[String(data.irq)] || '') + ')';
        }
        if (data.port !== undefined && data.pin !== undefined) {
          return 'P' + esc(String(data.port)) + esc(String(data.pin)) +
            (data.value !== undefined ? ' → ' + (data.value ? '高' : '低') : '');
        }
        if (data.hex !== undefined) return '<span class="val">' + esc(String(data.hex)) + '</span>';
        if (data.addr !== undefined && data.len !== undefined) {
          return esc(String(data.addr)) + ' × ' + esc(String(data.len)) + ' 字节';
        }
        try { return esc(JSON.stringify(data)); } catch (e) { return ''; }
      }
      return esc(String(data));
    }

    function dataPlain() {
      if (data === null || data === undefined) return '';
      if (typeof data === 'number') return hx(data);
      if (typeof data === 'string') return data;
      if (Array.isArray(data)) return data.map(function (b) { return typeof b === 'number' ? ('0x' + (b & 0xFF).toString(16).toUpperCase().padStart(2, '0')) : String(b); }).join(' ');
      if (typeof data === 'object') {
        if (data.irq !== undefined) return 'IRQ ' + String(data.irq);
        if (data.port !== undefined && data.pin !== undefined) return 'P' + String(data.port) + String(data.pin) + (data.value !== undefined ? (data.value ? ' 高' : ' 低') : '');
        if (data.hex !== undefined) return String(data.hex);
        try { return JSON.stringify(data); } catch (e) { return ''; }
      }
      return String(data);
    }

    function strip(html) { return html.replace(/<[^>]*>/g, ''); }
    let html;
    let plain = '';
    const dHtml = dataHtml();
    const dPlain = dataPlain();
    switch (ev.type) {
      case 'mmio_read':
        html = (nameHtml || addrHtml) + ' 读取 = ' + dHtml;
        plain = ((ev.name || (ev.addr !== null ? hx(ev.addr) : '')) + ' 读取 = ' + dPlain).trim();
        break;
      case 'mmio_write':
        html = (nameHtml || addrHtml) + ' 写入 ' + dHtml;
        plain = ((ev.name || (ev.addr !== null ? hx(ev.addr) : '')) + ' 写入 ' + dPlain).trim();
        break;
      case 'call':
        html = '调用 ' + (nameHtml || addrHtml || esc(dPlain));
        plain = '调用 ' + (ev.name || (ev.addr !== null ? hx(ev.addr) : dPlain));
        break;
      case 'ret':
        html = '返回 ' + (nameHtml || dHtml);
        plain = '返回 ' + (ev.name || dPlain);
        break;
      case 'irq_enter':
        html = '进入中断 ' + (nameHtml || '') + (dHtml ? ' ' + dHtml : '');
        plain = '进入中断 ' + (ev.name || '') + ' ' + dPlain;
        break;
      case 'irq_exit':
        html = '退出中断 ' + (nameHtml || '') + (dHtml ? ' ' + dHtml : '');
        plain = '退出中断 ' + (ev.name || '') + ' ' + dPlain;
        break;
      case 'exception':
        html = '异常 ' + (nameHtml || '') + (dHtml ? ' ' + dHtml : '');
        plain = '异常 ' + (ev.name || '') + ' ' + dPlain;
        break;
      case 'gpio_change':
        html = '<span class="sym">' + esc(ev.name || 'GPIO') + '</span> ' + dHtml;
        plain = (ev.name || 'GPIO') + ' ' + dPlain;
        break;
      case 'unimpl':
        html = '未实现访问 ' + addrHtml + (dHtml ? ' · ' + dHtml : '');
        plain = '未实现访问 ' + (ev.addr !== null ? hx(ev.addr) : '') + ' ' + dPlain;
        break;
      case 'fault':
        html = '<span class="sym">故障</span> ' + (nameHtml || '') + (dHtml ? ' · ' + dHtml : '');
        plain = '故障 ' + (ev.name || '') + ' ' + dPlain;
        break;
      case 'uart_tx':
      case 'uart_rx':
        html = (ev.type === 'uart_tx' ? '串口发送 ' : '串口接收 ') + (nameHtml || '') + (dHtml ? ' · ' + dHtml : '');
        plain = (ev.type === 'uart_tx' ? '串口发送 ' : '串口接收 ') + (ev.name || '') + ' ' + dPlain;
        break;
      default:
        html = (nameHtml || ev.type) + (addrHtml && !ev.name ? ' ' + addrHtml : '') + (dHtml ? ' · ' + dHtml : '');
        plain = (ev.name || ev.type) + ' ' + (ev.addr !== null ? hx(ev.addr) : '') + ' ' + dPlain;
    }
    // 进一步限制长度，避免 DOM 过宽
    if (plain.length > 160) plain = plain.slice(0, 157) + '…';
    return { html: html, plain: plain.replace(/\s+/g, ' ').trim(), text: strip(html) };
  }

  /* ------------------------------------------------------------------------
   * 调用栈 / 控制台
   * ---------------------------------------------------------------------- */

  async function refreshBacktrace() {
    try {
      const raw = await api('/api/backtrace', { timeout: 4000 });
      const frames = normalizeBacktrace(raw);
      renderBacktrace(frames);
    } catch (err) {
      renderBacktrace([]);
    }
  }

  function renderBacktrace(frames) {
    const box = $('backtraceList');
    if (!frames || !frames.length) {
      box.innerHTML = '<div class="bt-empty">暂无调用栈（后端未提供或栈为空）</div>';
      return;
    }
    box.innerHTML = frames.map(function (f) {
      const sym = f.symbol ? { name: f.symbol, offset: f.offset } : resolveSymbol(f.pc);
      const name = sym ? sym.name : '??';
      const off = sym && sym.offset ? '+0x' + sym.offset.toString(16).toUpperCase() : '';
      return '<div class="bt-frame" data-pc="' + f.pc + '" title="点击跳到该地址反汇编">' +
        '<span class="fr-idx">#' + f.index + '</span>' +
        '<span class="fr-addr">' + hx(f.pc) + '</span>' +
        '<span class="fr-sym">' + esc(name) + '</span>' +
        '<span class="fr-off">' + esc(off) + '</span></div>';
    }).join('');
  }

  /* ------------------------------------------------------------------------
   * 动作：复位 / 运行 / 暂停 / 单步
   * ---------------------------------------------------------------------- */

  async function doReset() {
    if (!state.connected) { toast('模拟器未连接，无法复位', 'warn'); return; }
    try {
      const raw = await api('/api/reset', { method: 'POST', timeout: 20000 });
      markConnected();
      state.traceSeq = 0;
      state.traceBuffer = [];
      clearTraceDom();
      state.lastHighlightPc = null;
      applyActionStatus(raw);
      state.consoleLastText = '';
      state.consoleSuppressed = '';
      await Promise.all([loadDisasm(Math.max(CODE_START, (state.lastStatus ? state.lastStatus.pc : RESET_PC) - 0x10)), loadDevice(), refreshBacktrace(), loadMem(state.memAddr)]);
      pollTrace();
      toast('固件已复位', 'ok');
    } catch (err) {
      if (err.status === 0) handleConnFailure(err);
      toast('复位失败：' + err.message, 'err');
    }
  }

  async function doRun() {
    if (!state.connected) { toast('模拟器未连接，无法运行', 'warn'); return; }
    if (state.runningRequest) return;
    state.runningRequest = true;
    updateControlState();
    try {
      const raw = await api('/api/run', { method: 'POST', body: { max_insns: state.maxInsns }, timeout: 120000 });
      markConnected();
      applyActionStatus(raw);
      const running = state.lastStatus && state.lastStatus.state === 'running';
      const reason = (raw && (raw.stop_reason || raw.stopReason)) || (state.lastStatus && state.lastStatus.stop_reason);
      if (running) {
        toast('已开始运行（后端异步执行中，状态自动刷新）', 'ok');
      } else {
        toast('运行结束：' + stopReasonLabel(reason) + ' · PC ' + hx(state.lastStatus ? state.lastStatus.pc : 0), 'ok');
      }
      refreshAfterStop();
    } catch (err) {
      if (err.status === 0) handleConnFailure(err);
      toast('运行请求失败：' + err.message + '（后端若仍在运行，可稍候刷新）', 'err');
    } finally {
      state.runningRequest = false;
      updateControlState();
    }
  }

  async function doPause() {
    if (!state.connected) { toast('模拟器未连接，无法暂停', 'warn'); return; }
    try {
      const raw = await api('/api/pause', { method: 'POST', timeout: 20000 });
      markConnected();
      applyActionStatus(raw);
      refreshAfterStop();
      toast('已暂停 · PC ' + hx(state.lastStatus ? state.lastStatus.pc : 0), 'ok');
    } catch (err) {
      if (err.status === 0) handleConnFailure(err);
      toast('暂停失败：' + err.message, 'err');
    }
  }

  async function doStep(n) {
    if (!state.connected) { toast('模拟器未连接，无法单步', 'warn'); return; }
    const count = Math.max(1, Math.min(1000000, toInt(n, 1)));
    try {
      const raw = await api('/api/step', { method: 'POST', body: { n: count }, timeout: 60000 });
      markConnected();
      applyActionStatus(raw);
      if (state.followPc && state.lastStatus) {
        loadDisasm(Math.max(CODE_START, state.lastStatus.pc - 0x10));
      }
      updateMemPcHighlight();
      refreshBacktrace();
    } catch (err) {
      if (err.status === 0) handleConnFailure(err);
      toast('单步失败：' + err.message, 'err');
    }
  }

  function doStepN() {
    const n = toInt($('stepN').value, 100);
    if (!isFinite(n) || n < 1) { toast('请输入合法的步进条数', 'warn'); return; }
    doStep(n);
  }

  function refreshAfterStop() {
    if (state.followPc && state.lastStatus) loadDisasm(Math.max(CODE_START, state.lastStatus.pc - 0x10));
    refreshBacktrace();
    loadDevice();
  }

  async function refreshAll() {
    await pollStatus();
    await loadSymbols();
    if (state.lastStatus && state.followPc) loadDisasm(Math.max(CODE_START, state.lastStatus.pc - 0x10));
    else loadDisasm(parseAddr($('disasmAddr').value) || MAIN_LOOP);
    loadMem(parseAddr($('memAddr').value) || state.memAddr);
    loadDevice();
    refreshBacktrace();
    pollTrace();
    pollConsole();
  }

  /* ------------------------------------------------------------------------
   * 弹窗 / 设置
   * ---------------------------------------------------------------------- */

  function closePopovers() {
    $('settingsPop').classList.add('hidden');
    $('helpPop').classList.add('hidden');
  }

  function toggleHelp() {
    const help = $('helpPop');
    const show = help.classList.contains('hidden');
    closePopovers();
    if (show) help.classList.remove('hidden');
  }

  function toggleSettings() {
    const pop = $('settingsPop');
    const show = pop.classList.contains('hidden');
    closePopovers();
    if (show) {
      $('apiBase').value = state.apiBase;
      pop.classList.remove('hidden');
    }
  }

  function applyApiBase() {
    const value = $('apiBase').value.trim().replace(/\/+$/, '');
    if (value && !/^https?:\/\//i.test(value)) {
      toast('API 地址需以 http:// 或 https:// 开头', 'warn');
      return;
    }
    state.apiBase = value;
    try { localStorage.setItem(LS_API_BASE, value); } catch (e) { /* ignore */ }
    closePopovers();
    state.connected = false;
    state.everConnected = false;
    state.failCount = 0;
    setConnUI(false, null);
    toast(value ? ('已切换后端：' + value) : '已恢复同源后端', 'ok');
    refreshAll();
  }

  function resetApiBase() {
    state.apiBase = '';
    try { localStorage.removeItem(LS_API_BASE); } catch (e) { /* ignore */ }
    $('apiBase').value = '';
    applyApiBase();
  }

  /* ------------------------------------------------------------------------
   * 事件绑定
   * ---------------------------------------------------------------------- */

  function bindEvents() {
    $('btnReset').addEventListener('click', doReset);
    $('btnRun').addEventListener('click', doRun);
    $('btnPause').addEventListener('click', doPause);
    $('btnStep').addEventListener('click', function () { doStep(1); });
    $('btnStepN').addEventListener('click', doStepN);
    $('btnRefreshAll').addEventListener('click', refreshAll);
    $('bannerRetry').addEventListener('click', refreshAll);

    // 断点列表：点击删除
    $('bpList').addEventListener('click', function (e) {
      const li = e.target.closest ? e.target.closest('li[data-addr]') : null;
      if (!li) return;
      const addr = Number(li.getAttribute('data-addr'));
      if (isFinite(addr)) toggleBreakpoint(addr);
    });

    // 反汇编行：点击设置/清除断点
    $('disasmBody').addEventListener('click', function (e) {
      const tr = e.target.closest ? e.target.closest('tr[data-addr]') : null;
      if (!tr) return;
      const addr = Number(tr.getAttribute('data-addr'));
      if (isFinite(addr)) toggleBreakpoint(addr);
    });

    $('btnDisasmGo').addEventListener('click', function () {
      const addr = parseAddr($('disasmAddr').value);
      if (!isFinite(addr)) { toast('反汇编地址格式不正确', 'warn'); return; }
      // 手动跳转即进入“浏览模式”，关闭跟随 PC，避免下一次轮询把视图拉回去
      state.followPc = false;
      $('followPc').checked = false;
      loadDisasm(addr);
    });
    $('disasmAddr').addEventListener('keydown', function (e) {
      if (e.key === 'Enter') { e.preventDefault(); $('btnDisasmGo').click(); }
    });
    $('btnDisasmRefresh').addEventListener('click', function () {
      if (state.disasmBase !== null) loadDisasm(state.disasmBase);
      else loadDisasm(initialDisasmAddr());
    });
    $('followPc').addEventListener('change', function () {
      state.followPc = $('followPc').checked;
      if (state.followPc && state.lastStatus) loadDisasm(Math.max(CODE_START, state.lastStatus.pc - 0x10));
    });

    // 内存
    $('btnMemGo').addEventListener('click', function () {
      const addr = parseAddr($('memAddr').value);
      if (!isFinite(addr)) { toast('内存地址格式不正确', 'warn'); return; }
      loadMem(addr);
    });
    $('memAddr').addEventListener('keydown', function (e) {
      if (e.key === 'Enter') { e.preventDefault(); $('btnMemGo').click(); }
    });
    $('btnMemPrev').addEventListener('click', function () { loadMem(Math.max(0, state.memAddr - MEM_LEN)); });
    $('btnMemNext').addEventListener('click', function () { loadMem(state.memAddr + MEM_LEN); });
    $('btnMemPc').addEventListener('click', function () {
      if (state.lastStatus) loadMem(state.lastStatus.pc);
    });
    $('btnMemSp').addEventListener('click', function () {
      const sp = state.lastStatus && state.lastStatus.regs ? state.lastStatus.regs.r13 : RESET_PC;
      if (sp !== undefined && sp !== null) loadMem(sp & 0xFFFFFFF0);
    });
    $('btnMemRefresh').addEventListener('click', function () { loadMem(state.memAddr); });
    $('memBody').addEventListener('click', function (e) {
      const td = e.target.closest ? e.target.closest('td.c-hex[data-addr]') : null;
      if (td) beginMemEdit(td);
    });

    // 虚拟整机
    initSliders();
    $('btnRfSend').addEventListener('click', sendRf);
    $('rfHex').addEventListener('keydown', function (e) {
      if (e.key === 'Enter') { e.preventDefault(); sendRf(); }
    });

    // 调用栈跳转
    $('backtraceList').addEventListener('click', function (e) {
      const fr = e.target.closest ? e.target.closest('.bt-frame[data-pc]') : null;
      if (!fr) return;
      const pc = Number(fr.getAttribute('data-pc'));
      if (!isFinite(pc)) return;
      $('followPc').checked = false;
      state.followPc = false;
      loadDisasm(Math.max(CODE_START, pc - 0x10));
      toast('已跳转到 ' + hx(pc) + ' 的反汇编', 'ok');
    });
    $('btnBtRefresh').addEventListener('click', refreshBacktrace);

    // Trace
    $('traceAutoScroll').addEventListener('change', function () {
      state.traceAutoScroll = $('traceAutoScroll').checked;
    });
    $('btnTraceClear').addEventListener('click', function () {
      state.traceBuffer = [];
      clearTraceDom();
      toast('已清空 trace 视图（后端仍保留历史，新事件会继续显示）', 'ok');
    });

    // 控制台
    $('btnConsoleClear').addEventListener('click', function () {
      state.consoleSuppressed = state.consoleLastText;
      $('consoleText').textContent = '（已清空视图，等待新的固件日志…）';
    });

    // 弹窗
    $('btnHelp').addEventListener('click', toggleHelp);
    $('btnSettings').addEventListener('click', toggleSettings);
    $('btnApiApply').addEventListener('click', applyApiBase);
    $('btnApiReset').addEventListener('click', resetApiBase);
    $('apiBase').addEventListener('keydown', function (e) {
      if (e.key === 'Enter') { e.preventDefault(); applyApiBase(); }
    });
    document.addEventListener('click', function (e) {
      if (!e.target.closest || !e.target.closest('.top-actions')) closePopovers();
    });

    // 键盘快捷键
    document.addEventListener('keydown', function (e) {
      if (e.key === 'Escape') {
        closePopovers();
        if (state.memEditing && document.activeElement && document.activeElement.blur) document.activeElement.blur();
        return;
      }
      if (e.key === 'F5') { e.preventDefault(); doRun(); return; }
      if (e.key === 'F6') { e.preventDefault(); doPause(); return; }
      if (e.key === 'F7') { e.preventDefault(); doReset(); return; }
      if (e.key === 'F8') { e.preventDefault(); if (e.shiftKey) doStepN(); else doStep(1); return; }
    });

    // 页面重新可见时立刻刷新
    document.addEventListener('visibilitychange', function () {
      if (!document.hidden) { pollStatus(); pollTrace(); pollDevice(); }
    });
  }

  /* ------------------------------------------------------------------------
   * 初始化
   * ---------------------------------------------------------------------- */

  function init() {
    restoreFilters();
    initRegTable();
    initGpioTable();
    initButtonPad();
    initTraceFilters();
    clearTraceDom();
    bindEvents();
    $('followPc').checked = state.followPc;
    $('traceAutoScroll').checked = state.traceAutoScroll;
    $('apiBase').value = state.apiBase;
    updateControlState();
    updateSliderLabels();
    setConnUI(false, null);
    startPolling();
    // 首轮请求：状态 + 其它视图
    pollStatus();
    loadSymbols();
    loadDevice();
    refreshBacktrace();
    pollTrace();
    pollConsole();
    pollDevice();
  }

  if (document.readyState === 'loading') document.addEventListener('DOMContentLoaded', init);
  else init();
})();
