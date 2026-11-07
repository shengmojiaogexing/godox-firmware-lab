// 0x08013b80 FUN_08013b80 size=40

void FUN_08013b80(void)

{
  int iVar1;
  
  FUN_08006c90();
  iVar1 = DAT_08013ba8;
  disableIRQinterrupts();
  *(int *)(DAT_08013ba8 + 4) = *(int *)(DAT_08013ba8 + 4) + 1;
  if (0xd < *(uint *)(iVar1 + 4)) {
    *(undefined4 *)(iVar1 + 4) = 0;
    FUN_0802170c();
    *(undefined4 *)(iVar1 + 4) = 0;
  }
  FUN_080219e4();
  enableIRQinterrupts();
  return;
}

