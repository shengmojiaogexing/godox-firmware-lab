// 0x0802270c FUN_0802270c size=102

undefined8 FUN_0802270c(int param_1,int *param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_28;
  int *piStack_24;
  int local_20;
  undefined4 uStack_1c;
  
  local_28 = param_1;
  piStack_24 = param_2;
  local_20 = param_3;
  uStack_1c = param_4;
  iVar1 = FUN_0802faa0(param_1,0);
  iVar2 = FUN_0802faa0(param_1,0x40000);
  iVar3 = *(int *)(iVar1 + 8);
  iVar2 = *(int *)(iVar2 + 8);
  iVar1 = FUN_0802fab2(param_1,0);
  iVar1 = iVar1 + (iVar2 + iVar3) / 2;
  iVar2 = FUN_0802ed88(param_1);
  iVar2 = (*(int *)(param_1 + 0x18) + iVar2 / 2) - iVar1 / 2;
  param_2[1] = iVar2;
  param_2[3] = iVar2 + iVar1;
  FUN_0802ed04(param_1,&local_28);
  *param_2 = local_28;
  param_2[2] = local_20;
  return CONCAT44(piStack_24,local_28);
}

