// 0x0801d126 FUN_0801d126 size=78

void FUN_0801d126(int param_1,int param_2,int param_3,undefined2 *param_4,int param_5)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  for (iVar5 = 0; iVar5 < param_3; iVar5 = iVar5 + 1) {
    iVar3 = (param_2 + -1) * param_3 + iVar5;
    if (param_1 != 0) {
      iVar3 = (param_2 * param_3 + -1) - iVar3;
    }
    for (iVar4 = 0; iVar4 < param_2; iVar4 = iVar4 + 1) {
      uVar1 = *param_4;
      param_4 = param_4 + 1;
      *(undefined2 *)(param_5 + iVar3 * 2) = uVar1;
      iVar2 = param_3;
      if (param_1 == 0) {
        iVar2 = -param_3;
      }
      iVar3 = iVar3 + iVar2;
    }
  }
  return;
}

