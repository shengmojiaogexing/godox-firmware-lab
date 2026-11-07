// 0x0801b8a0 FUN_0801b8a0 size=44

void FUN_0801b8a0(int param_1)

{
  int iVar1;
  int iVar2;
  
  *(int *)(param_1 + 8) = param_1;
  *(int *)(param_1 + 0xc) = param_1;
  *(undefined4 *)(param_1 + 0x10) = 0;
  iVar2 = 0;
  do {
    *(undefined4 *)(param_1 + iVar2 * 4 + 0x14) = 0;
    iVar1 = 0;
    do {
      *(int *)(param_1 + iVar2 * 0x80 + iVar1 * 4 + 0x40) = param_1;
      iVar1 = iVar1 + 1;
    } while (iVar1 < 0x20);
    iVar2 = iVar2 + 1;
  } while (iVar2 < 0xb);
  return;
}

