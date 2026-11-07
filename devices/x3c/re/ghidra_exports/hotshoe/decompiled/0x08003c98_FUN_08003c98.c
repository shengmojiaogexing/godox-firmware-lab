// 0x08003c98 FUN_08003c98 size=22

void FUN_08003c98(void)

{
  int iVar1;
  
  FUN_08003c88();
  iVar1 = DAT_08003cb0;
  *(uint *)(DAT_08003cb0 + 8) = *(uint *)(DAT_08003cb0 + 8) | DAT_08003cb0 >> 0x16;
  *(undefined4 *)(iVar1 + 0x24) = 0xff;
  return;
}

