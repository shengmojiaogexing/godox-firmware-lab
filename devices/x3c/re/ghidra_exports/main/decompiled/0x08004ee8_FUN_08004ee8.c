// 0x08004ee8 FUN_08004ee8 size=126

int FUN_08004ee8(uint *param_1,uint *param_2)

{
  uint *puVar1;
  uint uVar2;
  uint *puVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  
  while( true ) {
    uVar4 = *param_1;
    uVar5 = *param_2;
    if (uVar4 != uVar5) break;
    if ((uVar4 + 0xfefefeff & ~uVar4 & 0x80808080) != 0) {
      return 0;
    }
    puVar1 = param_1 + 2;
    uVar4 = param_1[1];
    puVar3 = param_2 + 2;
    uVar5 = param_2[1];
    if (uVar4 != uVar5) break;
    if ((uVar4 + 0xfefefeff & ~uVar4 & 0x80808080) != 0) {
      return 0;
    }
    param_1 = param_1 + 3;
    uVar4 = *puVar1;
    param_2 = param_2 + 3;
    uVar5 = *puVar3;
    if (uVar4 != uVar5) break;
    if ((uVar4 + 0xfefefeff & ~uVar4 & 0x80808080) != 0) {
      return 0;
    }
  }
  uVar2 = uVar4 - uVar5;
  uVar2 = LZCOUNT(uVar2 * 0x1000000 | (uVar2 >> 8 & 0xff) << 0x10 | (uVar2 >> 0x10 & 0xff) << 8 |
                  uVar2 >> 0x18) & 0x18;
  uVar6 = 0x1010101 >> (0x20 - uVar2 & 0xff);
  if ((uVar4 - uVar6 & ~uVar4 & uVar6 << 7) == 0) {
    return (uVar4 >> uVar2 & 0xff) - (uVar5 >> uVar2 & 0xff);
  }
  return 0;
}

