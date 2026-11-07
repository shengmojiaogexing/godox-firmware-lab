// 0x0802354c FUN_0802354c size=556

uint FUN_0802354c(void)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  
  uVar1 = DAT_08023778;
  uVar3 = 0;
  FUN_08022e38(DAT_08023778,1);
  iVar2 = FUN_08022eb4(uVar1,0x8000);
  if (iVar2 == 1) {
    uVar3 = 0x80;
  }
  FUN_08022e34(uVar1,1);
  FUN_08022e38(uVar1,1);
  iVar2 = FUN_08022eb4(uVar1,0x8000);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 0x40;
  }
  FUN_08022e34(uVar1,1);
  FUN_08022e38(uVar1,1);
  iVar2 = FUN_08022eb4(uVar1,0x8000);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 0x20;
  }
  FUN_08022e34(uVar1,1);
  FUN_08022e38(uVar1,1);
  iVar2 = FUN_08022eb4(uVar1,0x8000);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 0x10;
  }
  FUN_08022e34(uVar1,1);
  FUN_08022e38(uVar1,1);
  iVar2 = FUN_08022eb4(uVar1,0x8000);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 8;
  }
  FUN_08022e34(uVar1,1);
  FUN_08022e38(uVar1,1);
  iVar2 = FUN_08022eb4(uVar1,0x8000);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 4;
  }
  FUN_08022e34(uVar1,1);
  FUN_08022e38(uVar1,1);
  iVar2 = FUN_08022eb4(uVar1,0x8000);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 2;
  }
  FUN_08022e34(uVar1,1);
  FUN_08022e38(uVar1,1);
  iVar2 = FUN_08022eb4(uVar1,0x8000);
  if (iVar2 == 1) {
    uVar3 = uVar3 | 1;
  }
  FUN_08022e34(uVar1,1);
  return uVar3;
}

