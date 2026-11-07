// 0x0802b798 FUN_0802b798 size=126

void FUN_0802b798(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  
  if (*(int *)(param_1 + 0xc) != 0) {
    uVar3 = FUN_08022204(param_1);
    FUN_0802ad74(**(undefined4 **)(param_1 + 0xc),0xf,uVar3);
    FUN_080302d0(**(undefined4 **)(param_1 + 0xc));
    *(undefined4 *)(param_1 + 0xc) = 0;
  }
  piVar4 = (int *)FUN_08017f30(param_1);
  while (piVar4 != (int *)0x0) {
    if (*(int *)(*piVar4 + 8) != 0) {
      *(undefined4 *)(*(int *)(*piVar4 + 8) + 8) = 0;
    }
    piVar4 = (int *)FUN_08017f5e(param_1);
  }
  iVar1 = FUN_08017f30(param_1);
  while (iVar1 != 0) {
    iVar2 = FUN_08017f5e(param_1,iVar1);
    FUN_0801801e(param_1,iVar1);
    FUN_0802d6e4(iVar1);
    iVar1 = iVar2;
  }
  return;
}

