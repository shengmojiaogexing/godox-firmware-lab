// 0x0801be5c FUN_0801be5c size=172

uint FUN_0801be5c(void)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  
  uVar1 = DAT_0801bf08;
  iVar2 = FUN_08022eb4(DAT_0801bf08,0x20);
  uVar3 = (uint)(iVar2 == 1) << 1;
  iVar2 = FUN_08022eb4(uVar1,0x10);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 1;
  }
  uVar3 = uVar3 << 1;
  iVar2 = FUN_08022eb4(uVar1,8);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 1;
  }
  uVar3 = uVar3 << 1;
  iVar2 = FUN_08022eb4(uVar1,4);
  uVar1 = DAT_0801bf0c;
  if (iVar2 == 1) {
    uVar3 = uVar3 | 1;
  }
  uVar3 = uVar3 << 1;
  iVar2 = FUN_08022eb4(DAT_0801bf0c,0x1000);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 1;
  }
  uVar3 = uVar3 << 1;
  iVar2 = FUN_08022eb4(uVar1,0x800);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 1;
  }
  uVar3 = uVar3 << 1;
  iVar2 = FUN_08022eb4(uVar1,0x40);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 1;
  }
  uVar3 = uVar3 << 1;
  iVar2 = FUN_08022eb4(DAT_0801bf10,0x4000);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 1;
  }
  return uVar3;
}

