// 0x0802b9e0 FUN_0802b9e0 size=50

void FUN_0802b9e0(undefined4 param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_2 + 0x24);
  if (iVar1 != 0) {
    if (*(char *)(param_2 + 0x10) == '\x01') {
      FUN_0802afdc(iVar1);
    }
    if (*(int *)(iVar1 + 0xc) != 0) {
      FUN_0802d6e4();
    }
    if (*(int *)(iVar1 + 0x10) != 0) {
      FUN_0802d6e4();
    }
    FUN_0802d6e4(iVar1);
    *(undefined4 *)(param_2 + 0x24) = 0;
  }
  return;
}

