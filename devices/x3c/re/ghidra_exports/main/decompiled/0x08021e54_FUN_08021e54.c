// 0x08021e54 FUN_08021e54 size=88

void FUN_08021e54(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar2 = FUN_08021f20(param_1);
  iVar1 = DAT_08021eac;
  iVar5 = (*(int *)(DAT_08021eac + 8) - param_1) - iVar2;
  *(int *)(DAT_08021eac + 8) = *(int *)(DAT_08021eac + 8) - iVar2;
  if (iVar5 == 0) {
    return;
  }
  FUN_0802d764(param_1,param_1 + iVar2,iVar5);
  for (iVar4 = param_1; iVar4 != *(int *)(iVar1 + 8); iVar4 = iVar4 + iVar3) {
    *(int *)(iVar4 + 8) = *(int *)(iVar4 + 8) - iVar2;
    iVar3 = FUN_08021f20(iVar4);
  }
  FUN_0802d8d6(param_1 + iVar5,iVar2);
  return;
}

