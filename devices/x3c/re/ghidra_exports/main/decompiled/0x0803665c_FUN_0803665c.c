// 0x0803665c FUN_0803665c size=136

void FUN_0803665c(undefined4 param_1,int param_2)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  if (param_2 == 0) {
    param_2 = FUN_080282b0(*(undefined4 *)(DAT_080366e4 + 4));
  }
  if (param_2 != 0) {
    FUN_08036440(param_1,param_2);
    iVar2 = FUN_0802ee0a(param_2);
    while (iVar5 = iVar2, iVar5 != 0) {
      bVar1 = false;
      uVar3 = FUN_0802ebf4(iVar5);
      for (uVar4 = 0; uVar4 < uVar3; uVar4 = uVar4 + 1) {
        if (bVar1) {
          FUN_08036440(param_1);
        }
        else if (*(int *)(**(int **)(iVar5 + 8) + uVar4 * 4) == param_2) {
          bVar1 = true;
        }
      }
      FUN_0802ad74(iVar5,0x17,param_1);
      FUN_0802ad74(iVar5,0x18,param_1);
      FUN_0802ad74(iVar5,0x19,param_1);
      iVar2 = FUN_0802ee0a(iVar5);
      param_2 = iVar5;
    }
  }
  return;
}

