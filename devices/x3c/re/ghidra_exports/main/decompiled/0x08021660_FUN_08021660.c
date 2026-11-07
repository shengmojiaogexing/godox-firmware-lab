// 0x08021660 FUN_08021660 size=34

undefined4 FUN_08021660(void)

{
  undefined4 uVar1;
  
  uVar1 = 3;
  if ((*DAT_08021684 & 1) != 0) {
    return 0;
  }
  if ((int)(*DAT_08021684 << 0x1d) < 0) {
    uVar1 = 1;
  }
  else if ((int)(*DAT_08021684 << 0x1b) < 0) {
    return 2;
  }
  return uVar1;
}

