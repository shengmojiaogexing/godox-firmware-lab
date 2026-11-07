/*!
 * app.js — AD100 Pro 固件模拟器 · 网页调试界面主逻辑（v1.0）
 *
 * 设计要点：
 *   1. 纯函数（normalize / render 系列）不碰 DOM，可在 Node 中 require() 做冒烟自测；
 *      只有 boot() 之后的 DOM 事件/轮询部分依赖浏览器。
 *   2. 后端接口完全可选：?mock=1 时使用 mock.js；真实模式下轮询
 *      http://127.0.0.1:8765/api/state 等（字段命名变体做兼容，详见 API.md）。
 *   3. 任何网络错误都不清空界面：显示"连接断开"横幅并保留最后一次数据。
 */
(function (root) {
  'use strict';

  /* ============================== 常量 ============================== */
  var DEFAULT_API = 'http://127.0.0.1:8765';
  var DISASM_COUNT = 61;          // 反汇编窗口行数（PC ±30）
  var MEM_LEN = 256;              // 内存窗口字节数
  var POLL_RUNNING = 200;         // 运行时轮询间隔
  var POLL_PAUSED = 800;          // 暂停时轮询间隔
  var POLL_FAIL = 2000;           // 连接失败后的重试间隔
  var POLL_HIDDEN = 2500;         // 页面不可见时的间隔
  var SCREEN_POLL = 1500;         // LCD 屏幕轮询间隔
  var UART_MAX_CHARS = 20000;
  var LOG_MAX_ROWS = 150;
  var USART_TDR_ADDRS = [0x40013828, 0x40004428, 0x40004828, 0x40004C28, 0x40005028, 0x40013804, 0x40004404, 0x40004804];
  var BATTERY_MIN_MV = 2600;
  var BATTERY_MAX_MV = 4200;
  // 按键定义（后端 key 名以 emu/server.py 实际支持为准；见 API.md）
  var KEY_DEFS = [
    { key: 'power', label: '电源' },
    { key: 'mode', label: '模式' },
    { key: 'set', label: '设置' },
    { key: 'up', label: '上' },
    { key: 'down', label: '下' },
    { key: 'key2', label: '备用' }
  ];
  var MAIN_LOOP_PC = 0x0800D008;   // jump_main / boot_interactive 落点（1ms 主调度器）
  var MAIN_LOOP_LO = 0x0800C000;
  var MAIN_LOOP_HI = 0x0800E800;

  /* ============================== 工具（纯函数） ============================== */
  function hex2(n) {
    var s = (n & 0xFF).toString(16).toUpperCase();
    return s.length < 2 ? '0' + s : s;
  }
  function hex8(n) {
    var s = (n >>> 0).toString(16).toUpperCase();
    while (s.length < 8) s = '0' + s;
    return s;
  }
  function hex32(n) {
    return '0x' + hex8(n);
  }
  function hex0x(n) {
    return '0x' + hex8(n);
  }
  function escapeHtml(s) {
    return String(s == null ? '' : s).replace(/[&<>"']/g, function (c) {
      return { '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c];
    });
  }
  function firstDef() {
    for (var i = 0; i < arguments.length; i++) {
      if (arguments[i] !== undefined && arguments[i] !== null) return arguments[i];
    }
    return null;
  }
  // 解释地址/寄存器值：数字、0x 前缀、或"看起来像地址"的裸十六进制串
  function toAddr(v) {
    if (typeof v === 'number') return isFinite(v) ? (v >>> 0) : null;
    if (typeof v === 'boolean') return v ? 1 : 0;
    if (typeof v !== 'string') return null;
    var s = v.trim().replace(/[\s,]/g, '');
    if (!s) return null;
    if (/^0x/i.test(s)) return parseInt(s.slice(2), 16) >>> 0;
    if (!/^[0-9a-f]+$/i.test(s)) return null;
    if (/[a-f]/i.test(s)) return parseInt(s, 16) >>> 0;
    if (s.length >= 5) return parseInt(s, 16) >>> 0;   // 080030D4 这类无前缀地址
    return parseInt(s, 10) >>> 0;
  }
  // 解释计数/时间等十进制数：数字或十进制串（0x 前缀按十六进制）
  function toInt(v) {
    if (typeof v === 'number') return isFinite(v) ? Math.trunc(v) : null;
    if (typeof v === 'boolean') return v ? 1 : 0;
    if (typeof v !== 'string') return null;
    var s = v.trim().replace(/[\s,]/g, '');
    if (!s) return null;
    if (/^0x/i.test(s)) return parseInt(s.slice(2), 16);
    if (/^-?\d+$/.test(s)) return parseInt(s, 10);
    if (/^[0-9a-f]+$/i.test(s)) return parseInt(s, 16);
    return null;
  }
  function parseAddr(v) {
    return toAddr(v);
  }
  function clampInt(v, lo, hi) {
    return v < lo ? lo : (v > hi ? hi : v);
  }
  function align(v, n) {
    return (v >>> 0) & ~((n || 16) - 1);
  }
  function isFlashAddr(a) {
    return a != null && a >= 0x08000000 && a < 0x08040000;
  }
  function formatCount(n) {
    var s = String(n == null ? 0 : n);
    return s.replace(/\B(?=(\d{3})+(?!\d))/g, ',');
  }
  function formatIpc(v) {
    if (v == null || !isFinite(v) || v <= 0) return '—';
    if (v >= 1e6) return (v / 1e6).toFixed(2) + 'M/s';
    if (v >= 1e3) return (v / 1e3).toFixed(1) + 'k/s';
    return Math.round(v) + '/s';
  }
  function computeIpc(deltaInsns, deltaMs) {
    if (deltaMs == null || deltaMs <= 0 || deltaInsns == null) return null;
    return deltaInsns / (deltaMs / 1000);
  }
  function pickPollInterval(running, failed, hidden) {
    if (hidden) return POLL_HIDDEN;
    if (failed) return POLL_FAIL;
    return running ? POLL_RUNNING : POLL_PAUSED;
  }

  /* ============================== 虚拟输入（纯函数） ============================== */
  function batteryMvToPct(mv) {
    var v = toInt(mv);
    if (v == null) return null;
    v = clampInt(v, BATTERY_MIN_MV, BATTERY_MAX_MV);
    return Math.round((v - BATTERY_MIN_MV) * 100 / (BATTERY_MAX_MV - BATTERY_MIN_MV));
  }
  function formatBattery(mv) {
    var v = toInt(mv);
    if (v == null) return '—';
    return (v / 1000).toFixed(2) + ' V · ' + batteryMvToPct(v) + '%';
  }
  function batteryCmd(mv) {
    return { cmd: 'battery', mv: clampInt(toInt(mv) == null ? 3700 : toInt(mv), BATTERY_MIN_MV, BATTERY_MAX_MV) };
  }
  function keyDownCmd(key) { return { cmd: 'key', key: String(key), down: true }; }
  function keyUpCmd(key) { return { cmd: 'key_up', key: String(key) }; }
  function keyPressCmd(key, ms) { return { cmd: 'key_press', key: String(key), ms: ms == null ? 80 : ms }; }
  function screenSaveCmd() { return { cmd: 'screen_save' }; }
  function clearInputsCmd() { return { cmd: 'clear_inputs' }; }
  function bootInteractiveCmd(warmup) {
    return { cmd: 'boot_interactive', warmup: warmup == null ? 1000000 : warmup };
  }
  function isMainLoopPc(pc) {
    var a = toAddr(pc);
    return a != null && a >= MAIN_LOOP_LO && a < MAIN_LOOP_HI;
  }
  function mapTaskPhase(phase) {
    var p = String(phase || '').toLowerCase();
    if (p === 'scheduler') return '主循环 1ms';
    if (p === 'main_loop') return '主循环';
    if (p === 'boot') return '启动中';
    if (p === 'fault') return '异常';
    return phase || '';
  }
  // 状态栏文案：已进入主循环 · 主循环 1ms · 0x0800D008 · tick=45 · LCD 非零 290B
  function formatPhaseInfo(model) {
    if (!model) return '—';
    var parts = [];
    if (model.inMainLoop === true) parts.push('已进入主循环');
    else if (model.inMainLoop === false) parts.push('未进入主循环');
    var phase = mapTaskPhase(model.taskPhase);
    if (phase) parts.push(phase);
    if (model.pcName) parts.push(String(model.pcName));
    if (model.virtualTicks != null) parts.push('tick=' + formatCount(model.virtualTicks));
    if (model.lcd && model.lcd.nonzero != null) parts.push('LCD 非零 ' + model.lcd.nonzero + 'B');
    return parts.join(' · ') || '—';
  }

  /* ============================== 2.4G 射频（虚拟 CC2500，纯函数） ============================== */
  function rfInitCmd() { return { cmd: 'rf_init' }; }
  function rfStatusCmd() { return { cmd: 'rf_status' }; }
  function rfRxCmd(hex) {
    var norm = normalizeRfHex(hex);
    return { cmd: 'rf_rx', hex: norm == null ? String(hex == null ? '' : hex) : norm };
  }
  function normalizeRfHex(input) {
    var s = String(input == null ? '' : input)
      .replace(/0x/gi, '').replace(/[\s,:-]/g, '').toUpperCase();
    if (!s || s.length % 2 !== 0 || !/^[0-9A-F]+$/.test(s)) return null;
    var out = [];
    for (var i = 0; i < s.length; i += 2) out.push(s.substr(i, 2));
    return out.join(' ');
  }
  function normalizeRf(raw) {
    if (!raw || typeof raw !== 'object') return null;
    var d = (raw.rf && typeof raw.rf === 'object') ? raw.rf : raw;
    var state = firstDef(d.state, d.marcState, d.mode);
    if (typeof state === 'number') state = ({ 0: 'IDLE', 1: 'RX', 2: 'TX' })[state] || ('0x' + hex2(state));
    var rf = {
      present: d.present !== false,
      state: state == null ? null : String(state).toUpperCase(),
      gdo0: toInt(firstDef(d.gdo0, d.gdo0_level, d.gdo0Level)),
      rxCount: toInt(firstDef(d.rx_count, d.rxCount, d.rx)),
      txCount: toInt(firstDef(d.tx_count, d.txCount, d.tx)),
      lastRx: firstDef(d.last_rx, d.lastRx) == null ? null : String(firstDef(d.last_rx, d.lastRx)),
      lastTx: firstDef(d.last_tx, d.lastTx) == null ? null : String(firstDef(d.last_tx, d.lastTx)),
      rxFifo: firstDef(d.rx_fifo, d.rxFifo) == null ? null : String(firstDef(d.rx_fifo, d.rxFifo)),
      txFifo: firstDef(d.tx_fifo, d.txFifo) == null ? null : String(firstDef(d.tx_fifo, d.txFifo)),
      freq2: toAddr(firstDef(d.freq2, d.freq_2, d.freq2Reg)),
      pktlen: toInt(firstDef(d.pktlen, d.packetLength, d.pkt_len)),
      patable: firstDef(d.patable, d.paTable) || null,
      recent: Array.isArray(d.recent) ? d.recent : []
    };
    if (rf.rxCount == null) rf.rxCount = 0;
    if (rf.txCount == null) rf.txCount = 0;
    return rf;
  }
  function formatRfStatus(model) {
    var rf = model && model.rf;
    if (!rf) return '未连接射频模型（/api/state.rf 缺失）';
    if (rf.present === false) return '虚拟 CC2500 不可用（rf.present=false）';
    var parts = ['state=' + (rf.state || '?')];
    if (rf.freq2 != null) parts.push('freq2=0x' + hex2(rf.freq2));
    if (rf.pktlen != null) parts.push('pktlen=' + rf.pktlen);
    parts.push('RX ' + formatCount(rf.rxCount) + ' / TX ' + formatCount(rf.txCount));
    if (rf.gdo0 != null) parts.push('GDO0=' + rf.gdo0);
    if (rf.lastRx) parts.push('last=' + rf.lastRx);
    return parts.join(' · ');
  }
  // Cortex-M0 异常号 → 可读名（emu: 15=SysTick, 35=TIM14, 21=EXTI0_1, 23=EXTI4_15）
  function irqLabel(key) {
    var n = toInt(key);
    if (n == null) return String(key);
    if (n === 15) return 'SysTick';
    if (n === 35) return 'TIM14';
    if (n === 21) return 'EXTI0_1';
    if (n === 23) return 'EXTI4_15/RF';
    if (n === 2) return 'NMI';
    if (n === 3) return 'HardFault';
    if (n === 11) return 'SVC';
    if (n === 14) return 'PendSV';
    if (n >= 16) return 'IRQ' + (n - 16);
    return 'exc' + n;
  }
  function normalizeIrqCounts(raw) {
    if (raw == null) return { items: [], total: 0 };
    var obj = raw;
    if (Array.isArray(raw)) {
      obj = {};
      for (var i = 0; i < raw.length; i++) {
        var e = raw[i];
        if (e && e.key != null) obj[e.key] = e.count != null ? e.count : e.value;
        else if (e && e.irq != null) obj[e.irq] = e.count != null ? e.count : e.value;
      }
    }
    var items = [];
    var total = 0;
    if (obj && typeof obj === 'object') {
      Object.keys(obj).forEach(function (k) {
        var c = toInt(obj[k]);
        if (c == null) return;
        items.push({ key: k, label: irqLabel(k), count: c });
        total += c;
      });
    }
    items.sort(function (a, b) { return (toInt(a.key) || 0) - (toInt(b.key) || 0); });
    return { items: items, total: total };
  }
  function normalizeIrqLog(raw) {
    if (!Array.isArray(raw)) return [];
    var out = [];
    for (var i = raw.length - 1; i >= 0 && out.length < 5; i--) {
      var e = raw[i];
      if (e == null) continue;
      if (typeof e === 'string') { out.push({ text: String(e) }); continue; }
      var irq = toInt(firstDef(e.irq, e.exception, e.exc));
      out.push({
        insn: toInt(firstDef(e.insn, e.count, e.insnCount)),
        irq: irq,
        label: irq == null ? 'IRQ' : irqLabel(irq),
        handler: toAddr(firstDef(e.handler, e.pc, e.vector)),
        source: firstDef(e.source, e.reason, e.name) || null
      });
    }
    return out;
  }
  function formatIrqLine(model) {
    var irq = model && model.irqCounts;
    if (!irq || !irq.items.length) return null;
    var shown = irq.items.filter(function (it) {
      return it.label === 'SysTick' || it.label === 'TIM14' ||
        it.label === 'EXTI0_1' || it.label === 'EXTI4_15/RF';
    });
    if (!shown.length) shown = irq.items;
    return shown.map(function (it) { return it.label + '=' + formatCount(it.count); }).join(' · ');
  }
  function formatIrqLogEntry(e) {
    if (!e) return '';
    if (e.text) return e.text;
    var s = e.label;
    if (e.handler != null) s += ' @' + hex32(e.handler);
    if (e.source) s += ' (' + e.source + ')';
    return s;
  }
  // 供 selftest 使用的 RF 面板渲染（DOM 端用 applyRfPanel 写同一内容）
  function renderRfPanel(model) {
    var rf = model && model.rf;
    var irq = model && model.irqCounts;
    var irqLog = model && model.irqLog;
    if (!rf) {
      return '<div class="rf-line">模拟器未提供 <code>rf</code> 字段（兼容缺失）</div>' +
        '<div class="rf-line rf-dim">若后端无 CC2500 模型，射频面板仅显示 IRQ 计数</div>' +
        (irq && irq.items.length ? '<div class="rf-line"><b>IRQ</b>' + escapeHtml(formatIrqLine(model)) + '</div>' : '');
    }
    var html = '';
    var stateCls = rf.state === 'RX' ? ' rx' : (rf.state === 'TX' ? ' tx' : '');
    html += '<div class="rf-line"><b>状态</b><span class="rf-state' + stateCls + '">' + escapeHtml('state=' + (rf.state || '?')) + '</span>' +
      (rf.freq2 != null ? '<span>freq2=0x' + hex2(rf.freq2) + '</span>' : '') +
      (rf.pktlen != null ? '<span>pktlen=' + rf.pktlen + '</span>' : '') +
      (rf.gdo0 != null ? '<span>GDO0=' + rf.gdo0 + '</span>' : '') + '</div>';
    html += '<div class="rf-line"><b>计数</b><span>RX ' + formatCount(rf.rxCount) + ' / TX ' + formatCount(rf.txCount) + '</span></div>';
    if (rf.lastRx) html += '<div class="rf-line"><b>last_rx</b><span class="rf-last">' + escapeHtml(rf.lastRx) + '</span></div>';
    if (rf.lastTx) html += '<div class="rf-line"><b>last_tx</b><span class="rf-last">' + escapeHtml(rf.lastTx) + '</span></div>';
    var irqLine = formatIrqLine(model);
    if (irqLine) html += '<div class="rf-line rf-irqline"><b>IRQ</b><span>' + escapeHtml(irqLine) + '</span></div>';
    if (irqLog && irqLog.length) {
      html += '<div class="rf-line rf-logline"><b>irqLog</b><span class="rf-dim">' +
        escapeHtml(irqLog.slice(0, 2).map(formatIrqLogEntry).join(' | ')) + '</span></div>';
    }
    return html;
  }

  // 兼容 /api/state.inputs 的多种写法，缺失时返回 null（UI 显示"未提供"）
  function normalizeInputs(raw) {    if (raw == null) return null;
    if (typeof raw === 'string') return { available: true, raw: raw, keys: {}, pressed: [] };
    if (Array.isArray(raw)) {
      var pressedArr = raw.map(function (k) { return typeof k === 'string' ? k : (k && (k.key || k.id)); }).filter(Boolean);
      return { available: true, raw: raw, keys: {}, pressed: pressedArr, batteryMv: null };
    }
    var o = raw;
    var mv = toInt(firstDef(o.battery_mv, o.batteryMv, o.battery, o.vbat_mv, o.voltage_mv, o.mv));
    var pct = toInt(firstDef(o.battery_pct, o.batteryPct, o.pct, o.percent));
    if (pct == null && mv != null) pct = batteryMvToPct(mv);
    var keys = {};
    var rawKeys = firstDef(o.keys, o.keyState, o.buttons, o.key);
    if (Array.isArray(rawKeys)) {
      for (var i = 0; i < rawKeys.length; i++) {
        var k = rawKeys[i];
        if (typeof k === 'string') keys[k] = true;
        else if (k && (k.key || k.id)) keys[k.key || k.id] = !!(k.down != null ? k.down : k.pressed);
      }
    } else if (rawKeys && typeof rawKeys === 'object') {
      for (var name in rawKeys) {
        if (!Object.prototype.hasOwnProperty.call(rawKeys, name)) continue;
        var kv = rawKeys[name];
        if (typeof kv === 'boolean') {
          keys[name] = kv;
        } else if (kv && typeof kv === 'object') {
          // emu 的 keys 里可能嵌套 matrix/all 子对象，一并展开
          for (var sub in kv) {
            if (typeof kv[sub] === 'boolean') keys[sub] = kv[sub];
          }
        }
      }
    }
    var pressed = [];
    var pr = firstDef(o.pressed, o.pressed_keys, o.held, o.pressedKeys);
    if (Array.isArray(pr)) {
      for (var j = 0; j < pr.length; j++) pressed.push(String(typeof pr[j] === 'object' ? (pr[j].key || pr[j].id) : pr[j]));
    } else {
      pressed = Object.keys(keys).filter(function (n) { return keys[n]; });
    }
    var last = firstDef(o.last, o.last_event, o.lastKey, o.event, o.last_key);
    if (last != null && typeof last !== 'object') last = { type: 'event', key: String(last) };
    var adc = firstDef(o.adc, o.adc_value, o.adc_mv, o.adcRaw);
    if (adc && typeof adc === 'object' && !Array.isArray(adc) && Object.keys(adc).length === 0) adc = null;
    var save = firstDef(o.screen_save, o.last_save, o.saved, o.lastSave);
    return {
      available: true,
      batteryMv: mv,
      batteryPct: pct,
      keys: keys,
      pressed: pressed,
      last: last || null,
      adc: adc == null ? null : adc,
      screenSave: save == null ? null : String(save),
      raw: raw
    };
  }

  function renderInputStatus(model, localLast) {
    var inputs = model && model.inputs;
    var html = '';
    if (!inputs) {
      html += '<div class="io-status-line">模拟器未提供 <code>inputs</code> 字段（兼容缺失）</div>' +
        '<div class="io-status-line">命令仍会发送，可看后端日志确认</div>';
    } else {
      html += '<div class="io-status-line"><b>电量</b>' +
        (inputs.batteryMv != null ? escapeHtml(formatBattery(inputs.batteryMv)) : '—') +
        (inputs.adc != null ? ' <span class="on">ADC ' + escapeHtml(typeof inputs.adc === 'object' ? JSON.stringify(inputs.adc) : String(inputs.adc)) + '</span>' : '') +
        '</div>';
      var pressed = inputs.pressed || [];
      html += '<div class="io-status-line"><b>按下</b>' +
        (pressed.length ? '<span class="on">' + escapeHtml(pressed.join(' · ')) + '</span>' : '无') + '</div>';
      if (inputs.last) {
        var l = inputs.last;
        var txt = [l.type || l.cmd || 'event', l.key, l.down != null ? (l.down ? 'down' : 'up') : '', l.ms != null ? l.ms + 'ms' : '']
          .filter(function (x) { return x !== '' && x != null; }).join(' ');
        if (txt) html += '<div class="io-status-line"><b>最近事件</b><span class="warn">' + escapeHtml(txt) + '</span></div>';
      }
      if (inputs.screenSave) {
        html += '<div class="io-status-line"><b>最近保存</b>' + escapeHtml(inputs.screenSave) + '</div>';
      }
    }
    if (localLast) {
      html += '<div class="io-status-line"><b>本地发送</b><span class="on">' + escapeHtml(localLast) + '</span></div>';
    }
    return html;
  }
  function normalizeBreakpoints(raw) {
    var list = raw;
    if (raw && !Array.isArray(raw)) list = raw.breakpoints || raw.bps || raw.bp || raw.breakpoints_list;
    if (!Array.isArray(list)) return [];
    var out = [];
    for (var i = 0; i < list.length; i++) {
      var a = toAddr(list[i]);
      if (a != null && out.indexOf(a) < 0) out.push(a);
    }
    return out.sort(function (x, y) { return x - y; });
  }

  /* ============================== 状态兼容层（纯函数） ============================== */
  function normalizeState(rawIn) {
    var raw = rawIn || {};
    // 兼容 {state:{...}} / {data:{...}} 包装
    if (raw.pc == null && raw.state && typeof raw.state === 'object') raw = raw.state;
    if (raw.pc == null && raw.data && typeof raw.data === 'object' && !Array.isArray(raw.data)) raw = raw.data;

    var regsRaw = firstDef(raw.regs, raw.registers, raw.gpr);
    var arr = Array.isArray(regsRaw) ? regsRaw : null;
    var obj = (regsRaw && typeof regsRaw === 'object' && !Array.isArray(regsRaw)) ? regsRaw : null;
    var vals = {};
    for (var i = 0; i < 13; i++) {
      var k = 'r' + i;
      var v = null;
      if (obj && obj[k] != null) v = obj[k];
      else if (raw[k] != null) v = raw[k];
      else if (arr && arr[i] != null) v = arr[i];
      vals[k] = toAddr(v);
    }
    var pc = toAddr(firstDef(raw.pc, raw.PC, raw.reg_pc, raw.pcValue, arr && arr[15], obj && (obj.pc || obj.r15)));
    var sp = toAddr(firstDef(raw.sp, raw.SP, raw.reg_sp, arr && arr[13], obj && (obj.sp || obj.r13)));
    var lr = toAddr(firstDef(raw.lr, raw.LR, raw.reg_lr, arr && arr[14], obj && (obj.lr || obj.r14)));
    if (pc == null && vals.r15 != null) pc = vals.r15;
    if (sp == null && vals.r13 != null) sp = vals.r13;
    if (lr == null && vals.r14 != null) lr = vals.r14;
    pc = pc == null ? 0 : pc;
    sp = sp == null ? 0 : sp;
    lr = lr == null ? 0 : lr;

    var xpsr = toAddr(firstDef(raw.xpsr, raw.XPSR, raw.psr, raw.cpsr, raw.xPSR));
    var flags = { N: false, Z: false, C: false, V: false };
    var fRaw = firstDef(raw.flags, raw.flag, raw.condition);
    if (fRaw && typeof fRaw === 'object') {
      flags.N = !!firstDef(fRaw.N, fRaw.n);
      flags.Z = !!firstDef(fRaw.Z, fRaw.z);
      flags.C = !!firstDef(fRaw.C, fRaw.c);
      flags.V = !!firstDef(fRaw.V, fRaw.v);
    } else if (typeof fRaw === 'string') {
      flags.N = /n/i.test(fRaw); flags.Z = /z/i.test(fRaw);
      flags.C = /c/i.test(fRaw); flags.V = /v/i.test(fRaw);
    } else if (xpsr != null) {
      flags.N = !!(xpsr & 0x80000000);
      flags.Z = !!(xpsr & 0x40000000);
      flags.C = !!(xpsr & 0x20000000);
      flags.V = !!(xpsr & 0x10000000);
    }
    if (xpsr == null) {
      xpsr = (flags.N ? 0x80000000 : 0) | (flags.Z ? 0x40000000 : 0) |
             (flags.C ? 0x20000000 : 0) | (flags.V ? 0x10000000 : 0) | 0x01000000;
    }

    var regs = [];
    for (i = 0; i < 16; i++) {
      regs.push({ name: ['r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'r6', 'r7', 'r8', 'r9', 'r10', 'r11', 'r12', 'sp', 'lr', 'pc'][i], value: vals['r' + i] });
    }
    regs[13].value = sp;
    regs[14].value = lr;
    regs[15].value = pc;

    var running = raw.running != null ? !!raw.running
      : (raw.isRunning != null ? !!raw.isRunning
      : (raw.is_running != null ? !!raw.is_running
      : String(firstDef(raw.state, raw.status, '')).toLowerCase() === 'running'));

    var insnCountRaw = firstDef(raw.insnCount, raw.instructionCount, raw.insn_count, raw.instructions, raw.count, raw.steps);
    var insnCount = toInt(insnCountRaw);
    insnCount = insnCount == null ? 0 : insnCount;

    var logRaw = firstDef(raw.log, raw.mmio, raw.trace, raw.periphLog, raw.peripheral_log, raw.events);
    var log = normalizePeripheralLog(logRaw);
    var bpsField = firstDef(raw.breakpoints, raw.bps, raw.bp);
    var hasBpsField = bpsField !== null && bpsField !== undefined;

    var callstackRaw = firstDef(raw.callstack, raw.callStack, raw.stack, raw.frames);
    var callstack = normalizeCallstack(callstackRaw);

    var funcRaw = firstDef(raw.func, raw.currentFunc, raw.function, raw.funcInfo);
    var func = normalizeFuncInfo(funcRaw);
    var nearestRaw = firstDef(raw.nearestFunc, raw.nextFunc, raw.next_function);
    var nearestFunc = normalizeFuncInfo(nearestRaw);

    var uart = normalizeUart(raw, log);
    var stats = (raw.stats && typeof raw.stats === 'object') ? raw.stats : null;
    var ipc = firstDef(raw.ipc, raw.ips, raw.insnsPerSec, raw.ipcRate,
      stats && (stats.ipc != null ? stats.ipc : (stats.ips != null ? stats.ips : stats.ipc_rate)));
    ipc = ipc == null ? null : Number(ipc);
    var lcd = normalizeLcd(firstDef(raw.lcd, raw.lcdInfo, raw.display));
    var faults = normalizeFaults(firstDef(raw.faults, raw.fault, raw.exceptions));
    var rf = normalizeRf(firstDef(raw.rf, raw.radio, raw.cc2500));
    var irqCounts = normalizeIrqCounts(firstDef(raw.irqCounts, raw.irq_counts, raw.irq));
    var irqLog = normalizeIrqLog(firstDef(raw.irqLog, raw.irq_log, raw.irqLogs));
    var inIsr = raw.inIsr != null ? !!raw.inIsr : (raw.in_isr != null ? !!raw.in_isr : null);
    var realIrq = raw.realIrq != null ? !!raw.realIrq : (raw.real_irq != null ? !!raw.real_irq : null);
    var tim14Count = toInt(firstDef(raw.tim14Count, raw.tim14_count, raw.tim14));

    return {
      pc: pc, sp: sp, lr: lr, xpsr: xpsr, flags: flags, regs: regs,
      insnCount: insnCount, running: running,
      breakpoints: normalizeBreakpoints(bpsField), hasBpsField: hasBpsField,
      log: log, uart: uart, callstack: callstack,
      func: func, nearestFunc: nearestFunc,
      ipc: isFinite(ipc) ? ipc : null,
      lcd: lcd, faults: faults, stats: stats,
      inputs: normalizeInputs(firstDef(raw.inputs, raw.input, raw.inject)),
      lastScreenSave: firstDef(raw.lastScreenSave, raw.last_screen_save, raw.screen_save) == null
        ? null : String(firstDef(raw.lastScreenSave, raw.last_screen_save, raw.screen_save)),
      pcName: firstDef(raw.pcName, raw.pc_name, raw.function) == null ? null : String(firstDef(raw.pcName, raw.pc_name, raw.function)),
      taskPhase: firstDef(raw.taskPhase, raw.task_phase, raw.phase) == null ? null : String(firstDef(raw.taskPhase, raw.task_phase, raw.phase)),
      inMainLoop: raw.inMainLoop != null ? !!raw.inMainLoop : (raw.in_main_loop != null ? !!raw.in_main_loop : null),
      virtualTicks: toInt(firstDef(raw.virtualTicks, raw.virtual_ticks, raw.ticks)),
      rf: rf,
      irqCounts: irqCounts,
      irqLog: irqLog,
      inIsr: inIsr,
      realIrq: realIrq,
      tim14Count: tim14Count,
      raw: raw
    };
  }

  function normalizeLcd(raw) {
    if (!raw || typeof raw !== 'object') return null;
    var w = toInt(firstDef(raw.w, raw.width));
    var h = toInt(firstDef(raw.h, raw.height));
    var lcd = {
      w: w, h: h,
      format: String(firstDef(raw.format, raw.pixelFormat, raw.mode, '1bpp')),
      addr: toAddr(firstDef(raw.addr, raw.address, raw.framebuffer, raw.fb)),
      frames: toInt(firstDef(raw.frames, raw.frame, raw.frameCount)),
      bytesWritten: toInt(firstDef(raw.bytes_written, raw.bytesWritten)),
      nonzero: toInt(firstDef(raw.nonzero, raw.non_zero, raw.pixels)),
      page: toInt(firstDef(raw.page, raw.pageAddr, raw.page_addr)),
      col: toInt(firstDef(raw.col, raw.column, raw.colAddr)),
      startLine: toInt(firstDef(raw.start_line, raw.startLine))
    };
    if (!lcd.w && !lcd.h && lcd.addr == null) return null;
    return lcd;
  }

  function normalizeFaults(v) {
    if (v == null) return [];
    if (Array.isArray(v)) {
      var out = [];
      for (var i = 0; i < v.length; i++) {
        var e = v[i];
        if (e == null) continue;
        if (typeof e === 'string') { out.push({ text: e }); continue; }
        out.push({
          addr: toAddr(firstDef(e.addr, e.faultAddr, e.address)),
          pc: toAddr(firstDef(e.pc, e.PC)),
          lr: toAddr(firstDef(e.lr, e.LR)),
          type: firstDef(e.type, e.kind, e.reason, e.note) || 'FAULT',
          text: firstDef(e.text, e.message, '') || ''
        });
      }
      return out;
    }
    if (typeof v === 'object') return normalizeFaults([v]);
    if (typeof v === 'number') {
      var arr = [];
      for (var k = 0; k < v; k++) arr.push({ type: 'FAULT', text: 'fault #' + (k + 1) });
      return arr;
    }
    return [{ text: String(v) }];
  }

  function normalizeFuncInfo(v) {
    if (!v) return null;
    if (typeof v === 'string') return { addr: null, name: v };
    if (typeof v === 'object') {
      var addr = toAddr(firstDef(v.addr, v.address, v.start, v.pc));
      var name = firstDef(v.name, v.label, v.symbol, v.func, v.function);
      if (addr == null && name == null) return null;
      return { addr: addr, name: name == null ? null : String(name) };
    }
    return null;
  }

  // 解析 emu/server.py 的字符串日志行：
  // "[  3145930] 0800A4BC  GPIOB       BSRR         W 0x00000040  sz=4  func_08007426@08007426"
  function parseLogLine(s, i) {
    var t = String(s).trim();
    var m = /^\[\s*([0-9.]+)\s*\]\s+([0-9A-Fa-f]{8})\s+(\S+)\s+(\S+)\s+([WR])\s+(0x[0-9A-Fa-f]+)\s*(.*)$/.exec(t);
    if (m) {
      return {
        index: i,
        time: toInt(m[1]),
        pc: toAddr(m[2]),
        dir: m[5],
        reg: m[3] + '.' + m[4],
        periph: m[3],
        addr: null,
        value: toAddr(m[6]),
        note: (m[7] || '').replace(/^\s*sz=\d+\s*/, '').trim()
      };
    }
    // 有些模拟器用 "W reg value @pc" 的简单文本
    var m2 = /^([WR])\s+(\S+)\s+(0x[0-9A-Fa-f]+|\d+)(?:\s+@?([0-9A-Fa-fx]+))?\s*(.*)$/.exec(t);
    if (m2) {
      return {
        index: i, dir: m2[1], reg: m2[2], value: toAddr(m2[3]),
        pc: m2[4] ? toAddr(m2[4]) : null, note: (m2[5] || '').trim()
      };
    }
    return { index: i, text: t, _raw: true };
  }

  function normalizePeripheralLog(rawLog) {
    if (!Array.isArray(rawLog)) return [];
    var out = [];
    for (var i = 0; i < rawLog.length; i++) {
      var e = rawLog[i];
      if (e == null) continue;
      if (typeof e === 'string') { out.push(parseLogLine(e, i)); continue; }
      var addr = toAddr(firstDef(e.addr, e.address, e.offset, e.regAddr));
      var pc = toAddr(firstDef(e.pc, e.pcAddr, e.programCounter, e.insnAddr));
      var value = firstDef(e.value, e.val, e.data, e.written);
      var dir = firstDef(e.dir, e.rw, e.direction, e.type);
      if (dir == null) dir = e.write ? 'W' : (e.read ? 'R' : '?');
      out.push({
        index: i,
        dir: String(dir).toUpperCase().slice(0, 1) || '?',
        addr: addr,
        reg: firstDef(e.reg, e.register, e.name, e.symbol) || null,
        value: value == null ? null : value,
        pc: pc,
        note: String(firstDef(e.note, e.comment, e.text, '')),
        char: e.char != null ? String(e.char) : null,
        time: e.time != null ? e.time : null
      });
    }
    return out;
  }

  function normalizeCallstack(raw) {
    if (!Array.isArray(raw)) return [];
    var out = [];
    for (var i = 0; i < raw.length; i++) {
      var e = raw[i];
      if (e == null) continue;
      if (typeof e === 'number' || typeof e === 'string') {
        var a = toAddr(e);
        if (a != null) out.push({ addr: a, name: null, current: i === 0 });
        continue;
      }
      out.push({
        addr: toAddr(firstDef(e.addr, e.address, e.pc, e.returnAddr)),
        name: firstDef(e.name, e.func, e.label, e.symbol) || null,
        current: e.current === true || i === 0
      });
    }
    return out;
  }

  function normalizeUart(raw, log) {
    var u = firstDef(raw.uart, raw.uartText, raw.console, raw.serial, raw.stdout);
    if (u != null) {
      if (Array.isArray(u)) {
        var lines = [];
        for (var i = 0; i < u.length; i++) {
          var x = u[i];
          if (typeof x === 'string') lines.push(x);
          else if (x && typeof x === 'object') lines.push(String(firstDef(x.text, x.line, x.data, '')));
        }
        return lines.join('\n');
      }
      if (typeof u === 'object' && u.text != null) return String(u.text);
      return String(u);
    }
    // 从 MMIO 日志里的串口 TX 写操作推导（部分模拟器只记录日志）
    var txt = '';
    for (var j = 0; j < log.length; j++) {
      var e = log[j];
      if (!e) continue;
      if (e.char != null) { txt += e.char; continue; }
      if (e.dir !== 'W') continue;
      var isUart = (e.addr != null && USART_TDR_ADDRS.indexOf(e.addr) >= 0) ||
        (e.reg && /USART/i.test(e.reg) && /(TDR|DR|TX)/i.test(e.reg));
      if (!isUart) continue;
      var v = toInt(e.value);
      if (v == null) continue;
      v = v & 0xFF;
      if (v === 0x0A || v === 0x0D || (v >= 0x20 && v < 0x7F)) txt += String.fromCharCode(v);
    }
    return txt;
  }

  function normalizeDisasm(rawIn, fallbackAddr) {
    var raw = rawIn || {};
    if (raw.lines == null && raw.data && typeof raw.data === 'object') raw = raw.data;
    var linesRaw = firstDef(raw.lines, raw.disasm, raw.instructions);
    if (!Array.isArray(linesRaw) && Array.isArray(raw)) linesRaw = raw;
    var out = [];
    if (Array.isArray(linesRaw)) {
      for (var i = 0; i < linesRaw.length; i++) {
        var l = linesRaw[i];
        if (l == null) continue;
        if (typeof l === 'string') {
          var parsed = parseDisasmText(l);
          if (parsed) out.push(parsed);
          continue;
        }
        var addr = toAddr(firstDef(l.addr, l.address, l.pc, l.offset));
        if (addr == null) continue;
        var bytes = firstDef(l.bytes, l.encoding, l.hex, l.raw);
        if (Array.isArray(bytes)) {
          var bs = '';
          for (var b = 0; b < bytes.length; b++) bs += hex2(toInt(bytes[b]) || 0);
          bytes = bs;
        }
        bytes = bytes == null ? '' : String(bytes).replace(/[\s]/g, '').toUpperCase();
        var mnem = firstDef(l.mnem, l.mnemonic, l.opcode, l.insn, '');
        var op = firstDef(l.op, l.operands, l.args, l.op_str, '');
        out.push({ addr: addr, bytes: bytes, mnem: String(mnem), op: String(op) });
      }
    }
    out.sort(function (a, b) { return a.addr - b.addr; });
    var dedup = [];
    var last = -1;
    for (var k = 0; k < out.length; k++) {
      if (out[k].addr !== last) { dedup.push(out[k]); last = out[k].addr; }
    }
    return { addr: toAddr(firstDef(raw.addr, raw.address, fallbackAddr)) || 0, lines: dedup };
  }

  function parseDisasmText(text) {
    var m = /^\s*([0-9A-Fa-f]{8})\s+(?:([0-9A-Fa-f]{4,16})\s+)?(\S+)\s*(.*)$/.exec(String(text));
    if (!m) return null;
    var addr = parseInt(m[1], 16) >>> 0;
    var bytes = m[2] ? m[2].toUpperCase() : '';
    var mnem = m[3] || '';
    var op = (m[4] || '').replace(/\s*;.*$/, '').trim();
    if (!bytes) {
      var rest = (m[3] || '') + ' ' + (m[4] || '');
      var m2 = /^([0-9A-Fa-f]{4,16})\s+(\S+)\s*(.*)$/.exec(rest.trim());
      if (m2) { bytes = m2[1].toUpperCase(); mnem = m2[2]; op = (m2[3] || '').trim(); }
    }
    return { addr: addr, bytes: bytes, mnem: mnem, op: op };
  }

  function normalizeMem(rawIn, fallbackAddr, wantLen) {
    var raw = rawIn || {};
    if (raw.bytes == null && raw.hex == null && raw.data == null && raw.mem != null) raw = raw.mem;
    var addr = toAddr(firstDef(raw.addr, raw.address, raw.base, fallbackAddr)) || 0;
    var hexData = firstDef(raw.hex, raw.bytes, raw.data, raw.mem);
    if (hexData && typeof hexData === 'object' && !Array.isArray(hexData)) {
      hexData = firstDef(hexData.hex, hexData.data, hexData.bytes);
    }
    var bytes = [];
    if (Array.isArray(hexData)) {
      for (var i = 0; i < hexData.length; i++) {
        var b = toInt(hexData[i]);
        if (b == null) b = toAddr(hexData[i]);
        bytes.push((b == null ? 0 : b) & 0xFF);
      }
    } else if (typeof hexData === 'string') {
      var s = hexData.replace(/0x/gi, '').replace(/[^0-9A-Fa-f]/g, '');
      for (var j = 0; j + 1 < s.length; j += 2) bytes.push(parseInt(s.substr(j, 2), 16));
    }
    var len = wantLen || MEM_LEN;
    while (bytes.length < len) bytes.push(0);
    return { addr: addr, bytes: bytes.slice(0, Math.max(len, bytes.length)), len: len };
  }

  function normalizeScreen(rawIn) {
    var raw = rawIn || {};
    if (raw.screen && typeof raw.screen === 'object') raw = raw.screen;
    var w = toInt(firstDef(raw.w, raw.width, raw.cols));
    var h = toInt(firstDef(raw.h, raw.height, raw.rows));
    if (!w || !h || w <= 0 || h <= 0 || w > 4096 || h > 4096) return null;
    var format = String(firstDef(raw.format, raw.pixelFormat, raw.mode, 'gray8')).toLowerCase();
    var addr = toAddr(firstDef(raw.addr, raw.address, raw.framebuffer, raw.fb));
    var info = (raw.info && typeof raw.info === 'object') ? raw.info : null;
    // 面板格式：SSD1306 风格 page/column（每字节 = 纵向 8 像素）。
    // emu/server.py 的 /api/screen 是 {w,h,format:"1bpp",info:{page,col,...}} 这种。
    var layout = 'linear';
    if (info && (info.page != null || info.col != null || info.start_line != null)) layout = 'page';
    if (raw.page != null || raw.col != null) layout = 'page';
    if (/page|ssd1306|vertical/.test(format)) layout = 'page';
    var dataRaw = firstDef(raw.data, raw.pixels, raw.framebuffer, raw.hex, raw.bytes, raw.b64, raw.base64);
    var data = null;
    if (Array.isArray(dataRaw)) {
      data = new Uint8Array(dataRaw.length);
      for (var i = 0; i < dataRaw.length; i++) {
        var v = toInt(dataRaw[i]);
        data[i] = (v == null ? 0 : v) & 0xFF;
      }
    } else if (typeof dataRaw === 'string') {
      data = base64OrHexToBytes(dataRaw);
    }
    // 兼容 raw.hex / raw.base64 单独字段
    if (!data) {
      var extra = firstDef(raw.hex, raw.base64, raw.b64);
      if (typeof extra === 'string') data = base64OrHexToBytes(extra);
    }
    if (data) {
      var need = screenBytesNeeded(w, h, format, layout);
      if (data.length < need) {
        // 单色数据可能被逐位展开，尝试自动识别
        if (format.indexOf('1') >= 0 || format.indexOf('mono') >= 0 || format.indexOf('bpp') >= 0 || format === 'bitmap') {
          need = screenBytesNeeded(w, h, format, layout);
        } else if (format.indexOf('rgb565') >= 0 && data.length >= w * h * 2) {
          need = w * h * 2;
        } else {
          need = data.length;
        }
      }
      if (data.length < need) data = null;
    }
    return { w: w, h: h, format: format, addr: addr, layout: layout, info: info, data: data, raw: raw };
  }

  function screenBytesNeeded(w, h, format, layout) {
    if (format.indexOf('rgb565') >= 0) return w * h * 2;
    if (format.indexOf('rgba') >= 0 || format.indexOf('32') >= 0) return w * h * 4;
    if (format.indexOf('rgb888') >= 0 || format.indexOf('rgb24') >= 0) return w * h * 3;
    if (format.indexOf('1') >= 0 || format.indexOf('mono') >= 0 || format.indexOf('bpp') >= 0 || format === 'bitmap') {
      return layout === 'page' ? w * Math.ceil(h / 8) : Math.ceil(w * h / 8);
    }
    return w * h;
  }

  // 取单像素灰度（0~255）：供 canvas 渲染与自测共用，兼容 page/column 1bpp。
  function screenPixelAt(screen, x, y) {
    if (!screen || !screen.data) return 0;
    var w = screen.w, h = screen.h, d = screen.data;
    if (x < 0 || y < 0 || x >= w || y >= h) return 0;
    var fmt = String(screen.format || '');
    if (fmt.indexOf('rgb565') >= 0) {
      var v = (d[(y * w + x) * 2] << 8) | d[(y * w + x) * 2 + 1];
      var r = (v >> 11) & 0x1F, g = (v >> 5) & 0x3F, b = v & 0x1F;
      return Math.round(((r * 255 / 31) + (g * 255 / 63) + (b * 255 / 31)) / 3);
    }
    if (fmt.indexOf('rgb888') >= 0 || fmt.indexOf('rgb24') >= 0) {
      var o = (y * w + x) * 3;
      return Math.round((d[o] + d[o + 1] + d[o + 2]) / 3);
    }
    if (fmt.indexOf('rgba') >= 0 || fmt.indexOf('32') >= 0) {
      var p = (y * w + x) * 4;
      return Math.round((d[p] + d[p + 1] + d[p + 2]) / 3);
    }
    if (fmt.indexOf('1') >= 0 || fmt.indexOf('mono') >= 0 || fmt.indexOf('bpp') >= 0) {
      var bit;
      if (screen.layout === 'page') {
        var idx = (y >> 3) * w + x;
        bit = (d[idx] >> (y & 7)) & 1;
      } else {
        var i = y * w + x;
        bit = (d[i >> 3] >> (7 - (i & 7))) & 1;
      }
      return bit ? 255 : 0;
    }
    return d[y * w + x] || 0;
  }

  function base64OrHexToBytes(s) {
    var str = String(s).trim();
    if (/^[0-9A-Fa-f\s]+$/.test(str) && str.replace(/\s/g, '').length % 2 === 0) {
      var hx = str.replace(/\s/g, '');
      var out = new Uint8Array(hx.length / 2);
      for (var i = 0; i < out.length; i++) out[i] = parseInt(hx.substr(i * 2, 2), 16);
      return out;
    }
    try {
      if (typeof atob === 'function') {
        var bin = atob(str);
        var arr = new Uint8Array(bin.length);
        for (var k = 0; k < bin.length; k++) arr[k] = bin.charCodeAt(k);
        return arr;
      }
      if (typeof Buffer !== 'undefined') return new Uint8Array(Buffer.from(str, 'base64'));
    } catch (e) { return null; }
    return null;
  }

  /* ============================== 渲染（纯函数，返回 HTML 字符串） ============================== */
  function renderRegisterRows(model) {
    if (!model || !model.regs) return '';
    var html = '';
    var hints = { sp: '栈顶', lr: '返回地址', pc: '程序计数器' };
    for (var i = 0; i < model.regs.length; i++) {
      var r = model.regs[i];
      var wide = (r.name === 'sp' || r.name === 'lr' || r.name === 'pc');
      html += '<div class="reg-row' + (wide ? ' wide' : '') + (r.name === 'pc' ? ' reg-pc' : '') +
        (r.name === 'sp' ? ' reg-sp' : '') + (r.name === 'lr' ? ' reg-lr' : '') +
        '" data-reg="' + r.name + '">' +
        '<span class="reg-name">' + r.name + '</span>' +
        '<span class="reg-val">' + hex32(r.value) + '</span>' +
        (wide ? '<span class="reg-hint">' + (hints[r.name] || '') + '</span>' : '') +
        '</div>';
    }
    return html;
  }

  function renderFlags(model) {
    var flags = (model && model.flags) || { N: false, Z: false, C: false, V: false };
    var html = '<div class="flag-chips">';
    ['N', 'Z', 'C', 'V'].forEach(function (f) {
      html += '<span class="flag-chip' + (flags[f] ? ' on' : '') + '" data-flag="' + f + '">' + f + '</span>';
    });
    html += '</div><span class="xpsr-val">' + (model ? hex32(model.xpsr) : '—') + '</span>';
    return html;
  }

  function mnemClass(mnem) {
    var m = String(mnem || '').toLowerCase();
    if (/^(b|bl|blx|bx|cbz|cbnz)$/.test(m) || /^b(eq|ne|hs|lo|cs|cc|mi|pl|vs|vc|hi|ls|ge|lt|gt|le)$/.test(m)) return 'mnem-branch';
    if (/^bl/.test(m)) return 'mnem-call';
    if (/^(pop|push|ldm|stm)$/.test(m)) return 'mnem-stack';
    if (/^(ldr|str|ldrb|strb|ldrh|strh|ldrsb|ldrsh)$/.test(m)) return 'mnem-mem';
    if (/^(mov|movs|adds|subs|add|sub|cmp|tst|ands|orrs|eors|lsls|lsrs|asrs|muls)$/.test(m)) return 'mnem-alu';
    return '';
  }

  function renderDisasmRows(lines, ctx) {
    ctx = ctx || {};
    var pc = ctx.pc != null ? (ctx.pc & ~1) : null;
    var bps = ctx.breakpoints || [];
    var selected = ctx.selected != null ? (ctx.selected & ~1) : null;
    var html = '';
    for (var i = 0; i < lines.length; i++) {
      var l = lines[i];
      var isPc = pc != null && l.addr === pc;
      var hasBp = bps.indexOf(l.addr) >= 0;
      html += '<div class="d-row' + (isPc ? ' pc-line' : '') + (selected === l.addr ? ' selected' : '') +
        '" data-addr="' + hex0x(l.addr) + '">' +
        '<span class="d-pc">' + (isPc ? '▶' : '') + '</span>' +
        '<button type="button" class="d-bp' + (hasBp ? ' has-bp' : '') + '" data-addr="' + hex0x(l.addr) +
        '" data-action="toggle-bp" title="' + (hasBp ? '删除断点' : '添加断点') + '" aria-label="断点"></button>' +
        '<span class="d-addr">' + hex8(l.addr) + '</span>' +
        '<span class="d-bytes">' + escapeHtml(l.bytes) + '</span>' +
        '<span class="d-mnem ' + mnemClass(l.mnem) + '">' + escapeHtml(l.mnem) + '</span>' +
        '<span class="d-op">' + escapeHtml(l.op) + '</span>' +
        '</div>';
    }
    return html;
  }

  function asciiOf(bytes, start, len) {
    var s = '';
    for (var i = 0; i < len; i++) {
      var b = bytes[start + i];
      s += (b >= 0x20 && b < 0x7F) ? String.fromCharCode(b) : '·';
    }
    return s;
  }

  function renderMemoryTable(bytes, baseAddr, pc) {
    baseAddr = align(baseAddr || 0, 16);
    pc = pc == null ? null : (pc & ~1);
    var html = '<div class="mem-row mem-head"><span class="mem-addr">地址</span><span class="mem-hex">' +
      ' 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F</span><span class="mem-ascii">ASCII</span></div>';
    for (var row = 0; row < bytes.length; row += 16) {
      var addr = (baseAddr + row) >>> 0;
      var take = Math.min(16, bytes.length - row);
      var hexs = '';
      for (var i = 0; i < 16; i++) {
        hexs += (i < take ? hex2(bytes[row + i]) : '  ') + ' ';
      }
      var hasPc = pc != null && pc >= addr && pc < addr + 16;
      html += '<div class="mem-row' + (hasPc ? ' pc-row' : '') + '" data-addr="' + hex0x(addr) + '">' +
        '<span class="mem-addr">' + hex8(addr) + '</span>' +
        '<span class="mem-hex">' + hexs.replace(/\s+$/, '') + '</span>' +
        '<span class="mem-ascii">' + escapeHtml(asciiOf(bytes, row, take)) + '</span>' +
        '</div>';
    }
    return html;
  }

  function normalizeLogFilter(filter) {
    return String(filter || '').trim().toLowerCase().replace(/^0x/, '');
  }

  function renderPeripheralRows(entries, filter) {
    var f = normalizeLogFilter(filter);
    var out = '';
    var shown = 0;
    for (var i = entries.length - 1; i >= 0 && shown < LOG_MAX_ROWS; i--) {
      var e = entries[i];
      var isEvent = e.dir === '!';
      var text = [e.reg || '', e.note || '', e.text || '', e.addr != null ? hex8(e.addr) : '', e.pc != null ? hex8(e.pc) : ''].join(' ').toLowerCase();
      if (f && text.indexOf(f) < 0 && String(e.dir || '').toLowerCase() !== f) continue;
      shown++;
      if (e.text && !e.addr && !e.reg) {
        out += '<div class="plog-row raw"><span class="plog-note">' + escapeHtml(e.text) + '</span></div>';
        continue;
      }
      var valStr = e.value == null ? '' : (typeof e.value === 'number' ? hex32(e.value) : escapeHtml(e.value));
      out += '<div class="plog-row' + (isEvent ? ' plog-event' : '') + '">' +
        '<span class="plog-dir ' + (isEvent ? 'evt' : (e.dir === 'W' ? 'w' : 'r')) + '">' + escapeHtml(e.dir) + '</span>' +
        '<span class="plog-reg">' + escapeHtml(e.reg || (e.addr != null ? 'MMIO' : '—')) + '</span>' +
        '<span class="plog-val">' + valStr + '</span>' +
        '<span class="plog-pc">' + (e.pc != null ? '@' + hex8(e.pc) : '') + '</span>' +
        (e.note ? '<span class="plog-note">' + escapeHtml(e.note) + '</span>' : '') +
        '</div>';
    }
    if (!out) out = '<div class="empty">暂无外设访问记录</div>';
    return out;
  }

  function renderConsole(text) {
    var t = String(text == null ? '' : text);
    if (!t) return '<div class="empty">UART 无输出</div>';
    var lines = t.split(/\r?\n/);
    if (lines.length > 400) lines = lines.slice(lines.length - 400);
    var html = '';
    for (var i = 0; i < lines.length; i++) {
      html += '<div class="uart-line">' + escapeHtml(lines[i]) + '</div>';
    }
    return html;
  }

  function renderCallstack(stack) {
    if (!stack || !stack.length) return '<div class="empty">模拟器未提供调用栈（可选扩展，见 API.md）</div>';
    var html = '';
    for (var i = 0; i < stack.length; i++) {
      var f = stack[i];
      html += '<div class="stack-row' + (f.current ? ' current' : '') + '">' +
        '<span class="stack-idx">#' + i + '</span>' +
        '<span class="stack-addr">' + (f.addr != null ? hex32(f.addr) : '—') + '</span>' +
        '<span class="stack-name">' + escapeHtml(f.name || '???') + '</span>' +
        '</div>';
    }
    return html;
  }

  function renderFunctionInfo(model, resolved) {
    resolved = resolved || {};
    var cur = resolved.current || (model && model.func) || null;
    var next = resolved.nearest || (model && model.nearestFunc) || null;
    function line(label, info, extraClass) {
      var name = info && info.name ? info.name : '—';
      var addr = info && info.addr != null ? hex32(info.addr) : '';
      return '<div class="func-line ' + (extraClass || '') + '"><span class="func-label">' + label + '</span>' +
        '<span class="func-name">' + escapeHtml(name) + '</span>' +
        (addr ? '<span class="func-addr">@' + escapeHtml(addr) + '</span>' : '') +
        '</div>';
    }
    return line('PC 函数', cur, 'cur') + line('最近函数', next, 'next');
  }

  function renderScreenPlaceholder(text) {
    return '<div class="lcd-placeholder-inner"><div class="lcd-icon">▦</div><div>' +
      escapeHtml(text || 'LCD 屏幕未连接') + '</div><div class="lcd-hint">模拟器提供 /api/screen 后自动显示</div></div>';
  }

  function formatScreenMeta(screen, rotated) {
    if (!screen) return '未连接';
    var s = screen.w + '×' + screen.h + ' · ' + screen.format + (screen.layout === 'page' ? '/page' : '');
    s += screen.addr != null ? ' · FB ' + hex32(screen.addr) : ' · FB 未映射';
    if (screen.info && screen.info.frames != null) s += ' · ' + formatCount(screen.info.frames) + ' 帧';
    if (rotated && screen.layout === 'page') s += ' · 面板 90°镜像';
    return s;
  }

  function formatLcdMeta(lcd) {
    if (!lcd) return '未连接';
    var s = (lcd.w || '?') + '×' + (lcd.h || '?') + ' · ' + lcd.format;
    if (lcd.addr != null) s += ' · FB ' + hex32(lcd.addr);
    if (lcd.frames != null) s += ' · ' + formatCount(lcd.frames) + ' 帧';
    if (lcd.nonzero != null) s += ' · 非零 ' + lcd.nonzero + 'B';
    return s;
  }

  /* ============================== 函数名解析（纯函数） ============================== */
  function lookupFunction(funcs, addr) {
    if (!funcs || !funcs.length || addr == null) return null;
    addr = addr >>> 0;
    var lo = 0, hi = funcs.length - 1, best = -1;
    while (lo <= hi) {
      var mid = (lo + hi) >> 1;
      if (funcs[mid].addr <= addr) { best = mid; lo = mid + 1; } else { hi = mid - 1; }
    }
    if (best < 0) return null;
    var f = funcs[best];
    if (addr < f.end) return { addr: f.addr, end: f.end, name: f.name, heuristic: false };
    return { addr: f.addr, end: f.end, name: f.name, heuristic: false, outside: true };
  }

  function nextFunction(funcs, addr) {
    if (!funcs || !funcs.length || addr == null) return null;
    for (var i = 0; i < funcs.length; i++) {
      if (funcs[i].addr > (addr >>> 0)) return { addr: funcs[i].addr, end: funcs[i].end, name: funcs[i].name };
    }
    return null;
  }

  // 没有函数表时的启发式：在已缓存反汇编窗口里向后找 "pop {..., pc}"/"bx lr" 后的下一行
  function heuristicFunction(lines, addr) {
    if (!lines || !lines.length || addr == null) return null;
    addr = addr & ~1;
    var idx = -1;
    for (var i = 0; i < lines.length; i++) {
      if (lines[i].addr === addr) { idx = i; break; }
      if (lines[i].addr > addr) break;
    }
    if (idx < 0) return null;
    for (var j = idx; j >= 0; j--) {
      var l = lines[j];
      var m = String(l.mnem || '').toLowerCase();
      var op = String(l.op || '').toLowerCase();
      var isRet = (m === 'pop' && /\bpc\b/.test(op)) || ((m === 'bx' || m === 'blx') && /\blr\b/.test(op)) ||
        (m === 'mov' && /\bpc\s*,\s*lr\b/.test(op));
      if (isRet && j + 1 < lines.length) {
        var start = lines[j + 1];
        return { addr: start.addr, name: 'sub_' + hex8(start.addr), heuristic: true };
      }
    }
    return { addr: lines[0].addr, name: 'sub_' + hex8(lines[0].addr), heuristic: true };
  }

  function mergeBreakpoints(serverBps, localBps) {
    var out = [];
    (serverBps || []).concat(localBps || []).forEach(function (a) {
      if (a != null && out.indexOf(a) < 0) out.push(a);
    });
    return out.sort(function (x, y) { return x - y; });
  }

  /* ============================== 浏览器运行时 ============================== */
  var S = {
    mock: false, apiBase: DEFAULT_API,
    dom: {}, model: null, rawState: null,
    disasm: null, disasmPc: null, center: null, followPc: true,
    bps: [], selected: null,
    memAddr: 0x080030D4, mem: null,
    funcs: null, funcsSupported: false,
    uart: '', log: [], callstack: [],
    screen: null, screenSupported: true, screenFail: 0, screenFrames: 0, screenFps: 0,
    pollTimer: null, screenTimer: null, connected: false, failCount: 0,
    rtt: null, ipc: null, prevCount: null, prevTime: null,
    held: {}, batteryTimer: null, lastBatteryMv: null, savePath: null, lastSent: null,
    inputsOverride: null, demoInject: null, demoMv: null, demoDone: false,
    screenRotate: true,
    bannerShown: false, toastTimer: null, booted: false
  };

  function $(id) {
    var key = String(id).replace(/^#/, '');
    return S.dom[key] || (S.dom[key] = document.getElementById(key));
  }
  function isDomReady() { return typeof document !== 'undefined'; }

  function readConfig() {
    var params = {};
    try {
      var search = (root.location && root.location.search) || '';
      var q = new URLSearchParams(search);
      params.mock = q.get('mock');
      params.api = q.get('api');
      params.demo = q.get('demo');
      params.mv = q.get('mv');
      params.screen = q.get('screen');
    } catch (e) { /* ignore */ }
    var wantMock = params.mock === '1' || params.mock === 'true' || params.mock === 'yes';
    if (wantMock && !root.AD100Mock) {
      showBanner('未找到 mock.js：请确认文件存在，或去掉 ?mock=1 连接真实模拟器。');
      wantMock = false;
    }
    S.mock = wantMock;
    if (params.api) S.apiBase = String(params.api).replace(/\/+$/, '');
    // 无头截图 / 联调辅助：?mv=3900&demo=key_press:power,key:mode
    S.demoInject = params.demo || null;
    S.demoMv = params.mv != null ? toInt(params.mv) : null;
    if (params.screen === 'raw' || params.screen === '0') S.screenRotate = false;
    var hint = document.getElementById('apiHint');
    if (hint) hint.textContent = S.mock ? 'mock.js（离线假数据）' : S.apiBase.replace(/^https?:\/\//, '');
  }

  // ?demo= 一次性注入序列，便于自动化截图验证（格式：cmd[:key[:ms]],...）
  function runDemo() {
    if (S.demoDone) return;
    S.demoDone = true;
    var actions = [];
    if (S.demoMv != null) actions.push({ cmd: 'battery', extra: { mv: S.demoMv } });
    String(S.demoInject || '').split(',').forEach(function (token) {
      token = token.trim();
      if (!token) return;
      var parts = token.split(':');
      var name = parts[0];
      var key = parts[1] || null;
      if (name === 'battery' && parts[1]) actions.push({ cmd: 'battery', extra: { mv: toInt(parts[1]) || 3700 } });
      else if (name === 'key' && key) actions.push({ cmd: 'key', extra: { key: key, down: true } });
      else if (name === 'key_up' && key) actions.push({ cmd: 'key_up', extra: { key: key } });
      else if (name === 'key_press' && key) actions.push({ cmd: 'key_press', extra: { key: key, ms: toInt(parts[2]) || 80 } });
      else if (name === 'screen_save') actions.push({ cmd: 'screen_save', extra: null });
      else if (name === 'boot_interactive' || name === 'jump_main') actions.push({ cmd: name, extra: { warmup: toInt(parts[1]) || 1000000 } });
      else if (name === 'clear_inputs') actions.push({ cmd: 'clear_inputs', extra: null });
    });
    if (!actions.length) return;
    function next() {
      var a = actions.shift();
      if (!a) return;
      sendCmd(a.cmd, a.extra).then(function () { setTimeout(next, 150); }, function () { setTimeout(next, 150); });
    }
    setTimeout(next, 300);
  }

  function apiFetch(path, opts) {
    if (S.mock && root.AD100Mock) return root.AD100Mock.fetch(path, opts);
    var options = opts || {};
    var ctrl = null;
    var timer = null;
    if (typeof AbortController !== 'undefined') {
      ctrl = new AbortController();
      options.signal = ctrl.signal;
      timer = setTimeout(function () { try { ctrl.abort(); } catch (e) {} }, 3500);
    }
    return fetch(S.apiBase + path, options).then(function (r) {
      if (timer) clearTimeout(timer);
      return r;
    }, function (err) {
      if (timer) clearTimeout(timer);
      throw err;
    });
  }

  function apiJson(path, opts) {
    return apiFetch(path, opts).then(function (r) {
      if (!r.ok) throw new Error('HTTP ' + r.status);
      return r.json();
    });
  }

  function schedulePoll() {
    if (S.pollTimer) clearTimeout(S.pollTimer);
    var interval = pickPollInterval(S.model ? S.model.running : false, S.failCount > 0, document.hidden);
    if (S.mock && S.failCount > 0) interval = POLL_PAUSED;
    S.pollTimer = setTimeout(pollState, interval);
  }

  function kickPoll(delay) {
    if (S.pollTimer) clearTimeout(S.pollTimer);
    S.pollTimer = setTimeout(pollState, delay == null ? 30 : delay);
  }

  function pollState() {
    var t0 = nowMs();
    apiJson('/api/state').then(function (raw) {
      var model = normalizeState(raw);
      var rtt = nowMs() - t0;
      S.rawState = raw;
      S.rtt = rtt;
      var now = nowMs();
      if (S.prevCount != null && S.prevTime != null && now > S.prevTime && model.running) {
        var delta = model.insnCount - S.prevCount;
        if (delta >= 0) {
          var ipc = computeIpc(delta, now - S.prevTime);
          if (ipc != null) S.ipc = ipc;
        }
      }
      S.prevCount = model.insnCount;
      S.prevTime = now;
      S.failCount = 0;
      applyModel(model);
      S.connected = true;
      updateConnectionUi();
      if (!S.demoDone && (S.demoInject || S.demoMv != null)) runDemo();
      schedulePoll();
    }).catch(function (err) {
      onFetchError(err);
    });
  }

  function onFetchError(err) {
    S.failCount++;
    S.connected = false;
    updateConnectionUi();
    if (S.failCount === 1) {
      showToast('无法连接 ' + (S.mock ? 'mock' : S.apiBase) + '：' + (err && err.message ? err.message : '网络错误'));
      if (!S.bannerShown) showBanner(buildDisconnectedMessage(err));
    }
    schedulePoll();
  }

  function buildDisconnectedMessage(err) {
    var msg = '未连接到模拟器（' + S.apiBase + '）。请先启动 emu/server.py，或使用 mock 模式自测。';
    if (err && /abort|Failed to fetch|NetworkError|load failed/i.test(String(err.message || err))) {
      msg += ' 若是跨域打开，请确认模拟器返回 Access-Control-Allow-Origin 头。';
    }
    return msg;
  }

  function applyModel(model) {
    S.model = model;
    // 断点：服务器返回了断点字段时以其为准（mock 会回传）
    if (model.hasBpsField) {
      S.bps = mergeBreakpoints(model.breakpoints, []);
    }
    if (model.uart !== undefined && model.uart !== null) S.uart = model.uart;
    S.log = model.log || [];
    S.callstack = model.callstack || [];

    // 顶栏
    $('#insnCount').textContent = formatCount(model.insnCount);
    var ipcVal = model.ipc != null && model.ipc > 0 ? model.ipc : S.ipc;
    $('#ipcVal').textContent = formatIpc(ipcVal);
    $('#rttVal').textContent = S.rtt == null ? '—' : Math.round(S.rtt) + ' ms';
    var phaseEl = $('#phaseInfo');
    if (phaseEl) {
      phaseEl.textContent = formatPhaseInfo(model);
      phaseEl.classList.toggle('in-loop', model.inMainLoop === true || isMainLoopPc(model.pc) ||
        model.taskPhase === 'scheduler' || model.taskPhase === 'main_loop');
    }
    var bootBtn = $('#btnBoot');
    if (bootBtn) bootBtn.classList.toggle('active', model.inMainLoop === true);
    var faultEl = $('#faultVal');
    if (faultEl) {
      faultEl.textContent = model.faults.length;
      if (faultEl.parentNode && faultEl.parentNode.classList) {
        faultEl.parentNode.classList.toggle('fault', model.faults.length > 0);
      }
      if (model.faults.length > (S.faultCount || 0)) {
        showToast('模拟器报告 ' + model.faults.length + ' 个异常（faults）');
      }
      S.faultCount = model.faults.length;
    }

    // 寄存器 / 标志 / 函数信息
    $('#regGrid').innerHTML = renderRegisterRows(model);
    $('#flagRow').innerHTML = renderFlags(model);
    var resolved = resolveFunctionInfo(model);
    $('#funcInfo').innerHTML = renderFunctionInfo(model, resolved);

    // 外设日志 / 控制台 / 调用栈
    renderPeripheralPanel();
    renderUartPanel();
    $('#callstack').innerHTML = renderCallstack(S.callstack);
    // 兼容：/api/state 未带 inputs 时，用命令响应里的 inputs 回显
    if (!model.inputs && S.inputsOverride) model.inputs = S.inputsOverride;
    applyInputs(model);
    applyRfPanel(model);
    if (model.lastScreenSave) showSaveResult('最近保存：' + model.lastScreenSave);

    // 反汇编跟随 PC
    var pcEven = model.pc & ~1;
    if (S.followPc && (S.disasmPc !== pcEven || !S.disasm)) {
      S.disasmPc = pcEven;
      S.center = pcEven;
      refreshDisasm(pcEven, true);
    } else if (S.disasm) {
      renderDisasmPanel();
    }
    updateRunButtons();
    updateRangeLabel();
  }

  function resolveFunctionInfo(model) {
    var current = model.func || null;
    var nearest = model.nearestFunc || null;
    if (!current && S.funcs) {
      current = lookupFunction(S.funcs, model.pc);
    }
    if (!nearest && S.funcs) {
      nearest = nextFunction(S.funcs, current ? current.addr : model.pc);
    }
    if (!current && S.disasm && S.disasm.lines) {
      current = heuristicFunction(S.disasm.lines, model.pc);
    }
    if (!nearest && S.disasm && S.disasm.lines) {
      nearest = heuristicFunction(S.disasm.lines, (current && current.addr != null ? current.addr : model.pc) + 2);
      if (nearest && current && nearest.addr === current.addr) nearest = null;
    }
    return { current: current, nearest: nearest };
  }

  function refreshFunctions() {
    apiJson('/api/functions').then(function (raw) {
      var list = raw && (raw.functions || raw.funcs || raw);
      if (!Array.isArray(list)) throw new Error('bad functions payload');
      var out = [];
      for (var i = 0; i < list.length; i++) {
        var f = list[i];
        if (!f) continue;
        var a = toAddr(firstDef(f.addr, f.address, f.start));
        var e = toAddr(firstDef(f.end, f.endAddr, f.end_address));
        if (a == null) continue;
        if (e == null) e = a + (toInt(f.size) || 2);
        if (e <= a) continue;
        out.push({ addr: a, end: e, name: String(firstDef(f.name, f.label, f.symbol, 'sub_' + hex8(a))) });
      }
      out.sort(function (x, y) { return x.addr - y.addr; });
      S.funcs = out.length ? out : null;
      S.funcsSupported = !!out.length;
      if (S.model) {
        $('#funcInfo').innerHTML = renderFunctionInfo(S.model, resolveFunctionInfo(S.model));
      }
    }).catch(function () {
      S.funcs = null;
      S.funcsSupported = false;
    });
  }

  function refreshDisasm(addr, force) {
    addr = addr >>> 0;
    if (!force && S.disasm && S.disasm.addr === addr) { renderDisasmPanel(); return; }
    var requested = addr;
    apiJson('/api/disasm?addr=' + hex0x(addr) + '&count=' + DISASM_COUNT + '&n=' + DISASM_COUNT).then(function (raw) {
      var d = normalizeDisasm(raw, requested);
      if (!d.lines.length) throw new Error('empty disasm');
      S.disasm = d;
      S.disasmPc = (S.model ? S.model.pc : requested) & ~1;
      if (S.followPc) S.center = S.disasmPc;
      renderDisasmPanel();
      updateRangeLabel();
      if (S.model) {
        $('#funcInfo').innerHTML = renderFunctionInfo(S.model, resolveFunctionInfo(S.model));
      }
    }).catch(function (err) {
      if (!S.mock && S.failCount > 0) return;
      $('#disasmBody').innerHTML = '<div class="empty">反汇编加载失败：' + escapeHtml(err.message || err) + '</div>';
    });
  }

  function renderDisasmPanel() {
    var lines = (S.disasm && S.disasm.lines) || [];
    if (!lines.length) {
      $('#disasmBody').innerHTML = '<div class="empty">正在连接模拟器，等待反汇编数据…</div>';
      return;
    }
    var html = renderDisasmRows(lines, {
      pc: S.model ? S.model.pc : null,
      breakpoints: S.bps,
      selected: S.selected
    });
    var body = $('#disasmBody');
    body.innerHTML = html;
    // 保持滚动位置：跟随 PC 时滚到高亮行
    if (S.followPc) {
      var pcRow = body.querySelector('.pc-line');
      if (pcRow) {
        var offset = pcRow.offsetTop - body.clientHeight / 2 + pcRow.offsetHeight / 2;
        body.scrollTop = Math.max(0, offset);
      }
    }
  }

  function updateRangeLabel() {
    var lines = (S.disasm && S.disasm.lines) || [];
    var el = $('#disasmRange');
    if (!el) return;
    if (!lines.length) { el.textContent = '—'; return; }
    el.textContent = hex0x(lines[0].addr) + ' – ' + hex0x(lines[lines.length - 1].addr) + ' · ' + lines.length + ' 行';
  }

  function scrollDisasm(dir) {
    var lines = (S.disasm && S.disasm.lines) || [];
    if (!lines.length) return;
    var idx = dir > 0 ? Math.min(lines.length - 1, 40) : Math.max(0, lines.length - 21);
    var target = lines[idx].addr;
    S.followPc = false;
    updateFollowUi();
    S.center = target;
    S.disasmPc = null;
    refreshDisasm(target, true);
  }

  function updateFollowUi() {
    var chk = $('#chkFollow');
    if (chk) chk.checked = S.followPc;
    var btn = $('#btnFollowPc');
    if (btn) btn.classList.toggle('active', S.followPc);
  }

  function refreshMem() {
    var addr = S.memAddr >>> 0;
    apiJson('/api/mem?addr=' + hex0x(addr) + '&len=' + MEM_LEN).then(function (raw) {
      var mem = normalizeMem(raw, addr, MEM_LEN);
      S.mem = mem;
      S.memAddr = mem.addr;
      $('#memTable').innerHTML = renderMemoryTable(mem.bytes, mem.addr, S.model ? S.model.pc : null);
      $('#memInfo').textContent = hex0x(mem.addr) + ' – ' + hex0x((mem.addr + mem.len - 1) >>> 0);
      var inp = $('#memAddrInput');
      if (inp && document.activeElement !== inp) inp.value = hex8(mem.addr);
    }).catch(function (err) {
      if (S.mock) showToast('内存读取失败：' + (err.message || err));
    });
  }

  function jumpMem(addr) {
    S.memAddr = align(addr >>> 0, 16);
    refreshMem();
  }

  function refreshScreen() {
    if (!S.screenSupported && !S.mock) {
      // 每 10 次轮询重试一次（模拟器可能稍后才提供屏幕数据）
      S.screenFail++;
      if (S.screenFail % 10 !== 0) return;
    }
    apiFetch('/api/screen').then(function (r) {
      if (!r.ok) throw new Error('HTTP ' + r.status);
      return r.json();
    }).then(function (raw) {
      var screen = normalizeScreen(raw);
      if (!screen || !screen.data) throw new Error('屏幕数据无效');
      S.screenFail = 0;
      S.screenSupported = true;
      S.screen = screen;
      drawScreen(screen);
      S.screenFrames++;
    }).catch(function () {
      S.screenSupported = false;
      S.screen = null;
      var lcdInfo = S.model && S.model.lcd;
      var text = lcdInfo
        ? 'LCD ' + (lcdInfo.w || '?') + '×' + (lcdInfo.h || '?') + ' · ' + lcdInfo.format + '（等待 /api/screen 帧数据）'
        : 'LCD 屏幕未连接（等待 /api/screen 数据）';
      if (S.lcdPlaceholderText !== text) {
        S.lcdPlaceholderText = text;
        showScreenPlaceholder(text);
        $('#lcdMeta').textContent = lcdInfo ? formatLcdMeta(lcdInfo) : '未连接';
      }
    });
  }

  function showScreenPlaceholder(text) {
    var wrap = $('#lcdWrap');
    var empty = $('#lcdEmpty');
    if (!wrap || !empty) return;
    empty.innerHTML = renderScreenPlaceholder(text);
    empty.hidden = false;
    $('#lcdCanvas').hidden = true;
  }

  // 把 1bpp page/column 帧渲染到 canvas。
  // 默认应用 emu/main.py --screen 的 panel 方向：rotate90 + mirror
  // （结果等价于把原始 128×64 帧转置成 64×128 竖屏）。
  function drawScreen(screen) {
    var cv = $('#lcdCanvas');
    var empty = $('#lcdEmpty');
    if (!cv) return;
    var w = screen.w, h = screen.h;
    var rotated = !!(S.screenRotate && screen.layout === 'page');
    var outW = rotated ? h : w;
    var outH = rotated ? w : h;
    if (cv.width !== outW || cv.height !== outH) { cv.width = outW; cv.height = outH; }
    var ctx = cv.getContext('2d');
    if (!ctx) return;
    var img = ctx.createImageData(outW, outH);
    var d = screen.data;
    var fmt = screen.format || '';
    var i, o;
    if (fmt.indexOf('rgb565') >= 0) {
      for (i = 0, o = 0; i < w * h; i++, o += 4) {
        var v = (d[i * 2] << 8) | d[i * 2 + 1];
        var r = (v >> 11) & 0x1F, g = (v >> 5) & 0x3F, b = v & 0x1F;
        img.data[o] = (r << 3) | (r >> 2);
        img.data[o + 1] = (g << 2) | (g >> 4);
        img.data[o + 2] = (b << 3) | (b >> 2);
        img.data[o + 3] = 255;
      }
    } else if (fmt.indexOf('rgba') >= 0 || fmt.indexOf('32') >= 0) {
      for (i = 0, o = 0; i < w * h; i++, o += 4) {
        img.data[o] = d[i * 4]; img.data[o + 1] = d[i * 4 + 1];
        img.data[o + 2] = d[i * 4 + 2]; img.data[o + 3] = d[i * 4 + 3];
      }
    } else if (fmt.indexOf('rgb888') >= 0 || fmt.indexOf('rgb24') >= 0) {
      for (i = 0, o = 0; i < w * h; i++, o += 4) {
        img.data[o] = d[i * 3]; img.data[o + 1] = d[i * 3 + 1];
        img.data[o + 2] = d[i * 3 + 2]; img.data[o + 3] = 255;
      }
    } else if (fmt.indexOf('1') >= 0 || fmt.indexOf('mono') >= 0 || fmt.indexOf('bpp') >= 0) {
      var pageLayout = screen.layout === 'page';
      if (pageLayout && rotated) {
        // canvas(X,Y) = raw(row=X, col=Y)，即 rotate90+mirror（panel 方向）
        for (var CY = 0; CY < outH; CY++) {
          for (var CX = 0; CX < outW; CX++) {
            var vv = (d[(CX >> 3) * w + CY] >> (CX & 7)) & 1;
            o = (CY * outW + CX) * 4;
            var cc = vv ? 255 : 0;
            img.data[o] = cc; img.data[o + 1] = cc; img.data[o + 2] = cc; img.data[o + 3] = 255;
          }
        }
      } else {
        for (var py = 0; py < h; py++) {
          for (var px = 0; px < w; px++) {
            var bit;
            if (pageLayout) {
              // SSD1306 page/column：字节 = 纵向 8 像素，bit0 在上
              bit = (d[(py >> 3) * w + px] >> (py & 7)) & 1;
            } else {
              var ii = py * w + px;
              bit = (d[ii >> 3] >> (7 - (ii & 7))) & 1;
            }
            o = (py * w + px) * 4;
            var c = bit ? 255 : 0;
            img.data[o] = c; img.data[o + 1] = c; img.data[o + 2] = c; img.data[o + 3] = 255;
          }
        }
      }
    } else {
      for (i = 0; i < outW * outH; i++) {
        var g8 = d[i % (w * h)];
        o = i * 4;
        img.data[o] = g8; img.data[o + 1] = g8; img.data[o + 2] = g8; img.data[o + 3] = 255;
      }
    }
    ctx.putImageData(img, 0, 0);
    if (empty) empty.hidden = true;
    cv.hidden = false;
    fitCanvas(outW, outH);
    $('#lcdMeta').textContent = formatScreenMeta(screen, rotated);
  }

  function fitCanvas(w, h) {
    var cv = $('#lcdCanvas');
    if (!cv) return;
    var maxW = 320, maxH = 240;
    var scale = Math.min(maxW / w, maxH / h);
    cv.style.width = Math.round(w * scale) + 'px';
    cv.style.height = Math.round(h * scale) + 'px';
  }

  function renderPeripheralPanel() {
    var el = $('#periphList');
    if (!el) return;
    var filter = $('#logFilter') ? $('#logFilter').value : '';
    var prevScroll = el.scrollTop;
    el.innerHTML = renderPeripheralRows(S.log, filter);
    // 贴在底部时自动滚到最新
    if (el.dataset.follow !== '0') el.scrollTop = el.scrollHeight;
    else el.scrollTop = prevScroll;
    var count = $('#logCount');
    if (count) count.textContent = (filter ? '过滤 ' : '') + S.log.length + ' 条';
  }

  function renderUartPanel() {
    var el = $('#uartConsole');
    if (!el) return;
    el.innerHTML = renderConsole(S.uart);
    el.scrollTop = el.scrollHeight;
  }

  function updateRunButtons() {
    var running = S.model ? S.model.running : false;
    var btnRun = $('#btnRun'), btnPause = $('#btnPause');
    if (btnRun) btnRun.classList.toggle('active', running);
    if (btnPause) btnPause.classList.toggle('active', !running);
    var badge = $('#runState');
    if (badge) {
      badge.textContent = running ? '运行中' : '已暂停';
      badge.className = 'run-state ' + (running ? 'running' : 'paused');
    }
  }

  function updateConnectionUi() {
    var dot = $('#connDot');
    var text = $('#connText');
    var badge = $('#modeBadge');
    if (S.mock) {
      if (badge) { badge.textContent = 'MOCK'; badge.className = 'mode-badge mock'; }
    } else if (badge) {
      badge.textContent = 'LIVE';
      badge.className = 'mode-badge live';
    }
    if (dot) dot.className = 'conn-dot ' + (S.connected ? 'ok' : (S.mock ? 'mock' : 'bad'));
    var saveBtn = $('#btnScreenSave');
    if (saveBtn) saveBtn.disabled = !(S.connected || S.mock);
    var rfBtns = ['btnRfInit', 'btnRfPair', 'btnRfInject', 'btnRfRefresh'];
    for (var i = 0; i < rfBtns.length; i++) {
      var b = $('#' + rfBtns[i]);
      if (b) b.disabled = !(S.connected || S.mock);
    }
    if (text) {
      if (S.mock) text.textContent = 'MOCK 模拟数据';
      else if (S.connected) text.textContent = '已连接 · ' + (S.model && S.model.running ? '运行中' : '已暂停');
      else text.textContent = '未连接';
    }
    if (S.connected && S.bannerShown) hideBanner();
  }

  function showBanner(message) {
    var b = $('#connBanner');
    if (!b) return;
    $('#bannerText').textContent = message;
    b.hidden = false;
    S.bannerShown = true;
  }
  function hideBanner() {
    var b = $('#connBanner');
    if (b) b.hidden = true;
    S.bannerShown = false;
  }
  function showToast(msg) {
    var t = $('#toast');
    if (!t) return;
    t.textContent = msg;
    t.hidden = false;
    if (S.toastTimer) clearTimeout(S.toastTimer);
    S.toastTimer = setTimeout(function () { t.hidden = true; }, 3200);
  }

  function nowMs() {
    if (root.performance && typeof root.performance.now === 'function') return root.performance.now();
    return Date.now();
  }

  /* ---------- 命令 ---------- */
  function sendCmd(cmd, extra) {
    var body = { cmd: cmd };
    if (extra) for (var k in extra) if (Object.prototype.hasOwnProperty.call(extra, k)) body[k] = extra[k];
    return apiFetch('/api/cmd', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(body)
    }).then(function (r) {
      if (!r.ok) throw new Error('HTTP ' + r.status);
      return r.json();
    }).then(function (res) {
      if (res && res.ok === false) throw new Error(res.error || '命令被拒绝');
      // 命令响应里若带 inputs（emu 实现如此），立即回显，不必等 /api/state
      if (res && res.inputs) {
        S.inputsOverride = normalizeInputs(res.inputs);
        if (S.model) {
          S.model.inputs = S.inputsOverride;
          applyInputs(S.model);
        }
      }
      return res;
    });
  }

  function doRunPause() {
    var running = S.model ? S.model.running : false;
    sendCmd(running ? 'pause' : 'run').then(function () { kickPoll(40); })
      .catch(function (err) { showToast('命令失败：' + (err.message || err)); });
  }
  function doStep() {
    sendCmd('step').then(function () { kickPoll(40); })
      .catch(function (err) { showToast('单步失败：' + (err.message || err)); });
  }
  function doStepOver() {
    sendCmd('step_over').then(function () { kickPoll(40); })
      .catch(function () {
        // 模拟器不支持 step_over 时回退为普通单步
        sendCmd('step').then(function () { kickPoll(40); })
          .catch(function (err) { showToast('Step Over 失败：' + (err.message || err)); });
      });
  }
  function doReset() {
    sendCmd('reset').then(function () {
      S.prevCount = null; S.ipc = null; S.followPc = true; S.inputsOverride = null;
      updateFollowUi();
      kickPoll(120);
      refreshMem();
    }).catch(function (err) { showToast('复位失败：' + (err.message || err)); });
  }

  /* ---------- 开机 / 进入主循环（boot_interactive） ---------- */
  function setBootButtonBusy(busy) {
    var btn = $('#btnBoot');
    if (btn) btn.disabled = !!busy;
  }

  function checkBootState() {
    return apiJson('/api/state').then(function (raw) {
      var model = normalizeState(raw);
      S.model = model;
      S.connected = true;
      updateConnectionUi();
      applyModel(model);
      var okLoop = model.inMainLoop === true || isMainLoopPc(model.pc) ||
        model.taskPhase === 'scheduler' || model.taskPhase === 'main_loop';
      if (okLoop) {
        showToast('已进入主循环 0x' + hex8(model.pc) + (model.taskPhase ? ' · ' + mapTaskPhase(model.taskPhase) : ''));
        S.followPc = true;
        updateFollowUi();
        S.disasmPc = null;
        refreshDisasm(model.pc & ~1, true);
      } else {
        showToast('开机命令未确认生效（pc=0x' + hex8(model.pc) + '）');
      }
      schedulePoll();
      return model;
    }).catch(function (err) {
      showToast('开机后状态查询失败：' + (err.message || err));
      return null;
    }).then(function (model) {
      setBootButtonBusy(false);
      return model;
    });
  }

  function doBootInteractive() {
    setBootButtonBusy(true);
    noteSent('boot_interactive');
    showToast('正在开机：reset → 跳主循环 → settle…');
    sendCmd('boot_interactive', { warmup: 1000000 }).then(function (res) {
      if (res && res.inputs) {
        S.inputsOverride = normalizeInputs(res.inputs);
        if (S.model) { S.model.inputs = S.inputsOverride; applyInputs(S.model); }
      }
      if (res && res.pc != null) {
        S.followPc = true;
        updateFollowUi();
        S.disasmPc = null;
        S.prevCount = null;
        showToast('已进入主循环 0x' + hex8(res.pc) + (res.taskPhase ? ' · ' + mapTaskPhase(res.taskPhase) : ''));
        kickPoll(150);
        setBootButtonBusy(false);
        setTimeout(function () { checkBootState(); }, 1200);
      } else {
        setTimeout(function () { checkBootState(); }, 1500);
      }
    }).catch(function () {
      // 后端可能返回 500 但动作已生效：2 秒后查 /api/state 判断是否在主循环
      setTimeout(function () { checkBootState(); }, 2000);
    });
  }

  function clearInputs() {
    var btn = $('#btnClearInputs');
    if (btn) btn.disabled = true;
    noteSent('clear_inputs');
    sendCmd('clear_inputs').then(function (res) {
      if (res && res.inputs) {
        S.inputsOverride = normalizeInputs(res.inputs);
        if (S.model) { S.model.inputs = S.inputsOverride; applyInputs(S.model); }
      }
      S.held = {};
      for (var i = 0; i < KEY_DEFS.length; i++) setKeyVisual(KEY_DEFS[i].key, false);
      showToast('已清除虚拟输入');
      kickPoll(120);
    }).catch(function (err) {
      showToast('清除输入失败：' + (err.message || err));
    }).then(function () {
      if (btn) btn.disabled = false;
    });
  }

  function toggleScreenOrient() {
    S.screenRotate = !S.screenRotate;
    var btn = $('#btnScreenOrient');
    if (btn) btn.classList.toggle('active', S.screenRotate);
    if (S.screen) drawScreen(S.screen);
    showToast(S.screenRotate ? 'LCD 面板方向：rotate90+mirror（与 emu/main.py --screen 一致）' : 'LCD 面板方向：原始 page/column（未旋转）');
  }

  function toggleBp(addr) {
    addr = addr >>> 0;
    var has = S.bps.indexOf(addr) >= 0;
    var next = S.bps.slice();
    if (has) next = next.filter(function (a) { return a !== addr; });
    else next.push(addr);
    S.bps = next.sort(function (a, b) { return a - b; });
    renderDisasmPanel();
    sendCmd(has ? 'bp_del' : 'bp_add', { addr: addr, addr_hex: hex0x(addr) }).catch(function (err) {
      // 回滚
      if (has) S.bps.push(addr); else S.bps = S.bps.filter(function (a) { return a !== addr; });
      S.bps.sort(function (a, b) { return a - b; });
      renderDisasmPanel();
      showToast('断点操作失败：' + (err.message || err));
    });
  }

  function doGoto() {
    var input = $('#gotoAddr');
    if (!input) return;
    var addr = parseAddr(input.value);
    if (addr == null) { showToast('地址格式错误，例如 0x080030D4'); return; }
    S.followPc = false;
    updateFollowUi();
    S.center = addr;
    S.selected = addr;
    S.disasmPc = null;
    refreshDisasm(addr, true);
    jumpMem(addr);
  }

  function memJumpFromInput() {
    var input = $('#memAddrInput');
    var addr = parseAddr(input ? input.value : '');
    if (addr == null) { showToast('内存地址格式错误'); return; }
    jumpMem(addr);
  }

  /* ---------- 虚拟输入（电量滑块 / 按键 / 保存画面） ---------- */
  function setKeyVisual(key, down) {
    var btns = document.querySelectorAll('#keyPad .key-btn');
    for (var i = 0; i < btns.length; i++) {
      if (btns[i].getAttribute('data-key') === key) btns[i].classList.toggle('down', !!down);
    }
  }

  function applyInputs(model) {
    var inputs = model ? model.inputs : null;
    var src = $('#inputsSource');
    if (src) src.textContent = inputs ? '后端状态' : '后端未提供';
    var status = $('#inputStatus');
    if (status) status.innerHTML = renderInputStatus(model, S.lastSent);
    if (!inputs) return;
    var bv = $('#batteryVal');
    if (bv && inputs.batteryMv != null) bv.textContent = formatBattery(inputs.batteryMv);
    var slider = $('#batterySlider');
    if (slider && inputs.batteryMv != null && document.activeElement !== slider) {
      slider.value = String(inputs.batteryMv);
    }
    if (inputs.screenSave) showSaveResult(inputs.screenSave);
    var btns = document.querySelectorAll('#keyPad .key-btn');
    for (var i = 0; i < btns.length; i++) {
      var key = btns[i].getAttribute('data-key');
      var backendDown = !!(inputs.keys && inputs.keys[key]);
      btns[i].classList.toggle('down', !!(S.held[key] || backendDown));
    }
  }

  function applyRfPanel(model) {
    var el = $('#rfStatus');
    if (el) el.innerHTML = renderRfPanel(model);
    var irqEl = $('#rfIrq');
    if (irqEl) {
      var line = formatIrqLine(model);
      irqEl.textContent = line ? line : 'IRQ 计数未提供';
    }
    var rf = model && model.rf;
    var src = $('#rfSource');
    if (src) {
      src.textContent = rf ? (rf.present === false ? '不可用' : '虚拟 CC2500') :
        (model && model.irqCounts && model.irqCounts.items.length ? '仅 IRQ 计数' : '未提供');
    }
  }

  function mergeRfResult(res) {
    if (!res) return;
    if (S.model) {
      if (res.rf) S.model.rf = normalizeRf(res.rf);
      if (res.irqCounts) S.model.irqCounts = normalizeIrqCounts(res.irqCounts);
      if (res.irq_counts) S.model.irqCounts = normalizeIrqCounts(res.irq_counts);
      if (res.rf || res.irqCounts || res.irq_counts) applyRfPanel(S.model);
    }
  }

  function checkRfState() {
    return apiJson('/api/state').then(function (raw) {
      var model = normalizeState(raw);
      S.model = model;
      S.connected = true;
      updateConnectionUi();
      applyModel(model);
      if (model.rf && model.rf.state === 'RX') {
        showToast('射频已就绪：state=RX · RX ' + model.rf.rxCount + ' 包');
      } else {
        showToast('射频初始化未确认（rf=' + (model.rf ? model.rf.state : '无 rf 字段') + '）');
      }
      schedulePoll();
      return model;
    }).catch(function (err) {
      showToast('RF 状态查询失败：' + (err.message || err));
      return null;
    });
  }

  function doRfInit() {
    var btn = $('#btnRfInit');
    if (btn) btn.disabled = true;
    noteSent('rf_init');
    showToast('正在调用固件 RF 初始化…');
    sendCmd('rf_init').then(function (res) {
      mergeRfResult(res);
      var state = res && res.rf ? normalizeRf(res.rf).state : null;
      showToast('射频初始化完成' + (state ? '：state=' + state : ''));
      kickPoll(150);
    }).catch(function () {
      // 后端可能暂无 rf_init：1.5s 后查 /api/state 看 rf.state 是否 RX
      setTimeout(function () { checkRfState(); }, 1500);
    }).then(function () {
      if (btn) btn.disabled = false;
    });
  }

  function doRfInject(hex) {
    var norm = normalizeRfHex(hex);
    if (norm == null) {
      showToast('RF hex 格式错误：请输入偶数字节，如 A9 50 B4 02');
      return;
    }
    noteSent('rf_rx ' + norm);
    sendCmd('rf_rx', { hex: norm }).then(function (res) {
      mergeRfResult(res);
      var rf = res && res.rf ? normalizeRf(res.rf) : null;
      showToast('已注入 RF 包 ' + norm + (rf ? '（RX ' + rf.rxCount + '）' : ''));
      kickPoll(120);
    }).catch(function (err) {
      showToast('RF 注入失败：' + (err.message || err));
    });
  }

  function doRfStatus() {
    var btn = $('#btnRfRefresh');
    if (btn) btn.disabled = true;
    sendCmd('rf_status').then(function (res) {
      mergeRfResult(res);
      showToast('RF：' + formatRfStatus({ rf: normalizeRf(res && res.rf) }));
      kickPoll(120);
    }).catch(function (err) {
      showToast('rf_status 失败：' + (err.message || err));
    }).then(function () {
      if (btn) btn.disabled = false;
    });
  }

  function noteSent(text) {
    S.lastSent = text;
    var status = $('#inputStatus');
    if (status) status.innerHTML = renderInputStatus(S.model || {}, S.lastSent);
  }

  function sendBattery(mv) {
    mv = clampInt(toInt(mv) == null ? 3700 : toInt(mv), BATTERY_MIN_MV, BATTERY_MAX_MV);
    S.lastBatteryMv = mv;
    noteSent('battery ' + mv + ' mV');
    sendCmd('battery', { mv: mv }).then(function () { kickPoll(150); })
      .catch(function (err) { showToast('电量注入失败：' + (err.message || err)); });
  }

  function holdKey(key) {
    if (!key || S.held[key]) return;
    S.held[key] = true;
    setKeyVisual(key, true);
    noteSent('key ' + key + ' down');
    sendCmd('key', { key: key, down: true }).then(function () {
      kickPoll(120);
    }).catch(function (err) {
      S.held[key] = false;
      setKeyVisual(key, false);
      showToast('按键注入失败：' + (err.message || err));
    });
  }

  function releaseAllKeys() {
    var keys = Object.keys(S.held);
    var released = false;
    for (var i = 0; i < keys.length; i++) {
      var key = keys[i];
      if (!S.held[key]) continue;
      S.held[key] = false;
      released = true;
      setKeyVisual(key, false);
      noteSent('key_up ' + key);
      sendCmd('key_up', { key: key }).catch(function (err) {
        showToast('key_up 失败：' + (err.message || err));
      });
    }
    if (released) kickPoll(100);
  }

  function pulseKey(key) {
    if (!key) return;
    setKeyVisual(key, true);
    noteSent('key_press ' + key + ' 80ms');
    sendCmd('key_press', { key: key, ms: 80 }).then(function () {
      kickPoll(160);
    }).catch(function (err) {
      setKeyVisual(key, false);
      showToast('脉冲注入失败：' + (err.message || err));
    });
    setTimeout(function () { if (!S.held[key]) setKeyVisual(key, false); }, 240);
  }

  function showSaveResult(text) {
    var el = $('#lcdSaveResult');
    if (!el) return;
    S.savePath = text;
    el.textContent = text;
    el.hidden = false;
  }

  function saveScreenImage() {
    var btn = $('#btnScreenSave');
    if (btn) btn.disabled = true;
    noteSent('screen_save');
    sendCmd('screen_save').then(function (res) {
      var path = res && (res.path || res.file || res.saved || res.output || res.screenshot || res.png);
      if (path) {
        showSaveResult('已保存：' + path);
        showToast('LCD 画面已保存：' + path);
      } else {
        showSaveResult('已请求保存（后端未返回路径）');
        showToast('已请求保存 LCD 画面');
      }
    }).catch(function (err) {
      showToast('保存画面失败：' + (err.message || err) + '（后端可能尚未实现 screen_save）');
    }).then(function () {
      if (btn) btn.disabled = false;
      kickPoll(200);
    });
  }

  function bindInputs() {
    var slider = $('#batterySlider');
    if (slider) {
      slider.addEventListener('input', function () {
        var mv = clampInt(toInt(slider.value) == null ? 3700 : toInt(slider.value), BATTERY_MIN_MV, BATTERY_MAX_MV);
        var bv = $('#batteryVal');
        if (bv) bv.textContent = formatBattery(mv);
        if (S.batteryTimer) clearTimeout(S.batteryTimer);
        S.batteryTimer = setTimeout(function () { sendBattery(mv); }, 180);
      });
      slider.addEventListener('change', function () {
        if (S.batteryTimer) { clearTimeout(S.batteryTimer); S.batteryTimer = null; }
        sendBattery(slider.value);
      });
    }
    var keyPad = $('#keyPad');
    if (keyPad) {
      keyPad.addEventListener('pointerdown', function (e) {
        var btn = e.target && e.target.closest ? e.target.closest('.key-btn') : null;
        if (!btn) return;
        e.preventDefault();
        var key = btn.getAttribute('data-key');
        if ($('#pulseMode') && $('#pulseMode').checked) pulseKey(key);
        else holdKey(key);
      });
      // 老浏览器不支持 PointerEvent 时退回 mousedown
      keyPad.addEventListener('mousedown', function (e) {
        if (typeof PointerEvent !== 'undefined') return;
        var btn = e.target && e.target.closest ? e.target.closest('.key-btn') : null;
        if (!btn) return;
        e.preventDefault();
        var key = btn.getAttribute('data-key');
        if ($('#pulseMode') && $('#pulseMode').checked) pulseKey(key);
        else holdKey(key);
      });
    }
    document.addEventListener('pointerup', releaseAllKeys, true);
    document.addEventListener('pointercancel', releaseAllKeys, true);
    window.addEventListener('blur', releaseAllKeys);
    var saveBtn = $('#btnScreenSave');
    if (saveBtn) saveBtn.addEventListener('click', saveScreenImage);
    var clearBtn = $('#btnClearInputs');
    if (clearBtn) clearBtn.addEventListener('click', clearInputs);
    var orientBtn = $('#btnScreenOrient');
    if (orientBtn) orientBtn.addEventListener('click', toggleScreenOrient);
    var rfInitBtn = $('#btnRfInit');
    if (rfInitBtn) rfInitBtn.addEventListener('click', doRfInit);
    var rfPairBtn = $('#btnRfPair');
    if (rfPairBtn) rfPairBtn.addEventListener('click', function () { doRfInject('A9 50 B4 02'); });
    var rfInjectBtn = $('#btnRfInject');
    if (rfInjectBtn) rfInjectBtn.addEventListener('click', function () {
      doRfInject($('#rfHexInput') ? $('#rfHexInput').value : '');
    });
    var rfHexInput = $('#rfHexInput');
    if (rfHexInput) rfHexInput.addEventListener('keydown', function (e) {
      if (e.key === 'Enter') doRfInject(this.value);
    });
    var rfRefreshBtn = $('#btnRfRefresh');
    if (rfRefreshBtn) rfRefreshBtn.addEventListener('click', doRfStatus);
  }

  /* ---------- 事件绑定 ---------- */
  function bindEvents() {
    $('#btnBoot').addEventListener('click', doBootInteractive);
    $('#btnRun').addEventListener('click', doRunPause);
    $('#btnPause').addEventListener('click', doRunPause);
    $('#btnStep').addEventListener('click', doStep);
    $('#btnStepOver').addEventListener('click', doStepOver);
    $('#btnReset').addEventListener('click', doReset);
    $('#btnGoto').addEventListener('click', doGoto);
    $('#gotoAddr').addEventListener('keydown', function (e) { if (e.key === 'Enter') doGoto(); });
    $('#chkFollow').addEventListener('change', function () {
      S.followPc = this.checked;
      if (S.followPc && S.model) {
        S.center = S.model.pc & ~1;
        S.disasmPc = null;
        refreshDisasm(S.center, true);
      }
      updateFollowUi();
    });
    $('#btnFollowPc').addEventListener('click', function () {
      S.followPc = true;
      updateFollowUi();
      if (S.model) { S.center = S.model.pc & ~1; S.disasmPc = null; refreshDisasm(S.center, true); }
      else showToast('尚未连接模拟器');
    });
    $('#btnDisasmUp').addEventListener('click', function () { scrollDisasm(-1); });
    $('#btnDisasmDown').addEventListener('click', function () { scrollDisasm(1); });
    $('#disasmBody').addEventListener('click', function (e) {
      var bpBtn = e.target.closest('.d-bp');
      if (bpBtn) {
        e.stopPropagation();
        toggleBp(toAddr(bpBtn.getAttribute('data-addr')));
        return;
      }
      var row = e.target.closest('.d-row');
      if (row) {
        var addr = toAddr(row.getAttribute('data-addr'));
        S.selected = addr;
        var rows = $('#disasmBody').querySelectorAll('.d-row.selected');
        for (var i = 0; i < rows.length; i++) rows[i].classList.remove('selected');
        row.classList.add('selected');
        jumpMem(addr);
      }
    });
    $('#disasmBody').addEventListener('wheel', function (e) {
      if (Math.abs(e.deltaY) < 4) return;
      e.preventDefault();
      scrollDisasm(e.deltaY > 0 ? 1 : -1);
    }, { passive: false });
    $('#logFilter').addEventListener('input', renderPeripheralPanel);
    $('#periphList').addEventListener('scroll', function () {
      var el = this;
      var atBottom = el.scrollTop + el.clientHeight >= el.scrollHeight - 8;
      el.dataset.follow = atBottom ? '1' : '0';
    });
    $('#btnMemGo').addEventListener('click', memJumpFromInput);
    $('#memAddrInput').addEventListener('keydown', function (e) { if (e.key === 'Enter') memJumpFromInput(); });
    $('#btnMemPrev').addEventListener('click', function () { jumpMem((S.memAddr - MEM_LEN) >>> 0); });
    $('#btnMemNext').addEventListener('click', function () { jumpMem((S.memAddr + MEM_LEN) >>> 0); });
    var quick = document.querySelectorAll('[data-mem-quick]');
    for (var q = 0; q < quick.length; q++) {
      quick[q].addEventListener('click', function () {
        var kind = this.getAttribute('data-mem-quick');
        var target = S.memAddr;
        if (kind === 'pc') target = S.model ? S.model.pc : (S.disasm ? S.disasm.addr : 0x080030D4);
        else if (kind === 'sp') target = S.model ? S.model.sp : 0x20000CD0;
        else if (kind === 'ram') target = 0x20000000;
        else if (kind === 'lcd') target = 0x20001000;
        else if (kind === 'uart') target = 0x20000400;
        jumpMem(target);
      });
    }
    $('#btnRetry').addEventListener('click', function () {
      hideBanner();
      S.failCount = 0;
      kickPoll(0);
    });
    $('#btnSwitchMock').addEventListener('click', function () {
      if (!root.AD100Mock) { showToast('mock.js 未加载'); return; }
      S.mock = true;
      S.failCount = 0;
      hideBanner();
      updateConnectionUi();
      kickPoll(0);
      refreshFunctions();
      refreshScreen();
      showToast('已切换到 MOCK 演示数据');
    });
    document.addEventListener('keydown', function (e) {
      var tag = (e.target && e.target.tagName) || '';
      var typing = tag === 'INPUT' || tag === 'TEXTAREA' || tag === 'SELECT';
      if (e.key === 'F5') { e.preventDefault(); doRunPause(); }
      else if (e.key === 'F10' && !e.altKey) { e.preventDefault(); doStep(); }
      else if (e.key === 'F8') { e.preventDefault(); doReset(); }
      else if ((e.ctrlKey || e.metaKey) && (e.key === 'g' || e.key === 'G')) {
        e.preventDefault();
        var input = $('#gotoAddr');
        if (input) { input.focus(); input.select(); }
      } else if (e.key === 'Escape' && typing) {
        e.target.blur();
      }
    });
    document.addEventListener('visibilitychange', function () {
      if (!document.hidden) kickPoll(0);
      else schedulePoll();
    });
    window.addEventListener('beforeunload', function () {
      if (S.pollTimer) clearTimeout(S.pollTimer);
      if (S.screenTimer) clearInterval(S.screenTimer);
    });
  }

  function renderInitial() {
    $('#regGrid').innerHTML = renderRegisterRows({
      regs: [{ name: 'r0', value: 0 }, { name: 'r1', value: 0 }, { name: 'r2', value: 0 }, { name: 'r3', value: 0 },
        { name: 'r4', value: 0 }, { name: 'r5', value: 0 }, { name: 'r6', value: 0 }, { name: 'r7', value: 0 },
        { name: 'r8', value: 0 }, { name: 'r9', value: 0 }, { name: 'r10', value: 0 }, { name: 'r11', value: 0 },
        { name: 'r12', value: 0 }, { name: 'sp', value: 0 }, { name: 'lr', value: 0 }, { name: 'pc', value: 0 }]
    });
    $('#flagRow').innerHTML = renderFlags(null);
    $('#funcInfo').innerHTML = renderFunctionInfo(null, {});
    $('#disasmBody').innerHTML = '<div class="empty">正在连接模拟器，等待状态数据…</div>';
    $('#memTable').innerHTML = renderMemoryTable(new Array(MEM_LEN).fill(0), S.memAddr, null);
    $('#uartConsole').innerHTML = '<div class="empty">UART 无输出</div>';
    $('#callstack').innerHTML = '<div class="empty">模拟器未提供调用栈（可选扩展，见 API.md）</div>';
    $('#periphList').innerHTML = '<div class="empty">暂无外设访问记录</div>';
    $('#inputStatus').innerHTML = '<div class="io-status-line">等待模拟器 inputs 状态…</div>';
    $('#inputsSource').textContent = '—';
    $('#batteryVal').textContent = formatBattery(3700);
    $('#phaseInfo').textContent = '—';
    $('#btnScreenOrient').classList.toggle('active', S.screenRotate);
    $('#rfStatus').innerHTML = '<div class="rf-line">等待模拟器 rf 状态…</div>';
    $('#rfIrq').textContent = 'IRQ 计数未提供';
    $('#rfSource').textContent = '—';
    showScreenPlaceholder('LCD 屏幕未连接（等待 /api/screen 数据）');
    updateRunButtons();
    updateConnectionUi();
    $('#modeBadge').textContent = S.mock ? 'MOCK' : 'LIVE';
    $('#modeBadge').className = 'mode-badge ' + (S.mock ? 'mock' : 'live');
  }

  function boot() {
    if (S.booted) return;
    S.booted = true;
    readConfig();
    renderInitial();
    bindEvents();
    bindInputs();
    updateFollowUi();
    refreshFunctions();
    pollState();
    refreshMem();
    refreshScreen();
    S.screenTimer = setInterval(function () {
      refreshScreen();
      // 粗略统计屏幕刷新率（轮询间隔 1.5s）
      var fps = S.screenFrames / (SCREEN_POLL / 1000);
      S.screenFrames = 0;
      var fpsEl = $('#lcdFps');
      if (fpsEl && S.screen) fpsEl.textContent = fps.toFixed(1) + ' fps';
    }, SCREEN_POLL);
  }

  /* ============================== 导出（供 selftest.js） ============================== */
  var APP = {
    version: '1.0.0',
    // 纯函数
    hex2: hex2, hex8: hex8, hex32: hex32, hex0x: hex0x,
    escapeHtml: escapeHtml, toAddr: toAddr, toInt: toInt, parseAddr: parseAddr,
    align: align, formatCount: formatCount, formatIpc: formatIpc,
    computeIpc: computeIpc, pickPollInterval: pickPollInterval,
    normalizeState: normalizeState, normalizeDisasm: normalizeDisasm, normalizeMem: normalizeMem,
    normalizeScreen: normalizeScreen, normalizeBreakpoints: normalizeBreakpoints,
    normalizeUart: normalizeUart, normalizePeripheralLog: normalizePeripheralLog,
    parseLogLine: parseLogLine, screenPixelAt: screenPixelAt,
    screenBytesNeeded: screenBytesNeeded, asciiOf: asciiOf,
    renderRegisterRows: renderRegisterRows, renderFlags: renderFlags,
    renderDisasmRows: renderDisasmRows, renderMemoryTable: renderMemoryTable,
    renderPeripheralRows: renderPeripheralRows, renderConsole: renderConsole,
    renderCallstack: renderCallstack, renderFunctionInfo: renderFunctionInfo,
    renderScreenPlaceholder: renderScreenPlaceholder, formatScreenMeta: formatScreenMeta,
    formatLcdMeta: formatLcdMeta, normalizeLcd: normalizeLcd, normalizeFaults: normalizeFaults,
    lookupFunction: lookupFunction, nextFunction: nextFunction,
    heuristicFunction: heuristicFunction, mergeBreakpoints: mergeBreakpoints,
    mnemClass: mnemClass,
    // 虚拟输入（纯函数）
    batteryMvToPct: batteryMvToPct, formatBattery: formatBattery,
    normalizeInputs: normalizeInputs, renderInputStatus: renderInputStatus,
    batteryCmd: batteryCmd, keyDownCmd: keyDownCmd, keyUpCmd: keyUpCmd,
    keyPressCmd: keyPressCmd, screenSaveCmd: screenSaveCmd, KEY_DEFS: KEY_DEFS,
    clearInputsCmd: clearInputsCmd, bootInteractiveCmd: bootInteractiveCmd,
    isMainLoopPc: isMainLoopPc, mapTaskPhase: mapTaskPhase, formatPhaseInfo: formatPhaseInfo,
    // 2.4G 射频（纯函数）
    rfInitCmd: rfInitCmd, rfRxCmd: rfRxCmd, rfStatusCmd: rfStatusCmd,
    normalizeRfHex: normalizeRfHex, normalizeRf: normalizeRf, formatRfStatus: formatRfStatus,
    irqLabel: irqLabel, normalizeIrqCounts: normalizeIrqCounts, normalizeIrqLog: normalizeIrqLog,
    formatIrqLine: formatIrqLine, formatIrqLogEntry: formatIrqLogEntry, renderRfPanel: renderRfPanel,
    // 状态（只读调试用）
    state: S,
    // 启动
    boot: boot
  };
  root.AD100App = APP;

  if (typeof module !== 'undefined' && module.exports) module.exports = APP;

  if (typeof document !== 'undefined') {
    if (document.readyState === 'loading') {
      document.addEventListener('DOMContentLoaded', boot);
    } else {
      boot();
    }
  }
})(typeof globalThis !== 'undefined' ? globalThis : this);
