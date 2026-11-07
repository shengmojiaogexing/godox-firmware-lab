// 0x080067e8 FUN_080067e8 size=250

void FUN_080067e8(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 in_r3;
  
  uVar2 = FUN_08010acc(0,9,0,0,0x170,0x1c0,in_r3);
  uVar3 = FUN_08027af0();
  FUN_0803157c(uVar2,uVar3,0);
  FUN_08031594(uVar2,0xff,0);
  uVar3 = FUN_0802b9c4(uVar2);
  FUN_0802c400(uVar3,DAT_080068e4);
  FUN_0802db44(uVar3,9,0);
  uVar3 = FUN_08038042(uVar2,9,0,0xd,3,0x96,0x137);
  iVar1 = DAT_080068e8;
  *(undefined4 *)(DAT_080068e8 + 4) = uVar3;
  FUN_08031696(uVar3,0xf,0);
  FUN_080315fa(*(undefined4 *)(iVar1 + 4),0xff,0);
  FUN_080315fa(*(undefined4 *)(iVar1 + 4),0xff,0x20000);
  uVar3 = FUN_08027dbc(0x2a,0xe2,0x46);
  FUN_0803157c(*(undefined4 *)(iVar1 + 4),uVar3,0x20000);
  uVar3 = FUN_08027dbc(0x40,0x40);
  FUN_0803157c(*(undefined4 *)(iVar1 + 4),uVar3,0);
  FUN_08031594(*(undefined4 *)(iVar1 + 4),0xff,0);
  FUN_0802e2d2(*(undefined4 *)(iVar1 + 4),2);
  uVar3 = FUN_0802b9c4(uVar2);
  FUN_0802c400(uVar3,DAT_080068ec);
  FUN_0802e128(uVar3);
  FUN_0802832e(uVar2);
  uVar2 = FUN_08033500(DAT_080068f0,1000,0);
  *(undefined4 *)(iVar1 + 8) = uVar2;
  return;
}

