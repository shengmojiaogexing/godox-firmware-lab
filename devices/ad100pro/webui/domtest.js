/*!
 * domtest.js — 无 jsdom 的最小 DOM 冒烟测试
 *
 * 做法：读取 index.html 收集全部 id，构造一套"够用就好"的假 DOM（元素、
 * classList、querySelector 等），然后 require app.js 让它在 mock 模式下
 * 跑完整的 boot() 流程，最后断言各面板真的被渲染出内容。
 *
 * 运行：node webui/domtest.js
 * 这同时是 index.html?mock=1 在浏览器里能否无异常渲染的回归测试。
 */
'use strict';

const fs = require('fs');
const path = require('path');
const assert = require('assert');
const dir = __dirname;

const errors = [];
let checks = 0;

function ok(v, msg) {
  assert.ok(v, msg || 'expected truthy');
  checks++;
}
function includes(hay, needle, msg) {
  assert.ok(String(hay).indexOf(needle) >= 0, msg || ('expected to include ' + JSON.stringify(needle)));
  checks++;
}
function eq(a, b, msg) {
  assert.strictEqual(a, b, msg || ('expected ' + JSON.stringify(b) + ', got ' + JSON.stringify(a)));
  checks++;
}

/* ---------- 1. 收集 index.html 的所有 id ---------- */
const html = fs.readFileSync(path.join(dir, 'index.html'), 'utf8');
const ids = new Set();
let m;
const idRe = /\bid="([A-Za-z0-9_-]+)"/g;
while ((m = idRe.exec(html))) ids.add(m[1]);

/* ---------- 2. 假 DOM ---------- */
class FakeClassList {
  constructor() { this.set = new Set(); }
  add(c) { this.set.add(c); }
  remove(c) { this.set.delete(c); }
  contains(c) { return this.set.has(c); }
  toggle(c, force) {
    const on = force === undefined ? !this.set.has(c) : !!force;
    if (on) this.set.add(c); else this.set.delete(c);
    return on;
  }
}

class FakeElement {
  constructor(id) {
    this.id = id;
    this.innerHTML = '';
    this.textContent = '';
    this.className = '';
    this.classList = new FakeClassList();
    this.style = {};
    this.dataset = {};
    this.hidden = false;
    this.disabled = false;
    this.value = '';
    this.checked = false;
    this.scrollTop = 0;
    this.scrollHeight = 1000;
    this.clientHeight = 400;
    this.offsetTop = 0;
    this.offsetHeight = 20;
    this.width = 320;
    this.height = 240;
    this.listeners = {};
  }
  addEventListener(type, fn) { (this.listeners[type] = this.listeners[type] || []).push(fn); }
  removeEventListener() {}
  querySelector() { return null; }
  querySelectorAll() { return []; }
  getAttribute(name) { return this.dataset[name] || null; }
  setAttribute(name, v) { this.dataset[name] = String(v); }
  getContext() {
    return {
      createImageData: function (w, h) { return { width: w, height: h, data: new Uint8ClampedArray(w * h * 4) }; },
      putImageData: function () {}
    };
  }
  getBoundingClientRect() { return { top: 0, left: 0, width: 320, height: 240 }; }
  focus() {}
  blur() {}
  select() {}
  closest() { return null; }
}

const elements = {};
ids.forEach(id => { elements[id] = new FakeElement(id); });

const documentListeners = {};
global.document = {
  readyState: 'complete',
  hidden: false,
  activeElement: null,
  getElementById(id) { return elements[id] || null; },
  querySelectorAll() { return []; },
  querySelector() { return null; },
  addEventListener(type, fn) { (documentListeners[type] = documentListeners[type] || []).push(fn); },
  removeEventListener() {},
  createElement() { return new FakeElement('created'); }
};
global.window = globalThis;
global.location = { search: '?mock=1', href: 'file:///fake/index.html?mock=1' };
global.addEventListener = function (type, fn) { (documentListeners[type] = documentListeners[type] || []).push(fn); };
global.removeEventListener = function () {};
global.fetch = function () { throw new Error('mock 模式不应调用真实 fetch'); };
global.setInterval = global.setInterval || setInterval;

