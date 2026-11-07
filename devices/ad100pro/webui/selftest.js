/*!
 * selftest.js — webui 的最小冒烟自测（不依赖 jsdom / 浏览器）
 *
 * 运行：node webui/selftest.js
 * 覆盖：
 *   - mock.js 假数据后端（/api/state、/api/disasm、/api/mem、/api/screen、
 *     /api/functions、/api/cmd）以及运行/单步/断点语义；
 *   - app.js 的纯函数：字段兼容层、断点/反汇编/寄存器/内存/日志/调用栈渲染；
 *   - index.html 与 app.js 的 DOM id 对齐、无任何外部 CDN 依赖；
 *   - style.css 无外部资源引用。
 */
'use strict';

const assert = require('assert');
const fs = require('fs');
const path = require('path');

const mock = require('./mock.js');
const app = require('./app.js');
const webuiDir = __dirname;

let passed = 0;
const failures = [];

function ok(v, msg) { assert.ok(v, msg || 'expected truthy'); }
function eq(a, b, msg) { assert.strictEqual(a, b, msg || ('expected ' + JSON.stringify(b) + ', got ' + JSON.stringify(a))); }
function includes(hay, needle, msg) {
  assert.ok(String(hay).indexOf(needle) >= 0, msg || ('expected to include ' + JSON.stringify(needle)));
}

async function test(name, fn) {
  try {
    await fn();
    passed++;
    console.log('PASS  ' + name);
  } catch (e) {
    failures.push(name + ' :: ' + (e && e.message ? e.message : e));
    console.log('FAIL  ' + name + '  ->  ' + (e && e.message ? e.message : e));
  }
}

