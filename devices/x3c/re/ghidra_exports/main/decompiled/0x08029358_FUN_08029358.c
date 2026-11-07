// 0x08029358 FUN_08029358 size=60

void FUN_08029358(int param_1)

{
  int iVar1;
  
  if (*(char *)(param_1 + 4) == '\x02') {
    iVar1 = *(int *)(param_1 + 0x20);
    if (iVar1 != 0) {
      if (*(int *)(iVar1 + 0x10) < 0) {
        FUN_0802d6e4(*(undefined4 *)(iVar1 + 4));
        FUN_0802d6e4(*(undefined4 *)(param_1 + 0x20));
        return;
      }
      *(int *)(iVar1 + 0x14) = *(int *)(iVar1 + 0x14) + -1;
    }
  }
  else if (*(char *)(param_1 + 4) == '\x05') {
    FUN_0802d6e4(*(undefined4 *)(param_1 + 8));
    return;
  }
  return;
}

