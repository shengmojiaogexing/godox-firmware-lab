// 0x08003c88 FUN_08003c88 size=12

void FUN_08003c88(void)

{
  int iVar1;
  
  iVar1 = DAT_08003c94;
  *(undefined4 *)(DAT_08003c94 + 0x24) = 0xca;
  *(undefined4 *)(iVar1 + 0x24) = 0x53;
  return;
}