/* 捕获 boot 期间的异常 */
process.on('uncaughtException', e => errors.push('uncaught: ' + (e && e.stack || e)));

/* ---------- 3. 加载 mock.js + app.js（app.js 会自动 boot） ---------- */
let app, mock;
try {
  mock = require('./mock.js');
  app = require('./app.js');
} catch (e) {
  console.error('加载 app.js 失败：\n' + (e && e.stack || e));
  process.exit(1);
}

function el(id) { return elements[id]; }

async function wait(ms) { return new Promise(r => setTimeout(r, ms)); }

async function main() {
  // 等待 mock 的异步轮询（/api/state 延迟约 20~55ms）+ 屏幕轮询
  await wait(700);

  console.log('=== webui DOM 冒烟测试（fake DOM，共 ' + ids.size + ' 个 id）===');

  ok(errors.length === 0, 'boot 期间不应有未捕获异常：' + errors.join(' | '));

  // 顶栏
  eq(el('insnCount').textContent, '184,361', '指令计数');
  includes(el('connText').textContent, 'MOCK', '连接状态应显示 MOCK');
  eq(el('runState').textContent, '已暂停');
  includes(el('connDot').className, 'ok', 'mock 模式已连接时状态灯应为绿色');
  includes(el('modeBadge').textContent, 'MOCK');

  // 寄存器面板
  includes(el('regGrid').innerHTML, 'reg-name">pc<');
  includes(el('regGrid').innerHTML, '080030D4');
  includes(el('regGrid').innerHTML, 'reg-name">sp<');
  includes(el('regGrid').innerHTML, '20000CD0');
  eq((el('regGrid').innerHTML.match(/class="reg-row/g) || []).length, 16, '16 个寄存器行');
  includes(el('flagRow').innerHTML, 'data-flag="N"');
  includes(el('funcInfo').innerHTML, 'Reset_Handler');
  includes(el('funcInfo').innerHTML, '最近函数');

  // 反汇编
  includes(el('disasmBody').innerHTML, 'pc-line');
  includes(el('disasmBody').innerHTML, '0448');
  includes(el('disasmBody').innerHTML, 'ldr');
  includes(el('disasmBody').innerHTML, 'data-action="toggle-bp"');
  eq((el('disasmBody').innerHTML.match(/class="d-row/g) || []).length, 61, '61 行反汇编窗口');
  includes(el('disasmRange').textContent, '0x0800');

  // 内存
  includes(el('memTable').innerHTML, 'mem-row');
  includes(el('memTable').innerHTML, 'mem-ascii');
  eq((el('memTable').innerHTML.match(/class="mem-row/g) || []).length, 17, '16 行内存 + 表头');
  includes(el('memInfo').textContent, '0x');

  // 外设访问 / UART / 调用栈
  includes(el('periphList').innerHTML, 'plog-row');
  includes(el('periphList').innerHTML, 'RCC');
  includes(el('periphList').innerHTML, 'USART1_TDR');
  includes(el('uartConsole').innerHTML, 'AD100 Pro');
  includes(el('uartConsole').innerHTML, 'Reset_Handler');
  includes(el('callstack').innerHTML, '#0');
  includes(el('callstack').innerHTML, 'main');

  // LCD 屏幕（mock 返回 base64 1bpp page/column，画布应被显示，默认 rotate90+mirror）
  eq(el('lcdCanvas').hidden, false, 'LCD canvas 应显示');
  includes(el('lcdMeta').textContent, '128×64');
  includes(el('lcdMeta').textContent, '/page');
  includes(el('lcdMeta').textContent, '面板 90°镜像');
  eq(el('lcdCanvas').width, 64, '面板方向应转置为 64 宽');
  eq(el('lcdCanvas').height, 128, '面板方向应转置为 128 高');
  ok(app.state.screen && app.state.screen.w === 128, '屏幕数据应已归一化');
  ok(app.state.screen.data.length === 128 * Math.ceil(64 / 8), '1bpp/page 数据长度 = 1024');
  eq(app.state.screen.layout, 'page');

  // 开机按钮 / 主循环状态栏 / 清除输入按钮
  eq(el('btnBoot').disabled, false, '开机按钮可用');
  includes(el('phaseInfo').textContent, '启动中');
  includes(el('phaseInfo').textContent, 'Reset_Handler');
  eq(el('btnClearInputs').disabled, false, '清除输入按钮可用');
  eq(el('btnScreenOrient').classList.contains('active'), true, '默认面板方向为 rotate90+mirror');

  // 虚拟输入面板（mock inputs 回显 + 控件齐全）
  includes(el('inputStatus').innerHTML, '电量');
  includes(el('inputStatus').innerHTML, '3.70 V');
  includes(el('batteryVal').textContent, '3.70 V');
  eq(el('inputsSource').textContent, '后端状态');
  eq(el('btnScreenSave').disabled, false, 'mock 模式下保存按钮应可用');
  eq(el('pulseMode').checked, false);
  const htmlSrc = fs.readFileSync(path.join(dir, 'index.html'), 'utf8');
  eq((htmlSrc.match(/class="key-btn"/g) || []).length, 6, '应有 6 个虚拟按键');

  // 2.4G 射频面板
  includes(el('rfStatus').innerHTML, 'state=RX');
  includes(el('rfStatus').innerHTML, 'freq2=0x5E');
  includes(el('rfStatus').innerHTML, 'RX 1 / TX 0');
  includes(el('rfIrq').textContent, 'SysTick');
  eq(el('rfSource').textContent, '虚拟 CC2500');
  eq(el('btnRfInit').disabled, false, '射频初始化按钮可用');
  includes(htmlSrc, 'id="rfHexInput"');
  includes(htmlSrc, 'value="D5 80 07 00"');

  // 断点/内存/滚动等状态机
  eq(app.state.followPc, true);
  ok(app.state.bps.indexOf(0x0800312A) >= 0, '默认断点应同步到界面');
  eq(app.state.memAddr, 0x080030D4);

  console.log('PASS  insnCount=' + el('insnCount').textContent + '  conn=' + el('connText').textContent);
  console.log('PASS  regGrid 行数=' + (el('regGrid').innerHTML.match(/class="reg-row/g) || []).length + '  含 pc=080030D4');
  console.log('PASS  funcInfo=' + el('funcInfo').innerHTML.replace(/<[^>]+>/g, ' ').replace(/\s+/g, ' ').trim());
  console.log('PASS  disasm 行数=' + (el('disasmBody').innerHTML.match(/class="d-row/g) || []).length +
    '  范围=' + el('disasmRange').textContent);
  console.log('PASS  memTable 行数=' + (el('memTable').innerHTML.match(/class="mem-row/g) || []).length +
    '  memInfo=' + el('memInfo').textContent);
  console.log('PASS  periph 行数=' + (el('periphList').innerHTML.match(/class="plog-row/g) || []).length +
    '  uart 行数=' + (el('uartConsole').innerHTML.match(/class="uart-line/g) || []).length);
  console.log('PASS  callstack=' + el('callstack').innerHTML.replace(/<[^>]+>/g, ' ').replace(/\s+/g, ' ').trim());
  console.log('PASS  lcd meta=' + el('lcdMeta').textContent + '  canvas.hidden=' + el('lcdCanvas').hidden);
  console.log('PASS  虚拟输入 battery=' + el('batteryVal').textContent +
    '  status=' + el('inputStatus').innerHTML.replace(/<[^>]+>/g, ' ').replace(/\s+/g, ' ').trim());
  console.log('PASS  断点数=' + app.state.bps.length + '  followPc=' + app.state.followPc);
  console.log('---------------------------------------------');
  console.log('DOM 冒烟测试通过，共 ' + checks + ' 条断言。');
  process.exit(0);
}

main().catch(e => {
  console.error('DOM 冒烟测试失败：\n' + (e && e.stack || e));
  if (errors.length) console.error('boot 未捕获异常：\n' + errors.join('\n'));
  process.exit(1);
});
