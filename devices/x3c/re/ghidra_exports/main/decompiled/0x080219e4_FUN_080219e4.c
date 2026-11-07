// 0x080219e4 FUN_080219e4 size=62

void FUN_080219e4(void)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  FUN_080218e8();
  iVar3 = DAT_08021a28 + *(int *)(DAT_08021a24 + 4) * 0x8a;
  FUN_08021a30(iVar3,DAT_08021a2c,0x8a);
  iVar1 = DAT_08021a2c;
  uVar2 = 0;
  do {
    *(short *)(iVar1 + uVar2 * 2) = (short)*(undefined4 *)(iVar3 + uVar2 * 2);
    uVar2 = uVar2 + 1 & 0xffff;
  } while (uVar2 < 0x45);
  FUN_080217b0();
  return;
}

