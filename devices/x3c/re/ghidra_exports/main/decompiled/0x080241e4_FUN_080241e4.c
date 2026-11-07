// 0x080241e4 FUN_080241e4 size=110

void FUN_080241e4(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = *(int *)(param_1 + 0x84);
  if (iVar1 != 0) {
    iVar3 = *(int *)(iVar1 + *(int *)(param_2 + 0xc) * 8);
    iVar1 = *(int *)(iVar1 + *(int *)(param_2 + 0xc) * 8 + 4);
    if ((*(char *)(param_2 + 0x12) == '\x01') &&
       ((*(int *)(param_1 + 0x18) != iVar3 || (*(int *)(param_1 + 0x1c) != iVar1)))) {
      FUN_08024d38(param_1 + 4);
    }
    iVar2 = FUN_08024ea0(param_1 + 4);
    if (iVar2 == 0) {
      *(int *)(param_1 + 8) = iVar3;
      *(int *)(param_1 + 0xc) = iVar1;
      if (*(char *)(param_2 + 0x12) == '\x01') {
        FUN_08024b1c(param_1 + 4);
      }
      else {
        FUN_08024d38(param_1 + 4);
      }
      iVar1 = FUN_08024ea0(param_1 + 4);
      if (iVar1 == 0) {
        *(undefined4 *)(param_1 + 0x18) = *(undefined4 *)(param_1 + 8);
        *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(param_1 + 0xc);
      }
    }
  }
  return;
}

