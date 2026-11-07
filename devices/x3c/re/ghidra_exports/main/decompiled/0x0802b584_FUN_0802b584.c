// 0x0802b584 FUN_0802b584 size=130

void FUN_0802b584(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
  if ((param_1 != 0) && (iVar1 = FUN_080303b8(param_2), iVar1 != 0)) {
    FUN_0802b7e8(param_2);
    piVar2 = (int *)FUN_08017f30(param_1);
    while (piVar2 != (int *)0x0) {
      if (*piVar2 == param_2) {
        return;
      }
      piVar2 = (int *)FUN_08017f5e(param_1);
    }
    iVar1 = FUN_0802ed7a(param_2);
    if (((iVar1 != 0) && (iVar1 = *(int *)(*(int *)(param_2 + 8) + 8), iVar1 != 0)) &&
       (**(int **)(iVar1 + 0xc) == param_2)) {
      FUN_0802b76c();
    }
    if (*(int *)(param_2 + 8) == 0) {
      FUN_0802dee0(param_2);
    }
    *(int *)(*(int *)(param_2 + 8) + 8) = param_1;
    piVar2 = (int *)FUN_08017fc8(param_1);
    if (piVar2 == (int *)0x0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    *piVar2 = param_2;
    piVar3 = (int *)FUN_08017f30(param_1);
    if (piVar3 == piVar2) {
      FUN_0802b76c(param_1);
      return;
    }
  }
  return;
}

