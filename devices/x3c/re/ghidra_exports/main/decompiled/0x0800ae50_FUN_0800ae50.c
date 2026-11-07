// 0x0800ae50 FUN_0800ae50 size=64

uint FUN_0800ae50(uint param_1)

{
  uint uVar1;
  uint uVar2;
  
  if (*DAT_0800ae90 < param_1) {
    uVar1 = 0;
  }
  else {
    uVar1 = *DAT_0800ae90 - param_1 & 0xffff;
    uVar2 = uVar1 % 10;
    if ((*DAT_0800ae94 != '\0') && (uVar1 = (uVar1 / 10 & 0xff) * 3, 2 < uVar2)) {
      if (uVar2 < 7) {
        return uVar1 + 1;
      }
      return uVar1 + 2;
    }
  }
  return uVar1;
}

