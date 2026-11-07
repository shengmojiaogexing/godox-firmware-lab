// 0x080393c4 FUN_080393c4 size=150

void FUN_080393c4(void)

{
  uint *puVar1;
  int iVar2;
  
  FUN_0801bda0();
  FUN_0801bd04(DAT_0803945c,1);
  *(uint *)(DAT_08039460 + 0x10) = (*(uint *)(DAT_08039460 + 0x10) & 0xfffffff8) + 5;
  puVar1 = DAT_08039464;
  *DAT_08039464 = (*DAT_08039464 & 0xfffffffc) + 1;
  FUN_0801bad0(0,1);
  do {
    iVar2 = FUN_0801bc88(1);
  } while (iVar2 != 1);
  FUN_0801bd64(0,0x30,1,3);
  FUN_0801bad0(2,1);
  do {
    iVar2 = FUN_0801bc88(0x19);
  } while (iVar2 != 1);
  FUN_0801ba70(0);
  FUN_0801ba90(4);
  FUN_0801ba80(4);
  FUN_0801baa0(1);
  FUN_0801bdf8(2);
  do {
    iVar2 = FUN_0801be08();
  } while (iVar2 != 2);
  FUN_0801baa0(0);
  FUN_0803946c();
  *puVar1 = *puVar1 & 0xfffffffc;
  *DAT_08039468 = 1;
  return;
}

