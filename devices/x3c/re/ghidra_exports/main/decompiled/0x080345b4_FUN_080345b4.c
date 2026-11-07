// 0x080345b4 FUN_080345b4 size=1104

void FUN_080345b4(int param_1)

{
  int iVar1;
  char *pcVar2;
  undefined2 uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  undefined2 uVar14;
  undefined4 uVar13;
  
  iVar1 = DAT_080349b8;
  *(undefined1 *)(DAT_080349b8 + 7) = 0;
  *(undefined1 *)(iVar1 + 0xf) = 0;
  *DAT_080349bc = 0;
  *DAT_080349c0 = 0;
  iVar8 = DAT_080349c4;
  uVar9 = 0;
  iVar11 = DAT_080349c4 + 0x40;
  iVar12 = DAT_080349c4 + 0x80;
  iVar10 = DAT_080349c4 + 0x120;
  do {
    *(undefined4 *)(iVar8 + uVar9 * 4) = 0;
    *(undefined4 *)(iVar11 + uVar9 * 4) = 0;
    *(undefined4 *)(iVar12 + uVar9 * 4) = 0;
    *(undefined4 *)(iVar10 + uVar9 * 0x74) = 0;
    iVar4 = iVar10 + uVar9 * 0x74;
    *(undefined4 *)(iVar4 + 0x68) = 0;
    *(undefined4 *)(iVar4 + 0x38) = 0;
    *(undefined4 *)(iVar4 + 0x5c) = 0;
    *(undefined4 *)(iVar4 + 0x60) = 0;
    *(undefined4 *)(iVar4 + 0x3c) = 0;
    *(undefined4 *)(iVar4 + 100) = 0;
    *(undefined4 *)(iVar4 + 0x34) = 0;
    *(undefined4 *)(iVar4 + 0x30) = 0;
    *(undefined4 *)(iVar4 + 0x50) = 0;
    *(undefined4 *)(iVar4 + 0x2c) = 0;
    *(undefined4 *)(iVar4 + 0x54) = 0;
    *(undefined4 *)(iVar4 + 0x58) = 0;
    *(undefined4 *)(iVar4 + 0x44) = 0;
    *(undefined4 *)(iVar4 + 0x28) = 0;
    *(undefined4 *)(iVar4 + 0x24) = 0;
    *(undefined4 *)(iVar4 + 0x20) = 0;
    *(undefined4 *)(iVar4 + 0xc) = 0;
    *(undefined4 *)(iVar4 + 0x10) = 0;
    *(undefined4 *)(iVar4 + 0x14) = 0;
    *(undefined4 *)(iVar4 + 0x18) = 0;
    *(undefined4 *)(iVar4 + 8) = 0;
    *(undefined4 *)(iVar4 + 0x1c) = 0;
    *(undefined4 *)(iVar4 + 0x6c) = 0;
    *(undefined4 *)(iVar4 + 0x70) = 0;
    *(undefined4 *)(iVar4 + 4) = 0;
    *(undefined4 *)(iVar4 + 0x48) = 0;
    *(undefined4 *)(iVar4 + 0x40) = 0;
    *(undefined4 *)(iVar4 + 0x4c) = 0;
    uVar9 = uVar9 + 1 & 0xff;
  } while (uVar9 < 0x10);
  *(undefined4 *)(iVar1 + 0x34) = 0;
  *(undefined4 *)(iVar1 + 100) = 0;
  *(undefined4 *)(iVar1 + 0x68) = 0;
  *(undefined4 *)(iVar1 + 0x6c) = 0;
  *(undefined4 *)(iVar1 + 0x7c) = 0;
  iVar8 = DAT_080349b8;
  *(undefined4 *)(DAT_080349b8 + 0x84) = 0;
  *(undefined4 *)(iVar8 + 0x88) = 0;
  *(undefined4 *)(iVar1 + 0x38) = 0;
  *(undefined1 *)(iVar1 + 6) = 0;
  *(undefined1 *)(iVar1 + 0xf) = 0;
  FUN_0802b798(*(undefined4 *)(iVar8 + 0x94));
  *(undefined1 *)(iVar1 + 0xb) = 0;
  *(undefined1 *)(iVar1 + 8) = 0;
  uVar5 = FUN_08010acc(0,0,0,0,0x170,0x1c0);
  uVar6 = FUN_08027c88(0,0);
  FUN_0803157c(uVar5,uVar6,0);
  FUN_08031594(uVar5,0xff,0);
  *(undefined4 *)(iVar1 + 0x58) = uVar5;
  FUN_08010acc(uVar5,0,0,0,0x170,0x28);
  FUN_0800e168();
  uVar6 = FUN_08010acc(uVar5,0,0,0x27,0x170,0x198);
  FUN_0802e2d2(uVar6,0x10);
  FUN_08031330(uVar6,3);
  FUN_08031384(uVar6,0);
  *(undefined4 *)(iVar1 + 0x60) = uVar6;
  uVar6 = FUN_08010acc(uVar6,9,0,0,0x170,0x198);
  FUN_08031330(uVar6,3);
  FUN_0802d97a(uVar6,DAT_080349c8,10,0);
  FUN_080312a4(uVar6,0);
  FUN_08031350(uVar6,3);
  FUN_0802da22(uVar6,0x80);
  uVar7 = FUN_08010acc(uVar6,0,0,0x78,0x170,0x198);
  FUN_0800981c(uVar7);
  *(undefined4 *)(iVar1 + 0x74) = uVar7;
  FUN_0802da22(uVar6,0x80);
  uVar6 = FUN_08010acc(uVar6,1,0,0x32,0x170,0x198);
  *(undefined4 *)(iVar8 + 0x90) = uVar6;
  FUN_0802e2d2(uVar6,0x10);
  FUN_0802da22(*(undefined4 *)(iVar8 + 0x90),0x80);
  *(undefined4 *)(iVar1 + 0x5c) = *(undefined4 *)(iVar8 + 0x90);
  *DAT_080349cc = 0;
  uVar14 = 0;
  uVar6 = FUN_08006220(uVar5,1,4,200,4,0x5c);
  *(undefined4 *)(iVar1 + 0x78) = uVar6;
  FUN_08031696(uVar6,0xf,0);
  uVar6 = FUN_08027c88(0x46,0x46);
  FUN_0803157c(*(undefined4 *)(iVar1 + 0x78),uVar6,0);
  FUN_0802d97a(*(undefined4 *)(iVar1 + 0x78),DAT_080349d0,0);
  FUN_08031594(*(undefined4 *)(iVar1 + 0x78),0xff,0);
  FUN_0802da22(*(undefined4 *)(iVar1 + 0x78),1);
  FUN_0803161c(*(undefined4 *)(iVar1 + 0x78),0xff,4);
  FUN_08031626(*(undefined4 *)(iVar1 + 0x78),3,4);
  FUN_08031630(*(undefined4 *)(iVar1 + 0x78),2,4);
  uVar6 = FUN_08027f68();
  FUN_08031604(*(undefined4 *)(iVar1 + 0x78),uVar6,4);
  uVar3 = FUN_08027f68();
  uVar6 = DAT_080349d8;
  uVar13 = CONCAT22(uVar14,uVar3);
  uVar7 = FUN_0802553e(uVar5,1,0x98,0x1ae,DAT_080349d8,uVar13,&DAT_080349d4);
  uVar14 = (undefined2)((uint)uVar13 >> 0x10);
  *(undefined4 *)(iVar1 + 0x44) = uVar7;
  uVar3 = FUN_08027f68();
  uVar13 = CONCAT22(uVar14,uVar3);
  uVar7 = FUN_0802553e(uVar5,1,0xae,0x1b0,DAT_080349e0,uVar13,&DAT_080349dc);
  uVar14 = (undefined2)((uint)uVar13 >> 0x10);
  *(undefined4 *)(iVar1 + 0x48) = uVar7;
  uVar3 = FUN_08027f68();
  uVar6 = FUN_0802553e(uVar5,1,0xcc,0x1ae,uVar6,CONCAT22(uVar14,uVar3),&DAT_080349e4);
  *(undefined4 *)(iVar1 + 0x4c) = uVar6;
  FUN_080315fa(*(undefined4 *)(iVar1 + 0x44),0);
  FUN_080315fa(*(undefined4 *)(iVar1 + 0x48),0);
  FUN_080315fa(*(undefined4 *)(iVar1 + 0x4c),0);
  uVar6 = FUN_08010acc(uVar5,9,0,0,0x170,0x1c0);
  FUN_0802d97a(uVar6,DAT_080349e8,10,0);
  FUN_0802e2d2(uVar6,2);
  FUN_080312a4(uVar6,0);
  FUN_08031350(uVar6,3);
  *(undefined4 *)(iVar8 + 0x9c) = uVar6;
  uVar7 = FUN_08010acc(uVar6,0,0,0,0x170,0x1c0);
  *(undefined4 *)(iVar1 + 0x54) = uVar7;
  FUN_0802e2d2(uVar7,2);
  uVar6 = FUN_08010acc(uVar6,0,0,0,0x170,0x1c0);
  *(undefined4 *)(iVar1 + 0x50) = uVar6;
  uVar6 = FUN_08027a78();
  FUN_0803157c(*(undefined4 *)(iVar1 + 0x50),uVar6,0);
  FUN_08031594(*(undefined4 *)(iVar1 + 0x50),0xff,0);
  FUN_0803118e(*(undefined4 *)(iVar1 + 0x54),0);
  FUN_0802e2d2(*(undefined4 *)(iVar1 + 0x50),2);
  FUN_0802d97a(uVar5,DAT_080349ec,0);
  uVar14 = 0;
  uVar6 = FUN_08010acc(uVar5,1,0,0x28,0x170,0x198);
  FUN_0802e2d2(uVar6,0x10);
  uVar7 = FUN_08027c88(0,0);
  FUN_0803157c(uVar6,uVar7,0);
  FUN_08031594(uVar6,0xff,0);
  uVar3 = FUN_08027f68();
  uVar7 = CONCAT22(uVar14,uVar3);
  FUN_0802553e(uVar6,2,0,0x75,DAT_080349f4,uVar7,&DAT_080349f0);
  uVar14 = (undefined2)((uint)uVar7 >> 0x10);
  uVar3 = FUN_08027f68();
  FUN_0802553e(uVar6,2,0,0xea,DAT_08034a00,CONCAT22(uVar14,uVar3),s_L_858_080349f8);
  if (*DAT_08034a04 == '\0') {
    FUN_0802da22(uVar6,1);
  }
  else {
    FUN_0802e2d2(uVar6,1);
  }
  FUN_080079e8(uVar5);
  pcVar2 = DAT_08034a08;
  if (*DAT_08034a08 == '\0') {
    FUN_08032078(uVar5,6,200,param_1,1);
  }
  else {
    iVar8 = FUN_0803205a();
    FUN_0802832e(uVar5);
    if (iVar8 != 0) {
      FUN_0802e428(iVar8);
    }
    *pcVar2 = '\0';
  }
  if (param_1 == 0) {
    *DAT_08034a58 = 4;
  }
  else {
    *DAT_08034a58 = 6;
  }
  *(undefined1 *)(iVar1 + 9) = 1;
  return;
}

