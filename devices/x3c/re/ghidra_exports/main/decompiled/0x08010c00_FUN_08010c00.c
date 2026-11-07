// 0x08010c00 FUN_08010c00 size=128

void FUN_08010c00(void)

{
  short sVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  
  FUN_08013b80();
  FUN_080393c4();
  FUN_0801bd04(DAT_08010c80,1);
  disableIRQinterrupts();
  FUN_08036010(1);
  FUN_0801bac0(1);
  FUN_0801bac0(0);
  FUN_080202bc(0);
  enableIRQinterrupts();
  FUN_08013cdc();
  FUN_08037470();
  uVar3 = DAT_08010c88;
  iVar2 = DAT_08010c84;
  do {
    iVar4 = FUN_08022eb4(uVar3,0x100);
    if (iVar4 != 0) break;
    FUN_0803ba4c();
    FUN_0801c0fe(1);
    sVar1 = *(short *)(iVar2 + 0x10);
  } while ((sVar1 == 0) || (*(short *)(iVar2 + 0x10) = sVar1 + -1, sVar1 != 1));
  FUN_0801c0fe(0x14);
  FUN_08036084(0x100,1);
  FUN_0803604c();
  FUN_0801c118(0x78);
  return;
}

