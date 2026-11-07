// 0x08036c54 FUN_08036c54 size=140

void FUN_08036c54(void)

{
  int iVar1;
  
  FUN_0801bda0();
  FUN_0801bd04(DAT_08036ce0,1);
  *(uint *)(DAT_08036ce4 + 0x10) = (*(uint *)(DAT_08036ce4 + 0x10) & 0xfffffff8) + 1;
  *DAT_08036ce8 = (*DAT_08036ce8 & 0xfffffffc) + 1;
  FUN_0801bad0(1);
  do {
    iVar1 = FUN_0801bcac();
  } while (iVar1 == 0);
  FUN_0801bd64(1,0x48,1,2);
  FUN_0801bad0(2,1);
  do {
    iVar1 = FUN_0801bc88(0x19);
  } while (iVar1 != 1);
  FUN_0801ba70(0);
  FUN_0801ba90(4);
  FUN_0801ba80(4);
  FUN_0801baa0(1);
  FUN_0801bdf8(2);
  do {
    iVar1 = FUN_0801be08();
  } while (iVar1 != 2);
  FUN_0801baa0(0);
  FUN_0803946c();
  *DAT_08036cec = 0;
  return;
}

