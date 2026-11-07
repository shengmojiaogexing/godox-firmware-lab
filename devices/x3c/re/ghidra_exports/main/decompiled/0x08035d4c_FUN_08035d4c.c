// 0x08035d4c FUN_08035d4c size=292

void FUN_08035d4c(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  int *piVar6;
  
  iVar1 = FUN_0802ad74(param_1,0x21,0);
  if (iVar1 == 0) {
    return;
  }
  iVar1 = FUN_0802ebc8(param_1,0);
  while (iVar1 != 0) {
    FUN_08035d4c();
    iVar1 = FUN_0802ebc8(param_1,0);
  }
  iVar1 = FUN_0802ed7a(param_1);
  for (iVar2 = FUN_0802c610(0); iVar2 != 0; iVar2 = FUN_0802c610(iVar2)) {
    if ((*(int *)(iVar2 + 0x48) == param_1) || (*(int *)(iVar2 + 0x4c) == param_1)) {
      FUN_0802c7cc(iVar2,param_1);
    }
    if (*(int *)(iVar2 + 0x54) == param_1) {
      *(undefined4 *)(iVar2 + 0x54) = 0;
    }
    if ((*(int *)(iVar2 + 0x80) == iVar1) && (iVar3 = FUN_0802c628(), iVar3 == param_1)) {
      FUN_0802c7cc(iVar2,param_1);
    }
  }
  FUN_080180a6(param_1);
  if (*(int *)(param_1 + 4) == 0) {
    iVar1 = FUN_0802ed10(param_1);
    for (uVar4 = 0;
        (uVar4 < *(uint *)(iVar1 + 0x24) && (*(int *)(*(int *)(iVar1 + 0xc) + uVar4 * 4) != param_1)
        ); uVar4 = uVar4 + 1) {
    }
    for (; uVar4 < *(int *)(iVar1 + 0x24) - 1U; uVar4 = uVar4 + 1) {
      *(undefined4 *)(*(int *)(iVar1 + 0xc) + uVar4 * 4) =
           *(undefined4 *)(*(int *)(iVar1 + 0xc) + (uVar4 + 1) * 4);
    }
    iVar2 = *(int *)(iVar1 + 0x24) + -1;
    *(int *)(iVar1 + 0x24) = iVar2;
    uVar5 = FUN_0802d72c(*(undefined4 *)(iVar1 + 0xc),iVar2 * 4);
    *(undefined4 *)(iVar1 + 0xc) = uVar5;
  }
  else {
    for (uVar4 = FUN_0802ed92(param_1); piVar6 = *(int **)(*(int *)(param_1 + 4) + 8),
        uVar4 < piVar6[1] - 1U; uVar4 = uVar4 + 1) {
      *(undefined4 *)(*piVar6 + uVar4 * 4) = *(undefined4 *)(*piVar6 + (uVar4 + 1) * 4);
    }
    iVar1 = *(int *)(*(int *)(*(int *)(param_1 + 4) + 8) + 4) + -1;
    piVar6[1] = iVar1;
    uVar5 = FUN_0802d72c(**(undefined4 **)(*(int *)(param_1 + 4) + 8),iVar1 * 4);
    **(undefined4 **)(*(int *)(param_1 + 4) + 8) = uVar5;
  }
  FUN_0802d6e4(param_1);
  return;
}

