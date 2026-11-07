// 0x0800d504 FUN_0800d504 size=3000

void FUN_0800d504(undefined4 param_1,uint param_2)

{
  char cVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  undefined4 uVar22;
  undefined4 uVar23;
  undefined4 uVar24;
  undefined4 uVar25;
  undefined4 uVar26;
  undefined4 uVar27;
  undefined4 uVar28;
  undefined4 uVar29;
  undefined4 uVar30;
  uint uVar31;
  int iVar32;
  undefined4 *puVar33;
  undefined1 auStack_38 [12];
  undefined4 local_2c;
  uint uStack_28;
  
  iVar32 = DAT_0800d90c;
  uVar31 = 0;
  do {
    *(undefined4 *)(iVar32 + uVar31 * 0x74) = 0;
    iVar4 = iVar32 + uVar31 * 0x74;
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
    uVar31 = uVar31 + 1 & 0xff;
  } while (uVar31 < 0x10);
  local_2c = param_1;
  uStack_28 = param_2;
  FUN_0802e2d2(param_1,0x300);
  uVar5 = FUN_08010acc(local_2c,0,4,0,0x168,0xb2);
  uVar6 = FUN_08027a78();
  FUN_0803157c(uVar5,uVar6,0);
  FUN_08031594(uVar5,0xff,0);
  uVar7 = FUN_08010acc(uVar5,0,0,0,0x164,0x94);
  FUN_08031696(uVar7,0xf,0);
  FUN_080315c0(uVar7,2,0);
  uVar6 = FUN_08027f68();
  FUN_0803159e(uVar7,uVar6,4);
  FUN_080315b6(uVar7,0);
  uVar6 = FUN_08027c88(0xfd,0x9f,10);
  FUN_0803159e(uVar7,uVar6,8);
  FUN_080315b6(uVar7,0xff,4);
  FUN_080315b6(uVar7,0xff,8);
  uVar8 = FUN_08010acc(uVar5,1,0xe,0x78,0x148,10);
  FUN_0802e2d2(uVar8,2);
  uVar6 = FUN_08027c88(0x27,0x27);
  FUN_0803157c(uVar8,uVar6,0);
  FUN_08031594(uVar8,0xff,0);
  FUN_08031696(uVar8,8,0);
  FUN_0802d97a(uVar7,DAT_0800d910,0,local_2c);
  uVar3 = 0;
  uVar9 = FUN_08010acc(uVar5,1,0xe,0x79,0,8);
  uVar6 = FUN_08027f68();
  FUN_0803157c(uVar9,uVar6,0);
  FUN_08031594(uVar9,0xff,0);
  FUN_08031696(uVar9,8,0);
  FUN_080315b6(uVar5,0xff,2);
  FUN_0802e2d2(uVar9,2);
  uVar2 = FUN_08027f68();
  uVar6 = DAT_0800d918;
  uVar14 = CONCAT22(uVar3,uVar2);
  uVar10 = FUN_0802553e(uVar5,1,6,0x4e,DAT_0800d918,uVar14,&DAT_0800d914);
  uVar3 = (undefined2)((uint)uVar14 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar14 = CONCAT22(uVar3,uVar2);
  uVar11 = FUN_0802553e(uVar5,3,0xfffffffa,0x4e,uVar6,uVar14,&DAT_0800d91c);
  uVar3 = (undefined2)((uint)uVar14 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar6 = DAT_0800d924;
  uVar14 = CONCAT22(uVar3,uVar2);
  uVar12 = FUN_0802553e(uVar5,1,0x40,0x1e,DAT_0800d924,uVar14,&DAT_0800d920);
  uVar3 = (undefined2)((uint)uVar14 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar14 = DAT_0800d92c;
  uVar15 = CONCAT22(uVar3,uVar2);
  uVar13 = FUN_0802553e(uVar5,2,0xffffffea,10,DAT_0800d92c,uVar15,&DAT_0800d928);
  uVar3 = (undefined2)((uint)uVar15 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar15 = CONCAT22(uVar3,uVar2);
  uVar6 = FUN_0802553e(uVar5,1,199,0x1e,uVar6,uVar15,&DAT_0800d930);
  uVar3 = (undefined2)((uint)uVar15 >> 0x10);
  FUN_0802db62(uVar12,uVar13,0x11,0xfffffff7,0xd);
  FUN_0802db62(uVar6,uVar13,0x14,9,0xb);
  uVar2 = FUN_08027c88(0x2a,0xe2,0x46);
  uVar14 = FUN_0802553e(uVar5,1,0x6d,10,uVar14,CONCAT22(uVar3,uVar2),&DAT_0800d930);
  uVar15 = FUN_08015aa8(uVar5,9,0,0,0x8c,0x15c,0xa4);
  FUN_0802d97a(uVar15,DAT_0800d938,0,uVar9);
  uVar2 = 0;
  uVar16 = FUN_08010acc(local_2c,1,0xe,0xa2,0x2c,0x58);
  FUN_08031594(uVar16,0xff,0);
  uVar17 = FUN_08027c88(0x52,0x52);
  FUN_0803157c(uVar16,uVar17,0);
  FUN_08031696(uVar16,0x16,0);
  if (param_2 < 6) {
    FUN_08004c48(auStack_38,10,&DAT_0800d93c,param_2 + 0x41);
  }
  else {
    FUN_08004c48(auStack_38,10,&DAT_0800d93c,param_2 + 0x2a);
  }
  uVar3 = FUN_08027f68();
  uVar17 = FUN_0802553e(uVar16,9,0,0xfffffff8,DAT_0800d940,CONCAT22(uVar2,uVar3),auStack_38);
  uVar16 = FUN_08010acc(uVar16,2,0,0x3d,0x1c,9);
  FUN_08031696(uVar16,6,0);
  if (*DAT_0800d944 == '\0') {
    FUN_0802db44(uVar17,9,0);
  }
  else {
    switch(param_2) {
    case 0:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0xff,0);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 1:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0,0xff);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 2:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0,0,0xff);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 3:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0,0xff);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 4:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0xff,0);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 5:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0xff,0xff,0);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 6:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0xff,0x66,0);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 7:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0,0xc1,0x77);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 8:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0x80,0,0xff);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 9:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0xff,0,0x74);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 10:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0xd8,0xb7,0xff);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 0xb:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0,0xff,0xce);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 0xc:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0,0x99,0xff);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 0xd:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0xc9,0xff,0xad);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 0xe:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0xff,0x80);
      FUN_0803157c(uVar16,uVar17,0);
      break;
    case 0xf:
      FUN_08031594(uVar16,0xff,0);
      uVar17 = FUN_08027c88(0xff,0xb7,0x80);
      FUN_0803157c(uVar16,uVar17,0);
    }
  }
  uVar3 = 0;
  uVar16 = FUN_08006220(local_2c,1,0x3e,0xa2,200,0x58);
  uVar17 = FUN_08027c88(0x27,0x27);
  FUN_0803157c(uVar16,uVar17,0);
  FUN_08031594(uVar16,0xff,0);
  FUN_0802d97a(uVar16,DAT_0800df48,0,local_2c);
  uVar2 = FUN_08027f68();
  uVar17 = FUN_0802553e(uVar16,9,0,2,DAT_0800df50,CONCAT22(uVar3,uVar2),&DAT_0800df4c);
  uVar3 = 0;
  uVar18 = FUN_08006220(local_2c,1,0x10a,0xa2,0x58,0x58);
  uVar19 = FUN_08027c88(0x27,0x27);
  FUN_0803157c(uVar18,uVar19,0);
  FUN_08031594(uVar18,0xff,0);
  FUN_0802d97a(uVar18,DAT_0800df54,0,local_2c);
  uVar2 = FUN_08027f68();
  FUN_0802553e(uVar18,9,0,0,DAT_0800df5c,CONCAT22(uVar3,uVar2),&DAT_0800df58);
  uVar19 = FUN_08010acc(local_2c,2,0,0xf8,0x154,0x80);
  FUN_0802e2d2(uVar19,0x10);
  uVar3 = 0;
  uVar20 = FUN_08006220(uVar19,7,0,0xfffffff6,0x58,0x58);
  uVar21 = FUN_08027c88(0x27,0x27);
  FUN_0803157c(uVar20,uVar21,0);
  FUN_08031594(uVar20,0xff,0);
  FUN_08031696(uVar20,0xf,0);
  FUN_0802d97a(uVar20,DAT_0800df60,0,local_2c);
  uVar2 = FUN_08027f68();
  uVar22 = CONCAT22(uVar3,uVar2);
  uVar21 = FUN_0802553e(uVar20,9,0,0,DAT_0800df64,uVar22,&DAT_0800d91c);
  uVar3 = (undefined2)((uint)uVar22 >> 0x10);
  uVar2 = FUN_08027f68();
  uVar22 = FUN_0802553e(uVar20,2,0,0x39,DAT_0800df70,CONCAT22(uVar3,uVar2),&DAT_0800df68);
  FUN_0802da22(uVar22,1);
  uVar23 = FUN_08010acc(uVar19,7,0x60,0xfffffff6,0xf0,0x58);
  FUN_080315b6(uVar23,0xff,0);
  FUN_080315b6(uVar23,0);
  FUN_080315c0(uVar23,2,0);
  FUN_080315b6(uVar23,0xff,4);
  FUN_080315c0(uVar23,2,4);
  uVar24 = FUN_08027f68();
  FUN_0803159e(uVar23,uVar24,4);
  uVar24 = FUN_08027c88(0xfd,0x9f,10);
  FUN_0803159e(uVar23,uVar24,8);
  FUN_08031696(uVar23,0xf,0);
  FUN_0802d97a(uVar23,DAT_0800df74,0,local_2c);
  uVar3 = 0;
  uVar19 = FUN_08010acc(uVar19,7,0x52,0,0x10c,0x80);
  FUN_08031594(uVar19,0);
  FUN_08031696(uVar19,0xf,0);
  uVar2 = FUN_08027f68();
  uVar24 = CONCAT22(uVar3,uVar2);
  FUN_0802553e(uVar19,1,0x28,0x28,DAT_0800df78,uVar24,&DAT_0800d914);
  uVar3 = (undefined2)((uint)uVar24 >> 0x10);
  uVar2 = FUN_08027f68();
  FUN_0802553e(uVar19,1,0xd2,0x1b,DAT_0800df80,CONCAT22(uVar3,uVar2),&DAT_0800df7c);
  uVar3 = 0;
  uVar24 = FUN_08010acc(uVar19,2,0,0x13,0x3c,0x28);
  uVar2 = FUN_08027f68();
  uVar24 = FUN_0802553e(uVar24,9,0,5,DAT_0800df80,CONCAT22(uVar3,uVar2),&DAT_0800df84);
  uVar25 = FUN_08015aa8(uVar19,9,0,0,0x5a,0xdc,0x7c);
  FUN_0802d97a(uVar25,DAT_0800df88,0,local_2c);
  FUN_0802da22(uVar25,0x2000);
  uVar26 = FUN_08010acc(uVar19,2,0,0x44,0xdc,8);
  uVar27 = FUN_08027c88(0x3b,0x3b);
  FUN_0803157c(uVar26,uVar27,0);
  FUN_08031594(uVar26,0xff,0);
  FUN_08031696(uVar26,8,0);
  FUN_0802e2d2(uVar26,2);
  uVar27 = FUN_08010acc(uVar26,7,0,0,0,8);
  uVar28 = FUN_08027c88(0xfb,0xfb);
  FUN_0803157c(uVar27,uVar28,0);
  FUN_08031594(uVar27,0xff,0);
  FUN_08031696(uVar27,8,0);
  FUN_0802e2d2(uVar27,2);
  uVar28 = FUN_08010acc(local_2c,2,0,0,0x168,0x170);
  FUN_0802e2d2(uVar28,0x10);
  uVar29 = FUN_08010acc(uVar28,2,0,0,0x168,0x9e);
  uVar30 = FUN_08027a78();
  FUN_0803157c(uVar29,uVar30,0);
  FUN_08031594(uVar29,0xcc,0);
  uVar29 = FUN_08010acc(uVar28,1,0x3a,0x9e,0x12e,0x5c);
  uVar30 = FUN_08027a78();
  FUN_0803157c(uVar29,uVar30,0);
  FUN_08031594(uVar29,0xcc,0);
  uVar29 = FUN_08010acc(uVar28,2,0,0xfa,0x168,0x7a);
  uVar30 = FUN_08027a78();
  FUN_0803157c(uVar29,uVar30,0);
  FUN_08031594(uVar29,0xcc,0);
  uVar3 = 0;
  uVar29 = FUN_08006220(uVar28,9,0,0,0x50,0x50);
  uVar30 = FUN_08027c88(0x6b,0x6b);
  FUN_0803157c(uVar29,uVar30,0);
  FUN_08031594(uVar29,0);
  FUN_0802e2d2(uVar29,0x400);
  FUN_0802d97a(uVar29,DAT_0800e150,0,local_2c);
  uVar2 = FUN_08027f68();
  FUN_0802553e(uVar29,9,0,0xfffffff8,DAT_0800e154,CONCAT22(uVar3,uVar2),&DAT_0800df58);
  *(undefined4 *)(iVar32 + param_2 * 0x74) = local_2c;
  iVar32 = iVar32 + param_2 * 0x74;
  *(undefined4 *)(iVar32 + 4) = uVar7;
  *(undefined4 *)(iVar32 + 0x24) = uVar8;
  *(undefined4 *)(iVar32 + 0x20) = uVar5;
  *(undefined4 *)(iVar32 + 0x10) = uVar12;
  *(undefined4 *)(iVar32 + 0x14) = uVar13;
  *(undefined4 *)(iVar32 + 0x18) = uVar6;
  *(undefined4 *)(iVar32 + 0x1c) = uVar14;
  *(undefined4 *)(iVar32 + 8) = uVar15;
  *(undefined4 *)(iVar32 + 0x28) = uVar17;
  *(undefined4 *)(iVar32 + 0xc) = uVar9;
  *(undefined4 *)(iVar32 + 0x38) = uVar27;
  *(undefined4 *)(iVar32 + 0x3c) = uVar24;
  *(undefined4 *)(iVar32 + 0x34) = uVar25;
  *(undefined4 *)(iVar32 + 0x40) = uVar28;
  *(undefined4 *)(iVar32 + 0x44) = uVar16;
  *(undefined4 *)(iVar32 + 0x4c) = uVar18;
  *(undefined4 *)(iVar32 + 0x48) = uVar29;
  *(undefined4 *)(iVar32 + 0x50) = uVar20;
  *(undefined4 *)(iVar32 + 0x5c) = uVar26;
  *(undefined4 *)(iVar32 + 0x6c) = uVar11;
  *(undefined4 *)(iVar32 + 0x70) = uVar10;
  *(undefined4 *)(iVar32 + 0x68) = uVar19;
  *(undefined4 *)(iVar32 + 0x2c) = uVar22;
  *(undefined4 *)(iVar32 + 0x30) = uVar21;
  *(undefined4 *)(iVar32 + 0x60) = uVar19;
  *(undefined4 *)(iVar32 + 100) = uVar23;
  iVar4 = DAT_0800e158;
  puVar33 = (undefined4 *)(DAT_0800e158 + param_2 * 6);
  FUN_08007524(*puVar33,*(undefined2 *)(puVar33 + 1),uVar15,uVar9,uVar14,uVar12,uVar13,uVar6);
  cVar1 = *(char *)(iVar4 + param_2 * 6);
  if (cVar1 == '\0') {
    iVar4 = FUN_080303b8(*(undefined4 *)(iVar32 + 0x28));
    if (iVar4 != 0) {
      FUN_0802d33c(*(undefined4 *)(iVar32 + 0x28),&DAT_0800df4c);
      uVar6 = FUN_08027f68();
      FUN_080316b4(*(undefined4 *)(iVar32 + 0x28),uVar6,0);
    }
  }
  else if ((cVar1 == '\x01') && (iVar4 = FUN_080303b8(*(undefined4 *)(iVar32 + 0x28)), iVar4 != 0))
  {
    FUN_0802d33c(*(undefined4 *)(iVar32 + 0x28),&DAT_0800e15c);
    uVar6 = FUN_08027c88(0x2a,0xe2,0x46);
    FUN_080316b4(*(undefined4 *)(iVar32 + 0x28),uVar6,0);
  }
  if ((int)((uint)*(byte *)((int)puVar33 + 3) << 0x19) < 0) {
    FUN_0802da22(*(undefined4 *)(iVar32 + 0x40),1);
  }
  else {
    FUN_0802e2d2(*(undefined4 *)(iVar32 + 0x40),1);
  }
  FUN_0800d018(param_2);
  if ((param_2 < 5) && (*DAT_0800e160 == '\0')) {
    if ((2 < param_2) && (*DAT_0800e164 == '\x01')) {
      uVar6 = FUN_08027c88(0x4a,0x4a);
      FUN_080316b4(uVar17,uVar6,0);
    }
  }
  else {
    uVar6 = FUN_08027c88(0x4a,0x4a);
    FUN_080316b4(uVar17,uVar6,0);
  }
  return;
}

