// 0x08035f30 FUN_08035f30 size=158

void FUN_08035f30(int param_1,int *param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  if (param_4 == 0) {
    iVar6 = param_2[1] - (param_3 >> 1);
    param_2[1] = iVar6;
    param_2[3] = param_3 + -1 + iVar6;
    *param_2 = *(int *)(param_1 + 0x14);
    param_2[2] = *(int *)(param_1 + 0x1c);
  }
  else {
    iVar6 = *param_2 - (param_3 >> 1);
    *param_2 = iVar6;
    param_2[2] = param_3 + -1 + iVar6;
    param_2[1] = *(int *)(param_1 + 0x18);
    param_2[3] = *(int *)(param_1 + 0x20);
  }
  iVar6 = FUN_0802f950(param_1,0x30000);
  iVar1 = FUN_0802f97a(param_1,0x30000);
  iVar2 = FUN_0802f9b0(param_1,0x30000);
  iVar3 = FUN_0802f91a(param_1,0x30000);
  iVar4 = FUN_0802fad0(param_1,0x30000);
  iVar5 = FUN_0802fac4(param_1,0x30000);
  *param_2 = *param_2 - (iVar6 + iVar4);
  param_2[2] = param_2[2] + iVar1 + iVar4;
  param_2[1] = param_2[1] - (iVar2 + iVar5);
  param_2[3] = iVar5 + iVar3 + param_2[3];
  return;
}

