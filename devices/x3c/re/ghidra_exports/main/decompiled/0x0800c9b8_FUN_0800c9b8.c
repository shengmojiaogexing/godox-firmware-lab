// 0x0800c9b8 FUN_0800c9b8 size=302

undefined4 FUN_0800c9b8(uint param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  byte bVar4;
  ushort uVar5;
  undefined4 unaff_r4;
  undefined4 unaff_r5;
  undefined4 unaff_r6;
  undefined4 unaff_lr;
  
  uVar1 = DAT_0800ca5c;
  FUN_080147f4(DAT_0800ca5c,2,1,0);
  uVar2 = DAT_0800ca60;
  FUN_08022e34(DAT_0800ca60,0x80);
  FUN_0801c118(2);
  FUN_0801c118(2);
  bVar4 = 0;
  do {
    if ((int)(param_1 << 0x18) < 0) {
      FUN_08022e38(uVar1,2);
    }
    else {
      FUN_08022e34(uVar1,2);
    }
    param_1 = (param_1 & 0x7f) << 1;
    FUN_0801c118(1);
    FUN_0801c118(1);
    FUN_08022e38(uVar2,0x80);
    FUN_0801c118(2);
    FUN_0801c118(2);
    FUN_08022e34(uVar2,0x80);
    FUN_0801c118(1);
    FUN_0801c118(1);
    bVar4 = bVar4 + 1;
  } while (bVar4 < 8);
  FUN_0801c118(1);
  FUN_0801c118(1);
  FUN_0801c118(1);
  FUN_0801c118(1);
  uVar1 = DAT_0800cba0;
  uVar5 = 0;
  FUN_08022e38(DAT_0800cba0,2);
  FUN_080147f4(uVar1,2,0,1,unaff_r4,unaff_r5,unaff_r6,unaff_lr);
  uVar2 = DAT_0800cba4;
  FUN_08022e38(DAT_0800cba4,0x80);
  do {
    FUN_0801c118(1);
    FUN_0801c118(1);
    uVar5 = uVar5 + 1;
    if (100 < uVar5) {
      return 1;
    }
    iVar3 = FUN_08022eb4(uVar1,2);
  } while (iVar3 != 0);
  FUN_0801c118(1);
  FUN_0801c118(1);
  FUN_08022e34(uVar2,0x80);
  FUN_0801c118(1);
  FUN_0801c118(1);
  FUN_08022e38(uVar1,2);
  FUN_080147f4(uVar1,2,1,0);
  FUN_0801c118(1);
  FUN_0801c118(1);
  return 0;
}

