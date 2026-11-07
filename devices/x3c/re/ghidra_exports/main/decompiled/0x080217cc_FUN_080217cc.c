// 0x080217cc FUN_080217cc size=34

undefined4 FUN_080217cc(void)

{
  undefined4 uVar1;
  
  uVar1 = 3;
  if ((*DAT_080217f0 & 1) != 0) {
    return 0;
  }
  if ((int)(*DAT_080217f0 << 0x1d) < 0) {
    uVar1 = 1;
  }
  else if ((int)(*DAT_080217f0 << 0x1b) < 0) {
    return 2;
  }
  return uVar1;
}

