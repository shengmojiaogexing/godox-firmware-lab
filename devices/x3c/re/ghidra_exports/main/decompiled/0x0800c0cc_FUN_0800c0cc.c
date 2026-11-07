// 0x0800c0cc FUN_0800c0cc size=124

undefined4 FUN_0800c0cc(void)

{
  undefined4 uVar1;
  uint uVar2;
  
  uVar1 = 0;
  uVar2 = (uint)*(byte *)(DAT_0800c148 + 7);
  if (((uVar2 == 0x38) || (uVar2 == 0x42)) || (uVar2 == 0x48)) {
    return 2;
  }
  if (((uVar2 == 0x3d) || (uVar2 == 0x40)) || ((uVar2 == 0x43 || (uVar2 == 0x36)))) {
    return 3;
  }
  if (((uVar2 == 0x3f) || (uVar2 == 0x46)) || (uVar2 == 0x44)) {
    return 5;
  }
  if ((uVar2 == 0x39) || (uVar2 == 0x3a)) {
    return 6;
  }
  if (((uVar2 == 0x32) || (uVar2 == 0x3b)) || (uVar2 == 0x37)) {
    return 7;
  }
  if (uVar2 == 0xef) {
    return 9;
  }
  if ((((uVar2 == 0x3c) || (uVar2 == 0x3e)) ||
      ((uVar2 == 0x41 || ((uVar2 == 0x45 || (uVar2 == 0x47)))))) || (uVar2 - 0x49 < 0x18)) {
    uVar1 = 3;
  }
  return uVar1;
}

