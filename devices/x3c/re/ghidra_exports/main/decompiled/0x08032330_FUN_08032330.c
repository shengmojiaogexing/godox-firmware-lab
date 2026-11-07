// 0x08032330 FUN_08032330 size=90

void FUN_08032330(undefined4 param_1,int param_2)

{
  int iVar1;
  
  *(undefined4 *)(param_2 + 0x90) = 0;
  *(byte *)(param_2 + 0x94) = *(byte *)(param_2 + 0x94) & 0xfc;
  FUN_0802e2d2(param_2,0x100);
  FUN_0802e2d2(param_2,0x10);
  iVar1 = FUN_08028228(0);
  if (iVar1 * 8 + 0x50 < 0x140) {
    iVar1 = 1;
  }
  else {
    iVar1 = FUN_08028228(0);
    iVar1 = (iVar1 * 8 + 0x50) / 0xa0;
  }
  FUN_08031290(param_2,iVar1);
  return;
}

