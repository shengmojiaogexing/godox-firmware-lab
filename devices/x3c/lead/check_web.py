import json, urllib.request, zlib, struct, os
base='http://127.0.0.1:18765'
s=json.load(urllib.request.urlopen(base+'/api/screen'))
print('screen',s['w'],s['h'],s['format'],'hexlen',len(s['hex']))
fb=bytes.fromhex(s['hex'])
nz=sum(1 for b in fb if b)
print('nonzero bytes',nz,'of',len(fb))
# save PNG
rgb=bytearray(s['w']*s['h']*3)
for i in range(0,len(fb),2):
    v=fb[i]|(fb[i+1]<<8)
    o=(i//2)*3
    rgb[o]=((v>>11)&0x1f)<<3; rgb[o+1]=((v>>5)&0x3f)<<2; rgb[o+2]=(v&0x1f)<<3
raw=bytearray()
for y in range(s['h']):
    raw.append(0); raw+=rgb[y*s['w']*3:(y+1)*s['w']*3]
def chunk(t,d): return struct.pack('>I',len(d))+t+d+struct.pack('>I',zlib.crc32(t+d)&0xffffffff)
png=b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',s['w'],s['h'],8,2,0,0,0))+chunk(b'IDAT',zlib.compress(bytes(raw),6))+chunk(b'IEND',b'')
open('docs/images/screen_web_preview.png','wb').write(png)
print('saved docs/images/screen_web_preview.png')
# api sanity
st=json.load(urllib.request.urlopen(base+'/api/state'))
print('state pc=0x%x insn=%d regs=%d func=%s'%(st['pc'],st['insnCount'],len(st['regs']),st['func']))
d=json.load(urllib.request.urlopen(base+'/api/disasm?addr=0x08004af8&n=6'))
print('disasm:', [ (hex(l['addr']),l['mnem'],l['op']) for l in d['lines'][:4]])
f=json.load(urllib.request.urlopen(base+'/api/functions'))
print('functions:',len(f),'first',f[0])
m=json.load(urllib.request.urlopen(base+'/api/mem?addr=0x08004800&len=16'))
print('mem:',m['hex'])
