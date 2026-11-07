// 0x0800ad00 FUN_0800ad00 size=86

uint FUN_0800ad00(void)

{
  int iVar1;
  uint uVar2;
  uint extraout_r3;
  
  iVar1 = FUN_0800ae28();
  if (*DAT_0800ad58 == '\0') {
    uVar2 = iVar1 - extraout_r3 & 0xff;
  }
  else {
    uVar2 = extraout_r3;
    if (*DAT_0800ad58 == '\x01') {
      uVar2 = ((extraout_r3 / 3) * 5 & 0x7f) * 2;
      if (extraout_r3 % 3 != 0) {
        if (extraout_r3 % 3 == 1) {
          uVar2 = uVar2 + 3 & 0xff;
        }
        else {
          uVar2 = uVar2 + 7 & 0xff;
        }
      }
      uVar2 = *DAT_0800ad5c - uVar2 & 0xff;
    }
  }
  return uVar2;
}

