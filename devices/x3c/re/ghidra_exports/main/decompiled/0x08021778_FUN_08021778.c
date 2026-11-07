// 0x08021778 FUN_08021778 size=48

void FUN_08021778(int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = DAT_080217a8 + param_1 * 0x8a;
  FUN_080218e8();
  iVar1 = DAT_080217ac;
  uVar2 = 0;
  do {
    *(short *)(iVar1 + uVar2 * 2) = (short)*(undefined4 *)(iVar3 + uVar2 * 2);
    uVar2 = uVar2 + 1 & 0xffff;
  } while (uVar2 < 0x45);
  FUN_080217b0();
  return;
}

