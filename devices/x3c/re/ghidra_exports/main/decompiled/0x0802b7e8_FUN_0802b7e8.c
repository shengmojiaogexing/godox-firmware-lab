// 0x0802b7e8 FUN_0802b7e8 size=162

void FUN_0802b7e8(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  
  iVar1 = FUN_0802ed7a(param_1);
  if (iVar1 != 0) {
    if ((*(int **)(iVar1 + 0xc) != (int *)0x0) && (**(int **)(iVar1 + 0xc) == param_1)) {
      if ((*(byte *)(iVar1 + 0x1c) & 1) != 0) {
        *(byte *)(iVar1 + 0x1c) = *(byte *)(iVar1 + 0x1c) & 0xfe;
      }
      iVar2 = FUN_08017f30(iVar1);
      if ((iVar2 == *(int *)(iVar1 + 0xc)) &&
         (iVar2 = FUN_08017f70(iVar1), iVar2 == *(int *)(iVar1 + 0xc))) {
        uVar3 = FUN_08022204(iVar1);
        FUN_0802ad74(**(undefined4 **)(iVar1 + 0xc),0xf,uVar3);
      }
      else {
        FUN_0802b76c(iVar1);
      }
    }
    if ((*(int **)(iVar1 + 0xc) != (int *)0x0) && (**(int **)(iVar1 + 0xc) == param_1)) {
      *(undefined4 *)(iVar1 + 0xc) = 0;
    }
    for (piVar4 = (int *)FUN_08017f30(iVar1); piVar4 != (int *)0x0;
        piVar4 = (int *)FUN_08017f5e(iVar1,piVar4)) {
      if (*piVar4 == param_1) {
        FUN_0801801e(iVar1,piVar4);
        FUN_0802d6e4(piVar4);
        if (*(int *)(param_1 + 8) == 0) {
          return;
        }
        *(undefined4 *)(*(int *)(param_1 + 8) + 8) = 0;
        return;
      }
    }
  }
  return;
}

