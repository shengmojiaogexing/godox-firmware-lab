// 0x0801d174 FUN_0801d174 size=136

void FUN_0801d174(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  for (iVar4 = 0; iVar4 < param_2 / 2; iVar4 = iVar4 + 1) {
    for (iVar3 = 0; iVar3 < (param_2 + 1) / 2; iVar3 = iVar3 + 1) {
      iVar2 = (param_2 - iVar4) + -1;
      iVar1 = (param_2 - iVar3) + -1;
      if (param_1 == 0) {
        FUN_0801d0b0(param_3 + (iVar4 * param_2 + iVar3) * 2,param_3 + (iVar3 * param_2 + iVar2) * 2
                     ,param_3 + (iVar2 * param_2 + iVar1) * 2,
                     param_3 + (iVar1 * param_2 + iVar4) * 2);
      }
      else {
        FUN_0801d0b0(param_3 + (iVar4 * param_2 + iVar3) * 2,param_3 + (iVar1 * param_2 + iVar4) * 2
                     ,param_3 + (iVar2 * param_2 + iVar1) * 2,
                     param_3 + (iVar3 * param_2 + iVar2) * 2);
      }
    }
  }
  return;
}

