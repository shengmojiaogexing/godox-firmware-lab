// 0x0803a660 FUN_0803a660 size=30

void FUN_0803a660(int param_1)

{
  bool bVar1;
  uint uVar2;
  
  *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) | 0x10;
  uVar2 = 0;
  do {
    if (-1 < *(int *)(param_1 + 0x10) << 0x1b) {
      return;
    }
    bVar1 = uVar2 <= DAT_0803a680;
    uVar2 = uVar2 + 1;
  } while (bVar1);
  return;
}

