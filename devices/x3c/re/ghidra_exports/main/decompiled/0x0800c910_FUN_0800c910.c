// 0x0800c910 FUN_0800c910 size=160

uint FUN_0800c910(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  byte bVar5;
  
  uVar1 = DAT_0800c9b0;
  uVar4 = 0;
  FUN_08022e34(DAT_0800c9b0,0x80);
  uVar2 = DAT_0800c9b4;
  FUN_080147f4(DAT_0800c9b4,2,0,1);
  FUN_0801c118(2);
  FUN_0801c118(2);
  bVar5 = 0;
  do {
    FUN_08022e38(uVar1,0x80);
    FUN_0801c118(2);
    FUN_0801c118(2);
    uVar4 = (uVar4 & 0x7f) << 1;
    iVar3 = FUN_08022eb4(uVar2,2);
    if (iVar3 != 0) {
      uVar4 = uVar4 | 1;
    }
    FUN_0801c118(2);
    FUN_0801c118(2);
    FUN_08022e34(uVar1,0x80);
    FUN_0801c118(2);
    FUN_0801c118(2);
    bVar5 = bVar5 + 1;
  } while (bVar5 < 8);
  FUN_0801c118(2);
  FUN_0801c118(2);
  FUN_0800c880(param_1);
  FUN_0801c118(2);
  FUN_0801c118(2);
  return uVar4;
}

