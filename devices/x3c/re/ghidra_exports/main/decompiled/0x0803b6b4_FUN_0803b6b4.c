// 0x0803b6b4 FUN_0803b6b4 size=88

undefined4 FUN_0803b6b4(int param_1)

{
  byte bVar1;
  byte bVar2;
  
  if ((*(short *)(param_1 + 0x2d0) == 0) && (*(short *)(param_1 + 0x2d2) == 0)) {
    bVar1 = *(byte *)(param_1 + 0x2ce);
    bVar2 = bVar1 & 0x7f;
    if (*(char *)(param_1 + 0x2e4) == '\x03') {
      FUN_0803accc(param_1);
    }
    else {
      *(byte *)(param_1 + 0x2e6) = bVar2;
      FUN_0803b7b8(param_1,bVar2);
      if ((bVar1 & 0x7f) == 0) {
        *(undefined1 *)(param_1 + 0x2e4) = 1;
      }
      else {
        *(undefined1 *)(param_1 + 0x2e4) = 2;
      }
      FUN_0803acbc(param_1);
    }
  }
  else {
    FUN_0803accc(param_1);
  }
  return 0;
}

