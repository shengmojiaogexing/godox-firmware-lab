// 0x0803befc FUN_0803befc size=88

uint FUN_0803befc(uint param_1,uint param_2)

{
  uint extraout_r1;
  uint uVar1;
  bool bVar2;
  
  uVar1 = (param_2 & 0x7fffffff) + 0xc8000000;
  bVar2 = uVar1 == 0x100000;
  if (0xfffff < uVar1) {
    bVar2 = uVar1 == 0xff00000;
  }
  if ((0xfffff >= uVar1 || 0xff00000 < uVar1) || bVar2) {
    if ((int)uVar1 < 0x100000) {
      return param_2 & 0x80000000;
    }
    if (0xffdfffff < param_2 << 1) {
      FUN_0803c0b4(param_1);
      param_2 = extraout_r1;
    }
    return (param_2 >> 0x17 | 0xff) << 0x17;
  }
  bVar2 = (param_1 & 0x10000000) != 0;
  uVar1 = (param_2 & 0x80000000 | uVar1 * 8) + (param_1 >> 0x1d) + (uint)bVar2;
  if ((param_1 & 0xfffffff) != 0) {
    return uVar1;
  }
  if (bVar2) {
    uVar1 = uVar1 & 0xfffffffe;
  }
  return uVar1;
}

