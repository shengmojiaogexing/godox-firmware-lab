// 0x080fa2d4 FUN_080fa2d4 size=68

void FUN_080fa2d4(uint param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = (param_1 & 0xf) << ((param_2 & 3) << 2);
  if (param_2 < 4) {
    *(uint *)(DAT_080fa318 + 8) = *(uint *)(DAT_080fa318 + 8) | uVar1;
    return;
  }
  if (param_2 - 4 < 4) {
    *(uint *)(DAT_080fa318 + 0xc) = *(uint *)(DAT_080fa318 + 0xc) | uVar1;
    return;
  }
  if (param_2 - 8 < 4) {
    *(uint *)(DAT_080fa318 + 0x10) = *(uint *)(DAT_080fa318 + 0x10) | uVar1;
  }
  else if (param_2 - 0xc < 4) {
    *(uint *)(DAT_080fa318 + 0x14) = *(uint *)(DAT_080fa318 + 0x14) | uVar1;
    return;
  }
  return;
}

