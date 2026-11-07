// 0x0802e292 FUN_0802e292 size=64

void FUN_0802e292(int param_1)

{
  int iVar1;
  
  FUN_080302d0(param_1);
  iVar1 = FUN_0802ebc8(param_1,0);
  while (iVar1 != 0) {
    FUN_08035d4c();
    iVar1 = FUN_0802ebc8(param_1,0);
  }
  FUN_08031170(param_1,0,0);
  if (*(int *)(param_1 + 8) != 0) {
    *(undefined4 *)(*(int *)(param_1 + 8) + 0x10) = 0;
    *(undefined4 *)(*(int *)(param_1 + 8) + 0x14) = 0;
  }
  return;
}

