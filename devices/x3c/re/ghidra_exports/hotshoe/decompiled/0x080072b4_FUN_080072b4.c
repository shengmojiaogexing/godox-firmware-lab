// 0x080072b4 FUN_080072b4 size=28

void FUN_080072b4(int param_1)

{
  int iVar1;
  
  iVar1 = DAT_080072d0;
  *(int *)(DAT_080072d0 + 0x14) = param_1 << 4;
  *(undefined4 *)(iVar1 + 0x18) = 0;
  *(undefined4 *)(iVar1 + 0x10) = 5;
  do {
  } while (-1 < *(int *)(iVar1 + 0x10) << 0xf);
  *(undefined4 *)(iVar1 + 0x10) = 4;
  *(undefined4 *)(iVar1 + 0x18) = 0;
  return;
}

