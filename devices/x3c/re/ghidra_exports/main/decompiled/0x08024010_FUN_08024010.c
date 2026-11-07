// 0x08024010 FUN_08024010 size=402

undefined4 FUN_08024010(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined2 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined2 uVar6;
  
  uVar6 = 0;
  uVar2 = FUN_08010acc(0,9,0,0,0x170,0x1c0,param_3,param_4);
  uVar3 = FUN_08027af0();
  FUN_0803157c(uVar2,uVar3,0);
  FUN_08031594(uVar2,0xff,0);
  iVar5 = DAT_080241a8;
  if (*DAT_080241a4 == '\0') {
    uVar1 = FUN_08027ff4();
    uVar4 = CONCAT22(uVar6,uVar1);
    uVar3 = FUN_0802553e(uVar2,2,0,0x119,DAT_080241bc,uVar4,&DAT_080241ac);
    uVar6 = (undefined2)((uint)uVar4 >> 0x10);
    *(undefined4 *)(iVar5 + 0x10) = uVar3;
  }
  else if (*DAT_080241a4 == '\x01') {
    uVar1 = FUN_08027ff4();
    uVar4 = CONCAT22(uVar6,uVar1);
    uVar3 = FUN_0802553e(uVar2,2,0,0x119,DAT_080241d4,uVar4,s_Hot_Shoe_Upgrading_080241c0);
    uVar6 = (undefined2)((uint)uVar4 >> 0x10);
    *(undefined4 *)(iVar5 + 0x10) = uVar3;
  }
  uVar3 = FUN_08026bec(uVar2);
  *(undefined4 *)(iVar5 + 0xc) = uVar3;
  FUN_08027342(uVar3,0x10e);
  FUN_0802e2d2(*(undefined4 *)(iVar5 + 0xc),2);
  FUN_080271cc(*(undefined4 *)(iVar5 + 0xc),0,0x168);
  uVar3 = FUN_08027dbc(0x70,0x70);
  FUN_0803157c(*(undefined4 *)(iVar5 + 0xc),uVar3,0);
  FUN_08030e2a(*(undefined4 *)(iVar5 + 0xc),0,0x30000);
  FUN_0802db44(*(undefined4 *)(iVar5 + 0xc),2,0,0x53);
  FUN_080313b2(*(undefined4 *)(iVar5 + 0xc),0xbe);
  FUN_08032c54(DAT_080241a8 + 0x20);
  uVar3 = FUN_08027dbc(0x70,0x70);
  FUN_08032ed0(DAT_080241a8 + 0x20,uVar3);
  FUN_0802da9c(*(undefined4 *)(iVar5 + 0xc),DAT_080241a8 + 0x20,0);
  FUN_08032c54(DAT_080241a8 + 0x28);
  uVar3 = FUN_08027ff4();
  FUN_08032ed0(DAT_080241a8 + 0x28,uVar3);
  FUN_0802da9c(*(undefined4 *)(iVar5 + 0xc),DAT_080241a8 + 0x28,0x20000);
  uVar1 = FUN_08027ff4();
  uVar3 = DAT_080241dc;
  uVar4 = FUN_0802553e(uVar2,2,0,0x8c,DAT_080241dc,CONCAT22(uVar6,uVar1),&DAT_080241d8);
  *(undefined4 *)(iVar5 + 0x14) = uVar4;
  FUN_0802da22(uVar4,1);
  uVar4 = FUN_08033500(DAT_080241e0,0x1e,0);
  *(undefined4 *)(iVar5 + 0x18) = uVar4;
  iVar5 = FUN_08032068();
  FUN_0802832e(uVar2);
  if (iVar5 == 0) {
    return uVar3;
  }
  uVar2 = FUN_0802e428(iVar5);
  return uVar2;
}