async function main() {
  console.log('=== AD100 Pro webui 自测（node ' + process.version + '）===');
  console.log('mock.js imageBase = 0x' + mock.imageBase.toString(16).toUpperCase() +
    ', LCD = ' + mock.lcd.w + 'x' + mock.lcd.h + ' @0x' + mock.lcd.addr.toString(16).toUpperCase());
  console.log('');

  /* ---------------- mock.js ---------------- */
  await test('mock 状态字段完整（pc/sp/lr/xpsr/regs[16]/insnCount/breakpoints/log/uart）', () => {
    const s = mock.state();
    eq(s.pc, 0x080030D4, 'pc 应为 Reset_Handler 0x080030D4');
    eq(s.regs.length, 16);
    const count = s.insnCount;
    ok(Number.isInteger(count));
    ok(Array.isArray(s.breakpoints) && s.breakpoints.length >= 1);
    ok(Array.isArray(s.log) && s.log.length > 20);
    includes(s.uart, 'AD100');
    eq(s.func.name, 'Reset_Handler');
    ok(s.nearestFunc && s.nearestFunc.addr > s.pc);
  });

  await test('mock 反汇编窗口（PC ±30）命中 PC 行并含真实字节', () => {
    const d = mock.disasm(0x080030D4, 61);
    eq(d.addr, 0x080030D4);
    eq(d.lines.length, 61);
    const pcLine = d.lines.find(l => l.addr === 0x080030D4);
    ok(pcLine, '窗口应包含 PC 行');
    eq(pcLine.bytes, '0448');
    eq(pcLine.mnem, 'ldr');
    includes(pcLine.op, '[pc');
    const first = d.lines[0].addr;
    const last = d.lines[d.lines.length - 1].addr;
    ok(first < 0x080030D4 && last > 0x080030D4, 'PC 应位于窗口中间');
  });

  await test('mock 反汇编夹具含向量表 .word 与 main 区域', () => {
    const vec = mock.disasm(0x08003004, 5);
    includes(vec.lines.map(l => l.mnem).join(','), '.word');
    const main = mock.disasm(0x0800CF74, 9);
    ok(main.lines.some(l => l.addr === 0x0800CF74), 'main 0x0800CF74 应可反汇编');
    // 夹具范围外可确定性合成（同一地址两次一致）
    const a = mock.disasm(0x08005000, 5);
    const b = mock.disasm(0x08005000, 5);
    eq(a.lines.map(l => l.addr + ':' + l.bytes).join(','), b.lines.map(l => l.addr + ':' + l.bytes).join(','));
  });

  await test('mock 内存：UART 文本可读、栈顶对齐、Flash 字节与夹具一致', () => {
    const uartMem = mock.mem(0x20000400, 32);
    eq(uartMem.hex.length, 64);
    includes(Buffer.from(uartMem.hex, 'hex').toString('utf8'), 'AD100');
    eq(mock.mem(0x080030D4, 4).hex, '04488047');
    eq(mock.state().sp, 0x20000CD0);
  });

  await test('mock 屏幕：128x64、base64 1bpp page 格式（与 emu /api/screen 契约一致）', () => {
    const scr = mock.screen();
    eq(scr.w, 128); eq(scr.h, 64); eq(scr.format, '1bpp');
    ok(typeof scr.data === 'string' && scr.data.length > 100, 'data 应为 base64 字符串');
    const bytes = Buffer.from(scr.data, 'base64');
    eq(bytes.length, 128 * Math.ceil(64 / 8));
    let ones = 0;
    for (const b of bytes) for (let i = 0; i < 8; i++) if (b & (1 << i)) ones++;
    ok(ones > 100, '1bpp 位图应有较亮像素');
    includes(JSON.stringify(scr.info), '"page":0');
  });

  await test('mock /api/functions 函数表含 Reset_Handler 与 main', () => {
    const funcs = mock.functions();
    ok(funcs.length > 100);
    ok(funcs.some(f => f.addr === 0x080030D4 && f.name === 'Reset_Handler'));
    ok(funcs.some(f => f.addr === 0x0800CF74 && f.name === 'main'));
  });

  await test('mock 命令：step / step_over / 断点 / reset / run 语义', () => {
    mock.cmd({ cmd: 'reset' });
    let s = mock.state();
    eq(s.pc, 0x080030D4);
    eq(s.insnCount, 0);
    eq(s.running, false);

    mock.cmd({ cmd: 'step' });
    s = mock.state();
    eq(s.pc, 0x080030D6);
    eq(s.insnCount, 1);

    mock.cmd({ cmd: 'set_pc', addr: 0x080030C4 });   // bl 指令
    mock.cmd({ cmd: 'step_over' });
    s = mock.state();
    eq(s.pc, 0x080030C8, 'step_over 应跳过 bl 落到下一条');

    const bpsBefore = mock.state().breakpoints.length;
    mock.cmd({ cmd: 'bp_add', addr: '0x08003188' });
    ok(mock.state().breakpoints.indexOf(0x08003188) >= 0);
    mock.cmd({ cmd: 'bp_del', addr: 0x08003188 });
    eq(mock.state().breakpoints.length, bpsBefore);

    mock.cmd({ cmd: 'run' });
    ok(mock.state().running);
    mock.cmd({ cmd: 'pause' });
    eq(mock.state().running, false);
    mock.cmd({ cmd: 'reset' });   // 恢复初始状态，避免影响后续用例
  });

  await test('mock HTTP 外观：GET /api/state、/api/disasm、/api/mem、/api/screen、/api/functions', async () => {
    const rs = await mock.fetch('/api/state');
    eq(rs.status, 200);
    const s = await rs.json();
    eq(s.pc, 0x080030D4);

    const rd = await mock.fetch('/api/disasm?addr=0x080030D4&count=11');
    const d = await rd.json();
    eq(d.lines.length, 11);
    eq(d.lines[5].addr, 0x080030D4);

    const rm = await mock.fetch('/api/mem?addr=0x20000400&len=16');
    const m = await rm.json();
    eq(m.hex.length, 32);

    const rsc = await mock.fetch('/api/screen');
    eq((await rsc.json()).w, 128);

    const rf = await mock.fetch('/api/functions');
    ok((await rf.json()).functions.length > 100);

    const bad = await mock.fetch('/api/not-exist');
    eq(bad.status, 404);
    eq(bad.ok, false);
  });

  await test('mock POST /api/cmd（body JSON）可控制运行', async () => {
    const r = await mock.fetch('/api/cmd', { method: 'POST', body: JSON.stringify({ cmd: 'pause' }) });
    eq(r.status, 200);
    eq((await r.json()).ok, true);
    eq(mock.state().running, false);
  });

  /* ---------------- app.js 纯函数 ---------------- */
  await test('normalizeState：mock 状态 → 视图模型（16 寄存器 / PC 对齐 / 函数名）', () => {
    const s = app.normalizeState(mock.state());
    eq(s.pc, 0x080030D4);
    eq(s.sp, 0x20000CD0);
    eq(s.regs.length, 16);
    eq(s.regs[15].name, 'pc');
    eq(s.regs[15].value, 0x080030D4);
    eq(s.func.name, 'Reset_Handler');
    ok(s.hasBpsField);
    ok(Array.isArray(s.log) && s.log.length > 20);
  });

  await test('normalizeState：兼容 {PC,regs[],isRunning,instructionCount,bps,uart,xpsr} 变体', () => {
    const raw = {
      PC: '0x080030D4',
      regs: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0x20000CD0, 0x080030C9, 0x080030D4],
      isRunning: true,
      instructionCount: '1234',
      breakpoints: ['0x080030D4', 0x080030F0],
      uart: 'hello',
      xpsr: 0xF1000000,
      log: [{ dir: 'W', address: '0x40013828', register: 'USART1_TDR', value: 0x41, pc: '0x080030D4' }]
    };
    const s = app.normalizeState(raw);
    eq(s.pc, 0x080030D4);
    eq(s.sp, 0x20000CD0);
    eq(s.lr, 0x080030C9);
    eq(s.running, true);
    eq(s.insnCount, 1234);
    eq(s.breakpoints.length, 2);
    eq(s.flags.N, true); eq(s.flags.Z, true); eq(s.flags.C, true); eq(s.flags.V, true);
    eq(s.uart, 'hello');
    eq(s.regs[5].value, 6);
  });

  await test('normalizeState：flags 字符串 / 无 xpsr 推导 / 运行状态字符串', () => {
    const s = app.normalizeState({ pc: 0x1000, flags: 'nZcV', state: 'running' });
    eq(s.flags.N, true); eq(s.flags.Z, true); eq(s.flags.C, true); eq(s.flags.V, true);
    eq(s.running, true);
    ok(s.xpsr & 0x01000000, 'Thumb 位应置位');
  });

  await test('normalizeDisasm：兼容字符串行 / {addr,bytes,mnemonic,operands} / 排序去重', () => {
    const d1 = app.normalizeDisasm({
      lines: [
        { addr: '0x080030D6', bytes: '80 47', mnemonic: 'blx', operands: 'r0' },
        '080030D4  0448  ldr r0, [pc, #0x10]',
        { addr: 0x080030D6, bytes: '8047', mnem: 'blx', op: 'r0' }
      ]
    }, 0x080030D4);
    eq(d1.lines.length, 2);
    eq(d1.lines[0].addr, 0x080030D4);
    eq(d1.lines[0].bytes, '0448');
    eq(d1.lines[1].bytes, '8047');
    eq(d1.lines[1].mnem, 'blx');
  });

  await test('renderDisasmRows：PC 高亮、断点红点、行格式 "地址 字节 助记符 操作数"', () => {
    const d = mock.disasm(0x080030D4, 61);
    const lines = app.normalizeDisasm(d).lines;
    const html = app.renderDisasmRows(lines, { pc: 0x080030D4, breakpoints: [0x0800310C] });
    includes(html, 'pc-line');
    includes(html, '080030D4');
    includes(html, '0448');
    includes(html, '>ldr<');
    includes(html, 'r0, [pc, #0x10]');
    includes(html, 'has-bp');
    eq((html.match(/class="d-row/g) || []).length, 61);
    // 断点列可点击
    includes(html, 'data-action="toggle-bp"');
  });

  await test('renderRegisterRows / renderFlags / renderFunctionInfo：关键面板输出', () => {
    const m = app.normalizeState(mock.state());
    const regs = app.renderRegisterRows(m);
    includes(regs, 'reg-name">pc<');
    includes(regs, '080030D4');
    includes(regs, 'reg-name">sp<');
    includes(regs, '20000CD0');
    eq((regs.match(/class="reg-row/g) || []).length, 16);
    const flags = app.renderFlags(m);
    ['N', 'Z', 'C', 'V'].forEach(f => includes(flags, 'data-flag="' + f + '"'));
    const fi = app.renderFunctionInfo(m, {});
    includes(fi, 'Reset_Handler');
    includes(fi, '最近函数');
  });

  await test('lookupFunction / nextFunction：函数表解析（Reset_Handler / main）', () => {
    const funcs = mock.functions();
    eq(app.lookupFunction(funcs, 0x080030D4).name, 'Reset_Handler');
    eq(app.lookupFunction(funcs, 0x0800CF74).name, 'main');
    eq(app.nextFunction(funcs, 0x080030D4).name, 'func_080030D8');
    // 非函数表数据也走启发式
    const lines = app.normalizeDisasm(mock.disasm(0x080030D4, 61)).lines;
    const heur = app.heuristicFunction(lines, 0x080030D4);
    ok(heur && heur.heuristic && /^sub_/.test(heur.name));
  });

  await test('normalizeMem / renderMemoryTable：hex + ASCII（含 "AD100"）与 PC 行高亮', () => {
    const raw = mock.mem(0x20000400, 256);
    const mem = app.normalizeMem(raw, 0x20000400, 256);
    eq(mem.bytes.length, 256);
    const html = app.renderMemoryTable(mem.bytes, mem.addr, 0x20000410);
    includes(html, 'mem-hex');
    includes(html, 'mem-ascii');
    includes(html, '00:00.000');   // UART 缓冲区文本进入 ASCII 列
    includes(html, 'AD1');
    includes(html, 'pc-row');
    eq((html.match(/class="mem-row/g) || []).length, 17); // 1 表头 + 16 行
    // 兼容带空格的 hex 字符串
    const m2 = app.normalizeMem({ addr: 0x1000, hex: 'DE AD BE EF' }, 0x1000, 4);
    eq(m2.bytes.join(','), '222,173,190,239');
  });

  await test('外设访问日志：过滤（寄存器/地址）与事件行', () => {
    const m = app.normalizeState(mock.state());
    const all = app.renderPeripheralRows(m.log, '');
    eq((all.match(/class="plog-row/g) || []).length > 10, true);
    const rcc = app.renderPeripheralRows(m.log, 'RCC');
    ok(rcc.indexOf('RCC') >= 0);
    const addr = app.renderPeripheralRows(m.log, '400138');
    ok(addr.indexOf('USART') >= 0);
    const none = app.renderPeripheralRows(m.log, 'zzzz');
    includes(none, '暂无外设访问记录');
    const evt = app.renderPeripheralRows([{ dir: '!', reg: 'RESET', note: '外设复位' }], '');
    includes(evt, 'plog-event');
  });

  await test('normalizeUart：显式字符串 / 数组 / 从 USART1_TDR 写日志推导', () => {
    eq(app.normalizeUart({ uart: 'abc' }, []), 'abc');
    eq(app.normalizeUart({ uart: ['a', { text: 'b' }] }, []), 'a\nb');
    const derived = app.normalizeUart({}, [
      { dir: 'W', addr: 0x40013828, reg: 'USART1_TDR', value: 0x41 },
      { dir: 'W', addr: 0x40013828, reg: 'USART1_TDR', value: 0x42 },
      { dir: 'W', addr: 0x40013828, reg: 'USART1_TDR', value: 0x0A }
    ]);
    eq(derived, 'AB\n');
  });

  await test('renderConsole / renderCallstack：UART 文本与调用栈', () => {
    const m = app.normalizeState(mock.state());
    const uart = app.renderConsole(m.uart);
    includes(uart, 'AD100 Pro');
    includes(uart, 'main @0x0800CF74');
    const stack = app.renderCallstack(m.callstack);
    includes(stack, '#0');
    includes(stack, 'Reset_Handler');
    includes(stack, 'main');
    includes(app.renderCallstack([]), '未提供调用栈');
  });

  await test('normalizeState：lcd{w,h,format,frames} 与 faults 字段', () => {
    const s = app.normalizeState({
      pc: 0x080030D4,
      lcd: { w: 320, h: 240, format: '1bpp', frames: 1234, addr: '0x20001000' },
      faults: [{ type: 'HardFault', pc: '0x080030D4', lr: '0x08003101' }]
    });
    eq(s.lcd.w, 320); eq(s.lcd.h, 240); eq(s.lcd.format, '1bpp');
    eq(s.lcd.frames, 1234); eq(s.lcd.addr, 0x20001000);
    eq(s.faults.length, 1);
    eq(s.faults[0].type, 'HardFault');
    includes(app.formatLcdMeta(s.lcd), '320×240');
    includes(app.formatLcdMeta(s.lcd), '1,234 帧');
    // faults 也兼容数字 / 字符串数组
    eq(app.normalizeFaults(3).length, 3);
    eq(app.normalizeFaults('watchdog timeout').length, 1);
    eq(app.normalizeFaults(null).length, 0);
    // lcd 扩展字段 + lastScreenSave
    const s2 = app.normalizeState({
      pc: 0x080030D4,
      lcd: { w: 128, h: 64, format: '1bpp', frames: 5, nonzero: 290, addr: 0x200004CC },
      lastScreenSave: 'out/live_after.png'
    });
    eq(s2.lcd.nonzero, 290);
    eq(s2.lcd.addr, 0x200004CC);
    eq(s2.lastScreenSave, 'out/live_after.png');
    includes(app.formatLcdMeta(s2.lcd), '非零 290B');
  });

  await test('normalizeScreen：page/column 1bpp / gray8 / 缺数据', () => {
    const scr = app.normalizeScreen(mock.screen());
    eq(scr.w, 128); eq(scr.h, 64);
    eq(scr.format, '1bpp');
    eq(scr.layout, 'page');
    eq(scr.data.length, 128 * 8);
    eq(scr.addr, 0x20001000);
    // page/column 解码：byte0 bit0 => (0,0) 亮；(0,1) 灭
    const page = app.normalizeScreen({ w: 8, h: 8, format: '1bpp', data: [0x01, 0, 0, 0, 0, 0, 0, 0], info: { page: 0, col: 0 } });
    eq(page.layout, 'page');
    eq(app.screenPixelAt(page, 0, 0), 255);
    eq(app.screenPixelAt(page, 0, 1), 0);
    const mono = app.normalizeScreen({ w: 8, h: 1, format: '1bpp', data: [0xAA] });
    eq(mono.data.length, 1);
    eq(mono.layout, 'linear');
    const gray = app.normalizeScreen({ w: 2, h: 2, format: 'gray8', data: [0, 16, 32, 48] });
    eq(gray.data.length, 4);
    eq(app.normalizeScreen({ w: 0, h: 0 }), null);
    eq(app.normalizeScreen(null), null);
    const html = app.renderScreenPlaceholder('LCD 未连接');
    includes(html, 'LCD 未连接');
    includes(html, '/api/screen');
    includes(app.formatScreenMeta(scr), '128×64');
    includes(app.formatScreenMeta(scr), '/page');
  });

  await test('parseLogLine：解析 emu/server.py 的 MMIO 字符串日志', () => {
    const e = app.parseLogLine('[  3145930] 0800A4BC  GPIOB       BSRR         W 0x00000040  sz=4  func_08007426@08007426', 3);
    eq(e.dir, 'W');
    eq(e.reg, 'GPIOB.BSRR');
    eq(e.pc, 0x0800A4BC);
    eq(e.value, 0x40);
    eq(e.time, 3145930);
    includes(e.note, 'func_08007426');
    const entry = app.normalizePeripheralLog([
      '[  3145930] 0800A4BC  GPIOB       BSRR         W 0x00000040  sz=4  -',
      '00:28:44 service ready: reset pc=0x080030D4 sp=0x20000CD0'
    ]);
    eq(entry.length, 2);
    includes(app.renderPeripheralRows(entry, 'GPIOB'), 'GPIOB.BSRR');
    includes(app.renderPeripheralRows(entry, 'service ready'), 'service ready');
    eq(entry[0].addr, null);
  });

  await test('工具函数：hex / 地址解析 / 计数 / IPC / 轮询间隔', () => {
    eq(app.hex8(0x080030D4), '080030D4');
    eq(app.hex32(0x080030D4), '0x080030D4');
    eq(app.parseAddr('0x080030D4'), 0x080030D4);
    eq(app.parseAddr('080030D4'), 0x080030D4);
    eq(app.parseAddr('zzz'), null);
    eq(app.formatCount(184361), '184,361');
    eq(app.computeIpc(1200, 200), 6000);
    eq(app.formatIpc(6000), '6.0k/s');
    eq(app.formatIpc(2500000), '2.50M/s');
    eq(app.pickPollInterval(true, false, false), 200);
    eq(app.pickPollInterval(false, false, false), 800);
    eq(app.pickPollInterval(false, true, false), 2000);
    eq(app.pickPollInterval(true, false, true), 2500);
    eq(app.mergeBreakpoints([1, 3], [3, 5]).join(','), '1,3,5');
  });

  await test('虚拟输入工具函数：电量换算 / 命令体构造', () => {
    eq(app.batteryMvToPct(3700), 69);
    eq(app.batteryMvToPct(2600), 0);
    eq(app.batteryMvToPct(4200), 100);
    includes(app.formatBattery(3700), '3.70 V');
    includes(app.formatBattery(3700), '69%');
    eq(JSON.stringify(app.batteryCmd(3900)), JSON.stringify({ cmd: 'battery', mv: 3900 }));
    eq(JSON.stringify(app.batteryCmd(9999)), JSON.stringify({ cmd: 'battery', mv: 4200 }), '电压应被夹到 4.2V');
    eq(JSON.stringify(app.keyDownCmd('power')), JSON.stringify({ cmd: 'key', key: 'power', down: true }));
    eq(JSON.stringify(app.keyUpCmd('power')), JSON.stringify({ cmd: 'key_up', key: 'power' }));
    eq(JSON.stringify(app.keyPressCmd('power')), JSON.stringify({ cmd: 'key_press', key: 'power', ms: 80 }));
    eq(JSON.stringify(app.screenSaveCmd()), JSON.stringify({ cmd: 'screen_save' }));
    eq(app.KEY_DEFS.length, 6);
  });

  await test('normalizeInputs / renderInputStatus：后端 inputs 字段兼容与渲染', () => {
    const inputs = app.normalizeInputs({
      battery_mv: 3850,
      keys: { power: true, mode: false },
      last: { type: 'key_press', key: 'power', ms: 80 },
      adc: { ch0: 0x0A3C },
      screen_save: 'out/screen_001.png'
    });
    eq(inputs.batteryMv, 3850);
    eq(inputs.batteryPct, 78);
    eq(inputs.keys.power, true);
    eq(inputs.pressed.join(','), 'power');
    eq(inputs.last.type, 'key_press');
    eq(inputs.screenSave, 'out/screen_001.png');
    const html = app.renderInputStatus({ inputs: inputs });
    includes(html, '3.85 V');
    includes(html, 'power');
    includes(html, 'key_press');
    includes(html, 'ADC');
    includes(html, 'screen_001.png');
    includes(app.renderInputStatus({}), '未提供');
    eq(app.normalizeInputs(null), null);
    // 仅 pressed 数组 / 电压字符串写法
    const alt = app.normalizeInputs({ battery: '3700', pressed: ['up', 'down'] });
    eq(alt.batteryMv, 3700);
    eq(alt.pressed.join(','), 'up,down');
    // 嵌入 state 模型
    const model = app.normalizeState({ pc: 0x080030D4, inputs: { battery_mv: 3700 } });
    eq(model.inputs.batteryMv, 3700);
  });

  await test('mock 命令：battery / key / key_up / key_press / screen_save 状态与回显', async () => {
    mock.cmd({ cmd: 'battery', mv: 3900 });
    let s = mock.state();
    eq(s.inputs.battery_mv, 3900);
    eq(s.inputs.battery_pct, 81);
    eq(s.inputs.last.type, 'battery');
    ok(s.inputs.adc && typeof s.inputs.adc.ch0 === 'number');

    let r = mock.cmd({ cmd: 'key', key: 'power', down: true });
    eq(r.ok, true);
    s = mock.state();
    eq(s.inputs.keys.power, true);
    includes(s.inputs.pressed.join(','), 'power');
    eq(s.inputs.last.type, 'key_down');

    r = mock.cmd({ cmd: 'key_up', key: 'power' });
    eq(r.ok, true);
    s = mock.state();
    eq(s.inputs.keys.power, false);
    eq(s.inputs.last.type, 'key_up');

    r = mock.cmd({ cmd: 'key_press', key: 'up', ms: 60 });
    eq(r.ok, true);
    eq(r.key, 'up');
    eq(r.ms, 60);
    s = mock.state();
    eq(s.inputs.keys.up, true, '脉冲按下瞬间应为按下状态');
    eq(s.inputs.last.type, 'key_press');
    await new Promise(resolve => setTimeout(resolve, 120));
    s = mock.state();
    eq(s.inputs.keys.up, false, '脉冲 ms 后应自动释放');

    r = mock.cmd({ cmd: 'screen_save' });
    eq(r.ok, true);
    includes(r.path, '.png');
    includes(mock.state().inputs.screen_save, '.png');
    // 复位恢复到初始 inputs（不断言清空，仅确认命令仍可用）
    mock.cmd({ cmd: 'reset' });
  });

  await test('mock HTTP：POST /api/cmd 注入（battery/key/key_up/key_press/screen_save）', async () => {
    async function post(body) {
      const r = await mock.fetch('/api/cmd', { method: 'POST', body: JSON.stringify(body) });
      return { status: r.status, body: await r.json() };
    }
    let r = await post(app.batteryCmd(3700));
    eq(r.status, 200); eq(r.body.ok, true); eq(r.body.battery_mv, 3700);
    r = await post(app.keyDownCmd('mode'));
    eq(r.body.ok, true); eq(r.body.key, 'mode'); eq(r.body.down, true);
    r = await post(app.keyUpCmd('mode'));
    eq(r.body.down, false);
    r = await post(app.keyPressCmd('set', 80));
    eq(r.body.ok, true); eq(r.body.ms, 80);
    r = await post(app.screenSaveCmd());
    eq(r.body.ok, true); includes(r.body.path, 'screen_save_');
    const state = await (await mock.fetch('/api/state')).json();
    ok(state.inputs && typeof state.inputs.battery_mv === 'number');
  });

  await test('主循环/开机：isMainLoopPc / mapTaskPhase / formatPhaseInfo / 命令体', () => {
    eq(app.isMainLoopPc(0x0800D008), true);
    eq(app.isMainLoopPc(0x0800CAB8), true, 'boot_interactive settle 后的 pc 也算主循环');
    eq(app.isMainLoopPc(0x080030D4), false);
    eq(app.isMainLoopPc(null), false);
    eq(app.mapTaskPhase('scheduler'), '主循环 1ms');
    eq(app.mapTaskPhase('boot'), '启动中');
    eq(app.mapTaskPhase('main_loop'), '主循环');
    const m = app.normalizeState({
      pc: 0x0800D008, inMainLoop: true, taskPhase: 'scheduler',
      pcName: '0x0800D008', virtualTicks: 45,
      lcd: { w: 128, h: 64, format: '1bpp', nonzero: 290 }
    });
    eq(m.pcName, '0x0800D008');
    eq(m.taskPhase, 'scheduler');
    eq(m.inMainLoop, true);
    eq(m.virtualTicks, 45);
    const info = app.formatPhaseInfo(m);
    includes(info, '已进入主循环');
    includes(info, '主循环 1ms');
    includes(info, 'tick=45');
    includes(info, 'LCD 非零 290B');
    eq(JSON.stringify(app.bootInteractiveCmd()), JSON.stringify({ cmd: 'boot_interactive', warmup: 1000000 }));
    eq(JSON.stringify(app.bootInteractiveCmd(500000)), JSON.stringify({ cmd: 'boot_interactive', warmup: 500000 }));
    eq(JSON.stringify(app.clearInputsCmd()), JSON.stringify({ cmd: 'clear_inputs' }));
  });

  await test('mock 开机 / 清除输入命令与状态字段', () => {
    mock.cmd({ cmd: 'reset' });
    let s = mock.state();
    eq(s.inMainLoop, false);
    eq(s.taskPhase, 'boot');
    eq(s.pcName, 'Reset_Handler');
    let r = mock.cmd({ cmd: 'boot_interactive' });
    eq(r.ok, true);
    eq(r.taskPhase, 'scheduler');
    s = mock.state();
    eq(s.pc, 0x0800D008);
    eq(s.inMainLoop, true);
    eq(s.taskPhase, 'scheduler');
    eq(s.pcName, '0x0800D008');
    ok(s.virtualTicks > 0);
    // 清除输入
    mock.cmd({ cmd: 'key', key: 'power', down: true });
    s = mock.state();
    eq(s.inputs.keys.power, true);
    r = mock.cmd({ cmd: 'clear_inputs' });
    eq(r.ok, true);
    s = mock.state();
    eq(s.inputs.keys.power, false);
    eq(s.inputs.pressed.length, 0);
    eq(s.inputs.last.type, 'clear_inputs');
    mock.cmd({ cmd: 'reset' });
  });

  await test('2.4G 射频：RF 字段兼容 / IRQ 映射 / hex 校验 / 命令体', () => {
    eq(app.normalizeRfHex('A9 50 B4 02'), 'A9 50 B4 02');
    eq(app.normalizeRfHex('a950b402'), 'A9 50 B4 02');
    eq(app.normalizeRfHex('0xA9,0x50'), 'A9 50');
    eq(app.normalizeRfHex('A9 5'), null);
    eq(app.normalizeRfHex('zz'), null);
    eq(JSON.stringify(app.rfRxCmd('a950b402')), JSON.stringify({ cmd: 'rf_rx', hex: 'A9 50 B4 02' }));
    eq(JSON.stringify(app.rfInitCmd()), JSON.stringify({ cmd: 'rf_init' }));
    eq(JSON.stringify(app.rfStatusCmd()), JSON.stringify({ cmd: 'rf_status' }));

    const rf = app.normalizeRf(mock.state().rf);
    eq(rf.state, 'RX');
    eq(rf.rxCount, 1);
    eq(rf.txCount, 0);
    eq(rf.freq2, 0x5E);
    eq(rf.pktlen, 4);
    eq(rf.gdo0, 0);
    eq(rf.lastRx, 'd5190000');
    const rfStatus = app.formatRfStatus({ rf: rf });
    includes(rfStatus, 'state=RX');
    includes(rfStatus, 'freq2=0x5E');
    includes(rfStatus, 'pktlen=4');
    includes(rfStatus, 'RX 1 / TX 0');
    includes(rfStatus, 'GDO0=0');

    eq(app.irqLabel(15), 'SysTick');
    eq(app.irqLabel(35), 'TIM14');
    eq(app.irqLabel(21), 'EXTI0_1');
    eq(app.irqLabel(23), 'EXTI4_15/RF');
    const irq = app.normalizeIrqCounts({ 15: 33, 35: 166, 23: 2 });
    eq(irq.items.length, 3);
    eq(irq.total, 201);
    const irqLine = app.formatIrqLine({ irqCounts: irq });
    includes(irqLine, 'SysTick=33');
    includes(irqLine, 'TIM14=166');
    includes(irqLine, 'EXTI4_15/RF=2');
    const log = app.normalizeIrqLog([{ insn: 100, irq: 23, handler: 0x0800BA00, source: 'GDO0' }]);
    eq(log[0].label, 'EXTI4_15/RF');
    includes(app.formatIrqLogEntry(log[0]), '@0x0800BA00');
    includes(app.formatIrqLogEntry(log[0]), 'GDO0');
    const html = app.renderRfPanel({
      rf: rf,
      irqCounts: irq,
      irqLog: log
    });
    includes(html, 'state=RX');
    includes(html, 'RX 1 / TX 0');
    includes(html, 'IRQ');
    includes(html, 'GDO0');
  });

  await test('mock 射频命令：rf_init / rf_rx / rf_status / irq_status', () => {
    mock.cmd({ cmd: 'rf_init' });
    let s = mock.state();
    eq(s.rf.state, 'RX');
    const before = s.rf.rx_count;
    const irqBefore = s.irqCounts['23'] || 0;
    let r = mock.cmd({ cmd: 'rf_rx', hex: 'A9 50 B4 02' });
    eq(r.ok, true);
    eq(r.len, 4);
    eq(r.rf.rx_count, before + 1);
    eq(r.rf.last_rx, 'a950b402');
    eq(r.rf.gdo0, 1);
    eq(r.irqCounts['23'], irqBefore + 1);
    s = mock.state();
    includes(s.rf.last_rx, 'a950b402');
    eq(s.irqCounts['23'], irqBefore + 1);
    r = mock.cmd({ cmd: 'rf_status' });
    eq(r.ok, true);
    eq(r.rf.state, 'RX');
    r = mock.cmd({ cmd: 'irq_status' });
    eq(r.ok, true);
    ok(r.irqCounts['15'] >= 0);
    ok(Array.isArray(r.irqLog) && r.irqLog.length >= 1);
    r = mock.cmd({ cmd: 'rf_rx', hex: 'A9 5' });
    eq(r.ok, false);
    mock.cmd({ cmd: 'reset' });
  });

  /* ---------------- 前端静态资源一致性 ---------------- */
  await test('index.html：与 app.js 的 DOM id 对齐（最小 DOM 冒烟）', () => {
    const html = fs.readFileSync(path.join(webuiDir, 'index.html'), 'utf8');
    const js = fs.readFileSync(path.join(webuiDir, 'app.js'), 'utf8');
    const ids = new Set();
    const re = /\$\('#([A-Za-z0-9_-]+)'\)/g;
    let match;
    while ((match = re.exec(js))) ids.add(match[1]);
    ok(ids.size > 30, '应引用足够多的 DOM 节点');
    const missing = [];
    ids.forEach(id => { if (html.indexOf('id="' + id + '"') < 0) missing.push(id); });
    eq(missing.join(','), '', '缺少 DOM id: ' + missing.join(', '));
    // 关键面板存在
    ['disasmBody', 'regGrid', 'memTable', 'periphList', 'uartConsole', 'lcdCanvas'].forEach(id => {
      ok(html.indexOf('id="' + id + '"') >= 0, '缺少 #' + id);
    });
  });

  await test('离线约束：index.html / style.css 无外部 CDN、字体、脚本依赖', () => {
    const html = fs.readFileSync(path.join(webuiDir, 'index.html'), 'utf8');
    const css = fs.readFileSync(path.join(webuiDir, 'style.css'), 'utf8');
    const refs = [];
    const re = /(?:src|href)="([^"]+)"/g;
    let m;
    while ((m = re.exec(html))) refs.push(m[1]);
    refs.forEach(r => {
      ok(/^(style\.css|app\.js|mock\.js)$/.test(r), '非本地资源引用: ' + r);
    });
    ok(!/https?:\/\//i.test(css), 'style.css 不应含外部 URL');
    ok(!/@import/i.test(css), 'style.css 不应 @import');
    ok(!/<link[^>]+https?:/i.test(html), 'index.html 不应引用外链样式');
  });

  /* ---------------- 摘要 ---------------- */
  console.log('');
  console.log('---------------------------------------------');
  console.log('通过 ' + passed + ' 项，失败 ' + failures.length + ' 项');
  if (failures.length) {
    console.log('失败明细：');
    failures.forEach(f => console.log('  - ' + f));
    process.exitCode = 1;
  } else {
    console.log('关键面板自测结论：mock 模式可渲染 寄存器/标志/函数名、反汇编(PC 高亮+断点)、');
    console.log('内存(hex+ASCII)、外设日志(可过滤)、UART 控制台、调用栈、LCD 屏幕数据。');
  }
}

main().catch(function (e) {
  console.error('selftest 崩溃：', e);
  process.exitCode = 1;
});
