// 0x08021618 FUN_08021618 size=34

undefined4 FUN_08021618(void)

{
  undefined4 uVar1;
  
  uVar1 = 3;
  if ((*DAT_0802163c & 1) != 0) {
    return 0;
  }
  if ((int)(*DAT_0802163c << 0x1d) < 0) {
    uVar1 = 1;
  }
  else if ((int)(*DAT_0802163c << 0x1b) < 0) {
    return 2;
  }
  return uVar1;
}

