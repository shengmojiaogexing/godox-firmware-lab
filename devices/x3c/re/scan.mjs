// Scan literal pools / peripheral addresses / thumb2 density
import fs from 'fs';
const b = fs.readFileSync(process.argv[2] || 'devices/ad100pro/firmware/AD100pro_V1.2.bin');

// --- thumb2 density: count halfwords in prefixes
let hw = new Array(65536).fill(0);
let n = 0;
for (let i = 0; i + 2 <= b.length; i += 2) { hw[b.readUInt16LE(i)]++; n++; }
const range = (a, z) => { let c = 0; for (let v = a; v <= z; v++) c += hw[v]; return c; };
console.log('halfwords', n);
console.log('E800-EFFF (32-bit prefix):', range(0xe800, 0xefff), (range(0xe800, 0xefff) / n * 100).toFixed(2) + '%');
console.log('F000-F7FF (32-bit prefix):', range(0xf000, 0xf7ff), (range(0xf000, 0xf7ff) / n * 100).toFixed(2) + '%');
console.log('F800-FFFF:', range(0xf800, 0xffff));

// --- 32-bit words in address ranges
const buckets = {
  'flash 0x08000000': [], 'periph 0x40000000': [], 'periph 0x48000000': [], 'periph 0x50000000': [],
  'periph 0xE000': [], 'ram 0x20000000': [], 'other-32': []
};
const addrs = new Map();
for (let i = 0; i + 4 <= b.length; i += 4) {
  const v = b.readUInt32LE(i);
  let k = null;
  if (v >= 0x08000000 && v < 0x08010000) k = 'flash 0x08000000';
  else if (v >= 0x40000000 && v < 0x48000000) k = 'periph 0x40000000';
  else if (v >= 0x48000000 && v < 0x50000000) k = 'periph 0x48000000';
  else if (v >= 0x50000000 && v < 0x60000000) k = 'periph 0x50000000';
  else if (v >= 0xE0000000 && v < 0xE0100000) k = 'periph 0xE000';
  else if (v >= 0x20000000 && v < 0x20002000) k = 'ram 0x20000000';
  if (k) { buckets[k].push([i, v]); addrs.set(v, (addrs.get(v) || 0) + 1); }
}
for (const [k, arr] of Object.entries(buckets)) console.log(k, 'count', arr.length);

console.log('\n=== peripheral-ish addresses by frequency (>=2) ===');
const per = [...addrs.entries()].filter(([v]) => v >= 0x40000000 && v < 0x60000000).sort((a, b) => b[1] - a[1]);
for (const [v, c] of per) if (c >= 2) console.log('0x' + v.toString(16).padStart(8, '0'), 'x' + c);
console.log('\nall unique periph addrs:', per.length);
console.log(per.map(([v, c]) => '0x' + v.toString(16) + '(' + c + ')').join(' '));

console.log('\n=== flash pointers (top 40 by frequency) ===');
const fl = [...addrs.entries()].filter(([v]) => v >= 0x08000000 && v < 0x08010000).sort((a, b) => b[1] - a[1]);
for (const [v, c] of fl.slice(0, 40)) console.log('0x' + v.toString(16).padStart(8, '0'), 'x' + c);
console.log('unique flash ptrs:', fl.length);

// align 4 offsets histogram for flash pointers -> literal pool alignment hints
const mod4 = new Map();
for (const [v] of fl) { const m = (v - 0x08000000) & 3; mod4.set(m, (mod4.get(m) || 0) + 1); }
console.log('flash ptr mod4:', [...mod4.entries()]);
