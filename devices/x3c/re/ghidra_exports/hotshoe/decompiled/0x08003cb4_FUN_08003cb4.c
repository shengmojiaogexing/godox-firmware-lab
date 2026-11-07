// 0x08003cb4 FUN_08003cb4 size=56

undefined4 FUN_08003cb4(void)

{
  int iVar1;
  int local_8;
  
  iVar1 = DAT_08003cec;
  local_8 = 0;
  if (-1 < *(int *)(DAT_08003cec + 0xc) << 0x19) {
    *(undefined4 *)(DAT_08003cec + 0xc) = 0xffffffff;
    do {
      local_8 = local_8 + 1;
      if (local_8 == iVar1 >> 0x10) break;
    } while ((*(uint *)(iVar1 + 0xc) & 0x7f) >> 6 == 0);
    if (-1 < *(int *)(iVar1 + 0xc) << 0x19) {
      return 0;
    }
  }
  return 1;
}

