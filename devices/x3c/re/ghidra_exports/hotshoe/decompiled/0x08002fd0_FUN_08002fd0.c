// 0x08002fd0 FUN_08002fd0 size=50

short FUN_08002fd0(void)

{
  short sVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = (uint)*DAT_08003004;
  if (0x50 < uVar3 - 0x10) {
    return 0;
  }
  if (uVar3 < 0x10) {
    uVar2 = 0x10;
  }
  else {
    uVar2 = uVar3;
  }
  sVar1 = *(short *)(uVar2 * 2 + DAT_08003008 + -0x20);
  if (uVar3 < 0x39) {
    sVar1 = sVar1 + -100;
  }
  else {
    sVar1 = sVar1 + -0x1e;
  }
  return sVar1;
}

