// 0x0803b398 FUN_0803b398 size=84

undefined4
FUN_0803b398(int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  int iVar1;
  
  iVar1 = FUN_0803a738(param_3);
  *param_1 = iVar1;
  FUN_0803a326(param_1,param_2);
  if (*(char *)((int)param_1 + 0x2ff) != '\0') {
    *(uint *)(*param_1 + 0x38) = *(uint *)(*param_1 + 0x38) | 0x100000;
  }
  if (*(char *)((int)param_1 + 0x301) != '\0') {
    *(uint *)(*param_1 + 0x38) = *(uint *)(*param_1 + 0x38) | 0x200000;
  }
  FUN_0803aaa8(param_1 + 1,*param_1,param_4,param_5,param_2);
  return 0;
}

