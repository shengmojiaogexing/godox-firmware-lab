// 0x080079e8 FUN_080079e8 size=2102

void FUN_080079e8(undefined4 param_1)

{
  char *pcVar1;
  undefined4 *puVar2;
  undefined2 uVar3;
  undefined2 uVar4;
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
  undefined4 unaff_r4;
  undefined4 unaff_r10;
  
  uVar5 = FUN_08010acc(param_1,0,0,0,0x170,0x1c0);
  FUN_0802e2d2(uVar5,2);
  FUN_0802e2d2(uVar5,0x20);
  FUN_0802d97a(uVar5,DAT_08007e18,10,0);
  FUN_0803136a(uVar5,3);
  uVar6 = FUN_08010acc(uVar5,1,0,0xffffffce,0x170,0x224);
  FUN_0803163a(uVar6,0);
  FUN_08031696(uVar6,0xf,0);
  FUN_08031330(uVar6,0xc);
  uVar6 = FUN_08010acc(uVar5,0,0,0,0x170,0x1c0);
  uVar7 = FUN_08027d90(0x3a,0x3a);
  FUN_0803157c(uVar6,uVar7,0);
  FUN_08031594(uVar6,0xff,0);
  uVar7 = FUN_08010acc(uVar5,0,0,0x1f2,0x170,0x15c);
  FUN_0802e2d2(uVar7,2);
  FUN_0803118e(uVar7,0);
  *(undefined4 *)(DAT_08007e1c + 8) = uVar5;
  uVar8 = FUN_08010acc(uVar6,2,0,0xc,0x158,0x140);
  uVar9 = FUN_08027d90(0x12,0x12);
  FUN_0803157c(uVar8,uVar9,0);
  FUN_08031594(uVar8,0xff,0);
  FUN_08031696(uVar8,0x32,0);
  uVar4 = 0;
  uVar9 = FUN_08006220(uVar8,1,0x28,0x28,0x6c,0x6c);
  uVar10 = FUN_08027d90(0x27,0x27);
  FUN_0803157c(uVar9,uVar10,0);
  FUN_08031594(uVar9,0xff,0);
  FUN_0803161c(uVar9,0xff,4);
  uVar10 = FUN_08027fe0();
  FUN_08031604(uVar9,uVar10,4);
  FUN_08031630(uVar9,2,4);
  FUN_08031626(uVar9,4);
  FUN_080315b6(uVar9,0);
  FUN_080315c0(uVar9,2,0);
  FUN_080315b6(uVar9,0,4);
  FUN_080315c0(uVar9,2,4);
  uVar10 = FUN_08027fe0();
  FUN_0803159e(uVar9,uVar10,4);
  FUN_08031696(uVar9,0x6c,0);
  uVar3 = FUN_08027fe0();
  uVar10 = FUN_0802553e(uVar9,9,0,0,DAT_08007e24,CONCAT22(uVar4,uVar3),&DAT_08007e20);
  FUN_0802d97a(uVar9,DAT_08007e28,0);
  if (*DAT_08007e2c != '\0') {
    uVar11 = FUN_08027adc();
    FUN_080316b4(uVar10,uVar11,0);
    uVar11 = FUN_08027fe0();
    FUN_0803157c(uVar9,uVar11,0);
    FUN_080316cc(uVar10,DAT_08007e30,0);
    FUN_0802d33c(uVar10,&DAT_08007e34);
  }
  uVar4 = 0;
  uVar10 = FUN_08006220(uVar8,3,0xffffffd8,0x28,0x6c,0x6c);
  uVar11 = FUN_08027d90(0x27,0x27);
  FUN_0803157c(uVar10,uVar11,0);
  FUN_08031594(uVar10,0xff,0);
  FUN_08031696(uVar10,0x6c,0);
  FUN_0803161c(uVar10,0xff,4);
  uVar11 = FUN_08027fe0();
  FUN_08031604(uVar10,uVar11,4);
  FUN_08031630(uVar10,2,4);
  FUN_08031626(uVar10,4);
  FUN_080315b6(uVar10,0);
  FUN_080315c0(uVar10,2,0);
  FUN_080315b6(uVar10,0,4);
  FUN_080315c0(uVar10,2,4);
  uVar11 = FUN_08027fe0();
  FUN_0803159e(uVar10,uVar11,4);
  uVar3 = FUN_08027fe0();
  uVar11 = FUN_0802553e(uVar10,9,0,0,DAT_08007e3c,CONCAT22(uVar4,uVar3),&DAT_08007e38);
  FUN_0802d97a(uVar10,DAT_08007e40,0);
  if (*DAT_08007e44 == '\0') {
    if (*DAT_08007e48 != '\0') {
      uVar12 = FUN_08027adc();
      FUN_080316b4(uVar11,uVar12,0);
      FUN_0802d33c(uVar11,&DAT_08007e38);
      uVar11 = FUN_08027fe0();
      FUN_0803157c(uVar10,uVar11,0);
    }
  }
  else {
    uVar12 = FUN_08027d90(0x27,0x27);
    FUN_0803157c(uVar10,uVar12,0);
    uVar12 = FUN_08027d90(0x4a,0x4a);
    FUN_080316b4(uVar11,uVar12,0);
  }
  uVar4 = 0;
  uVar8 = FUN_08006220(uVar8,1,0x28,0xac,0x108,0x6c);
  uVar11 = FUN_08027d90(0x27,0x27);
  FUN_0803157c(uVar8,uVar11,0);
  FUN_08031594(uVar8,0xff,0);
  FUN_08031696(uVar8,0x6c,0);
  FUN_0803161c(uVar8,0xff,4);
  uVar11 = FUN_08027fe0();
  FUN_08031604(uVar8,uVar11,4);
  FUN_08031630(uVar8,2,4);
  FUN_08031626(uVar8,4);
  FUN_080315b6(uVar8,0);
  FUN_080315c0(uVar8,2,0);
  FUN_080315b6(uVar8,0,4);
  FUN_080315c0(uVar8,2,4);
  uVar11 = FUN_08027fe0();
  FUN_0803159e(uVar8,uVar11,4);
  uVar3 = FUN_08027fe0();
  uVar11 = FUN_0802553e(uVar8,9,0,0,DAT_08007e30,CONCAT22(uVar4,uVar3),&DAT_08007e38);
  FUN_0802d97a(uVar8,DAT_08007e4c,0);
  if (*DAT_08008250 != '\0') {
    uVar12 = FUN_08027fe0();
    FUN_0803157c(uVar8,uVar12,0);
    uVar12 = FUN_08027adc();
    FUN_080316b4(uVar11,uVar12,0);
  }
  uVar13 = FUN_08006220(uVar6,1,0xb,0x155,0xa8,0x5c);
  uVar11 = FUN_08027d90(0x12,0x12);
  FUN_0803157c(uVar13,uVar11,0);
  FUN_08031594(uVar13,0xff,0);
  FUN_08031696(uVar13,0x48,0);
  FUN_0803163a(uVar13,0);
  FUN_080315dc(uVar13,0x52,0x20);
  FUN_080316e0(uVar13,0x9e,0x20);
  FUN_080316ea(uVar13,0x11,0x20);
  FUN_080316f4(uVar13,0x15b,0x20);
  uVar3 = 0;
  uVar14 = FUN_08010acc(uVar13,7,8,0,0x30,0x30);
  FUN_08031696(uVar14,0x30,0);
  uVar11 = FUN_08027d90(0x45,0x45);
  FUN_0803157c(uVar14,uVar11,0);
  FUN_08031594(uVar14,0xff,0);
  pcVar1 = DAT_08008260;
  uVar12 = DAT_0800825c;
  uVar11 = DAT_08008258;
  if (*DAT_08008254 == '\0') {
    uVar4 = FUN_08027fe0();
    uVar15 = CONCAT22(uVar3,uVar4);
    unaff_r10 = FUN_0802553e(uVar14,9,0,0,DAT_08008264,uVar15,&DAT_08007e20);
    uVar3 = (undefined2)((uint)uVar15 >> 0x10);
    if (*pcVar1 == '\0') {
      uVar4 = FUN_08027fe0();
      unaff_r4 = FUN_0802553e(uVar13,9,0xf,0,uVar11,CONCAT22(uVar3,uVar4),&DAT_08008268);
    }
    else if (*pcVar1 == '\x01') {
      uVar4 = FUN_08027fe0();
      unaff_r4 = FUN_0802553e(uVar13,9,0xf,0,uVar12,CONCAT22(uVar3,uVar4),s_Multi_08008270);
    }
  }
  else if (*DAT_08008254 == '\x01') {
    uVar4 = FUN_08027fe0();
    uVar15 = CONCAT22(uVar3,uVar4);
    unaff_r10 = FUN_0802553e(uVar14,9,0,0,DAT_08008278,uVar15,&DAT_08007e20);
    uVar3 = (undefined2)((uint)uVar15 >> 0x10);
    if (*pcVar1 == '\0') {
      uVar4 = FUN_08027fe0();
      unaff_r4 = FUN_0802553e(uVar13,9,0x11,0,uVar11,CONCAT22(uVar3,uVar4),&DAT_0800827c);
    }
    else if (*pcVar1 == '\x01') {
      uVar4 = FUN_08027fe0();
      unaff_r4 = FUN_0802553e(uVar13,9,0xf,0,uVar12,CONCAT22(uVar3,uVar4),&DAT_08008288);
    }
  }
  if (*DAT_08008290 != '\0' || *DAT_08008294 != '\0') {
    uVar14 = FUN_08027d90(0x4a,0x4a);
    FUN_080316b4(unaff_r10,uVar14,0);
    uVar14 = FUN_08027d90(0x4a,0x4a);
    FUN_080316b4(unaff_r4,uVar14,0);
  }
  FUN_0802d97a(uVar13,DAT_08008298,0);
  uVar14 = FUN_08006220(uVar6,1,0xbb,0x155,0xa8,0x5c);
  uVar15 = FUN_08027d90(0x12,0x12);
  FUN_0803157c(uVar14,uVar15,0);
  FUN_08031594(uVar14,0xff,0);
  FUN_08031696(uVar14,0x48,0);
  FUN_0803163a(uVar14,0);
  FUN_080315dc(uVar14,0x52,0x20);
  FUN_080316e0(uVar14,0x9e,0x20);
  FUN_080316ea(uVar14,0xc1,0x20);
  FUN_080316f4(uVar14,0x15b,0x20);
  uVar4 = 0;
  uVar15 = FUN_08010acc(uVar14,7,8,0,0x30,0x30);
  FUN_08031696(uVar15,0x30,0);
  uVar16 = FUN_08027d90(0x45,0x45);
  FUN_0803157c(uVar15,uVar16,0);
  FUN_08031594(uVar15,0xff,0);
  uVar3 = FUN_08027fe0();
  uVar16 = CONCAT22(uVar4,uVar3);
  FUN_0802553e(uVar15,9,0,0,DAT_08008264,uVar16,&DAT_0800829c);
  uVar3 = (undefined2)((uint)uVar16 >> 0x10);
  if (*pcVar1 == '\0') {
    uVar4 = FUN_08027fe0();
    FUN_0802553e(uVar14,9,0xf,0,uVar11,CONCAT22(uVar3,uVar4),&DAT_080082a0);
  }
  else if (*pcVar1 == '\x01') {
    uVar4 = FUN_08027fe0();
    FUN_0802553e(uVar14,9,0x19,0,uVar12,CONCAT22(uVar3,uVar4),s_Setting_080082a8);
  }
  FUN_0802d97a(uVar14,DAT_080082b0,0);
  uVar11 = FUN_08010acc(param_1,9,0,0,0x170,0x1c0);
  pcVar1 = DAT_08008250;
  *(undefined4 *)(DAT_08008250 + 4) = uVar11;
  uVar11 = FUN_08027d90(0x32,0x32);
  FUN_0803157c(*(undefined4 *)(pcVar1 + 4),uVar11,0);
  FUN_08031594(*(undefined4 *)(pcVar1 + 4),0xcc,0);
  FUN_080267d8(*(undefined4 *)(pcVar1 + 4));
  FUN_0802d97a(*(undefined4 *)(pcVar1 + 4),DAT_080082b4,0,uVar8);
  FUN_0802da22(*(undefined4 *)(pcVar1 + 4),1);
  puVar2 = DAT_080082b8;
  *DAT_080082b8 = uVar5;
  puVar2[2] = uVar6;
  puVar2[1] = uVar7;
  puVar2[3] = uVar9;
  puVar2[4] = uVar10;
  puVar2[5] = uVar8;
  puVar2[6] = uVar13;
  puVar2[7] = uVar14;
  uVar5 = FUN_08010acc(uVar6,1,0x8a,0x1b2,0x5c,6);
  uVar6 = FUN_08027fe0();
  FUN_0803157c(uVar5,uVar6,0);
  FUN_08031594(uVar5,0xff,0);
  return;
}

