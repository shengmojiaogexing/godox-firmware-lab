// 0x08029fc6 FUN_08029fc6 size=88

void FUN_08029fc6(undefined4 param_1,int param_2,int param_3,int *param_4,int param_5,int param_6,
                 int *param_7)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = param_4[1];
  iVar1 = *param_4;
  iVar4 = param_2 + iVar3 * param_3 * 2 + iVar1 * 2;
  iVar5 = param_5 + param_7[1] * param_6 * 2 + *param_7 * 2;
  iVar2 = param_4[2];
  for (; iVar3 <= param_4[3]; iVar3 = iVar3 + 1) {
    FUN_0802d764(iVar4,iVar5,((iVar2 - iVar1) + 1) * 2);
    iVar4 = iVar4 + param_3 * 2;
    iVar5 = iVar5 + param_6 * 2;
  }
  return;
}

