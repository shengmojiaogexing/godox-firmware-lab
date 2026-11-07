// 0x08003c34 FUN_08003c34 size=62

undefined4 FUN_08003c34(void)

{
  int iVar1;
  int local_8;
  
  local_8 = 0;
  FUN_08003c88();
  iVar1 = DAT_08003c74;
  *(uint *)(DAT_08003c74 + 8) = *(uint *)(DAT_08003c74 + 8) & ~(DAT_08003c74 >> 0x16);
  while ((local_8 != iVar1 >> 0x10 && ((*(uint *)(iVar1 + 0xc) & 1) == 0))) {
    local_8 = local_8 + 1;
  }
  *(undefined4 *)(iVar1 + 0x24) = 0xff;
  if ((*(uint *)(iVar1 + 0xc) & 1) == 0) {
    return 0;
  }
  return 1;
}

