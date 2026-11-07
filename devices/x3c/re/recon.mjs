// Quick recon of AD100pro_V1.2.bin
import fs from 'fs';

const p = process.argv[2] || 'devices/ad100pro/firmware/AD100pro_V1.2.bin';
const b = fs.readFileSync(p);
const base = 0x08000000;
console.log('file', p, 'size', b.length, '0x' + b.length.toString(16));

// ---- vector table
console.log('\n=== vector table ===');
const names = ['SP','Reset','NMI','HardFault','MemManage','BusFault','UsageFault','RSV7','RSV8','RSV9','RSV10','SVC','DebugMon','RSV13','PendSV','SysTick'];
for (let i = 0; i < 64; i++) {
  const off = i * 4;
  if (off + 4 > b.length) break;
  const v = b.readUInt32LE(off);
  if (i > 15 && v === 0x080030e7) continue; // skip default handler duplicates
  let note;
  if (v === 0) note = 'zero';
  else if (v >= 0x20000000 && v < 0x20010000) note = 'RAM';
  else if (v >= base && v < base + 0x20000) note = 'flash+0x' + (v - base).toString(16);
  else note = '?';
  const nm = i < 16 ? names[i] : 'IRQ' + (i - 16);
  console.log(String(i).padStart(3), nm.padEnd(12), '0x' + v.toString(16).padStart(8, '0'), note);
}

// ---- entropy
console.log('\n=== entropy 512B blocks ===');
let lastH = -1, runStart = 0, runH = -1;
const blocks = [];
for (let off = 0; off < b.length; off += 512) {
  const end = Math.min(off + 512, b.length);
  const hist = new Array(256).fill(0);
  for (let i = off; i < end; i++) hist[b[i]]++;
  let H = 0;
  for (const c of hist) if (c) { const pr = c / (end - off); H -= pr * Math.log2(pr); }
  blocks.push([off, end, H]);
}
// collapse runs of similar entropy
for (const [s, e, H] of blocks) {
  if (runH < 0 || Math.abs(H - runH) > 0.35) {
    if (runH >= 0) console.log('0x' + runStart.toString(16).padStart(5, '0'), '-', '0x' + s.toString(16).padStart(5, '0'), 'H~' + runH.toFixed(2));
    runStart = s; runH = H;
  }
}
console.log('0x' + runStart.toString(16).padStart(5, '0'), '- end H~' + runH.toFixed(2));

// ---- strings
console.log('\n=== ASCII strings >=4 ===');
function asciiStrings(buf) {
  const out = [];
  let cur = '', start = 0;
  for (let i = 0; i <= buf.length; i++) {
    const c = i < buf.length ? buf[i] : 0;
    if (c >= 0x20 && c < 0x7f) { if (!cur) start = i; cur += String.fromCharCode(c); }
    else { if (cur.length >= 4) out.push([start, cur]); cur = ''; }
  }
  return out;
}
for (const [o, s] of asciiStrings(b)) console.log('0x' + o.toString(16).padStart(5, '0'), JSON.stringify(s));

// ---- utf16le strings >= 4
console.log('\n=== UTF-16LE strings >=4 ===');
const uo = [];
let u = '', us = 0;
for (let i = 0; i + 1 < b.length; i += 2) {
  const c = b[i] | (b[i + 1] << 8);
  if (c >= 0x20 && c < 0x7f) { if (!u) us = i; u += String.fromCharCode(c); }
  else { if (u.length >= 4) uo.push([us, u]); u = ''; }
}
if (u.length >= 4) uo.push([us, u]);
for (const [o, s] of uo) console.log('0x' + o.toString(16).padStart(5, '0'), JSON.stringify(s));
console.log('utf16 count', uo.length);
