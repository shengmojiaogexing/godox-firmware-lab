// 0x080fd4b8 FUN_080fd4b8 size=120

undefined4 FUN_080fd4b8(void)

{
  undefined4 uVar1;
  uint uVar2;
  
  uVar1 = 0;
  if (*DAT_080fd530 == '\x01') {
    uVar2 = (uint)*DAT_080fd534;
    if (((uVar2 == 0x38) || (uVar2 == 0x42)) || (uVar2 == 0x48)) {
      return 2;
    }
    if (((uVar2 != 0x3d) && (uVar2 != 0x40)) && (uVar2 != 0x43)) {
      if (((uVar2 == 0x3f) || (uVar2 == 0x46)) || (uVar2 == 0x44)) {
        return 5;
      }
      if ((uVar2 == 0x39) || (uVar2 == 0x3a)) {
        return 6;
      }
      if ((uVar2 == 0x32) || (uVar2 == 0x3b)) {
        return 7;
      }
      if (uVar2 == 0xef) {
        return 9;
      }
      if (((((uVar2 != 0x3c) && (uVar2 != 0x3e)) && (uVar2 != 0x41)) &&
          ((uVar2 != 0x45 && (uVar2 != 0x47)))) && (0x17 < uVar2 - 0x49)) {
        return 0;
      }
    }
    uVar1 = 3;
  }
  return uVar1;
}

