// 0x080180e2 FUN_080180e2 size=76

int FUN_080180e2(int param_1,int param_2,int param_3)

{
  int iVar1;
  
  if (param_2 == 0 && param_3 == 0) {
    return 1;
  }
  FUN_0802dee0(param_1);
  *(int *)(*(int *)(param_1 + 8) + 0x10) = *(int *)(*(int *)(param_1 + 8) + 0x10) + param_2;
  *(int *)(*(int *)(param_1 + 8) + 0x14) = *(int *)(*(int *)(param_1 + 8) + 0x14) + param_3;
  FUN_08030460(param_1,param_2,param_3,1);
  iVar1 = FUN_0802ad74(param_1,0xb,0);
  if (iVar1 == 1) {
    FUN_080302d0(param_1);
    iVar1 = 1;
  }
  return iVar1;
}

