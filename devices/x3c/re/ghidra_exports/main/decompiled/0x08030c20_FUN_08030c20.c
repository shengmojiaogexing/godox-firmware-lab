// 0x08030c20 FUN_08030c20 size=78

void FUN_08030c20(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  int local_10;
  
  local_10 = param_4;
  iVar1 = FUN_080180c2(param_1);
  local_10 = 0;
  FUN_0802ad74(param_1,0x13,&local_10);
  if (local_10 != iVar1) {
    FUN_080302d0(param_1);
  }
  if (*(int *)(param_1 + 8) == 0) {
    if (local_10 != 0) {
      FUN_0802dee0(param_1);
      *(int *)(*(int *)(param_1 + 8) + 0x1c) = local_10;
    }
  }
  else {
    *(int *)(*(int *)(param_1 + 8) + 0x1c) = local_10;
  }
  if (local_10 != iVar1) {
    FUN_080302d0(param_1);
  }
  return;
}

