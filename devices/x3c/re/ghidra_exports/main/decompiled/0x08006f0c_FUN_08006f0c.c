// 0x08006f0c FUN_08006f0c size=9716

uint FUN_08006f0c(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  byte bVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  byte *pbVar5;
  int iVar6;
  uint uVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  uint uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  char cVar18;
  undefined4 unaff_r4;
  undefined4 unaff_r5;
  undefined4 unaff_r6;
  undefined4 unaff_r7;
  char *pcVar19;
  undefined4 local_48 [3];
  undefined4 local_3c;
  char *local_38;
  undefined4 local_34;
  char *local_30;
  undefined4 local_2c;
  
  uVar7 = DAT_08015fe8;
  uVar12 = DAT_08015fe4;
  iVar6 = DAT_08015fe0;
  iVar2 = DAT_08007338;
  switch(param_2) {
  default:
    return param_1;
  case 1:
    *DAT_08012028 = 0;
    uVar8 = 0;
    local_2c = FUN_08006220(param_1,2,0,0x14,0x15e,0x4b);
    uVar10 = FUN_08027d38(0x27,0x27);
    FUN_0803157c(local_2c,uVar10,0);
    FUN_08031594(local_2c,0xff,0);
    FUN_0802d97a(local_2c,DAT_0801202c,0);
    uVar10 = DAT_08012034;
    if (*(char *)(DAT_08012030 + 3) == '\0') {
      uVar9 = FUN_08027d38(0xfd,0x9f,10);
      unaff_r5 = FUN_0802553e(local_2c,9,0,0,DAT_08012048,CONCAT22(uVar8,uVar9),&DAT_08012038);
    }
    else if (*(char *)(DAT_08012030 + 3) == '\x01') {
      uVar9 = FUN_08027d38(0xfd,0x9f,10);
      unaff_r5 = FUN_0802553e(local_2c,9,0,0,uVar10,CONCAT22(uVar8,uVar9),&DAT_0801204c);
    }
    uVar11 = FUN_08026bec(local_2c);
    FUN_080313b2(uVar11,0x3c);
    FUN_08027342(uVar11,0x10e);
    FUN_080271cc(uVar11,0,0x168);
    FUN_08030e2a(uVar11,0,0x30000);
    FUN_0802e2d2(uVar11,2);
    FUN_0802db44(uVar11,9,0);
    FUN_0802da22(uVar11,1);
    if (*(byte *)(DAT_08012030 + 0xa3) < 2) {
      FUN_08032c54(DAT_08012030 + 0x9c);
    }
    else {
      FUN_08032ea0(DAT_08012030 + 0x9c);
    }
    uVar13 = FUN_08027d38(0x70,0x70);
    FUN_08032ed0(DAT_08012030 + 0x9c,uVar13);
    FUN_08032eee(DAT_08012030 + 0x9c,6);
    FUN_0802da9c(uVar11,DAT_08012030 + 0x9c,0);
    if (*(byte *)(DAT_08012030 + 0xab) < 2) {
      FUN_08032c54(DAT_08012030 + 0xa4);
    }
    else {
      FUN_08032ea0(DAT_08012030 + 0xa4);
    }
    uVar13 = FUN_08027fb8();
    FUN_08032ed0(DAT_08012030 + 0xa4,uVar13);
    FUN_08032eee(DAT_08012030 + 0xa4,6);
    FUN_0802da9c(uVar11,DAT_08012030 + 0xa4,0x20000);
    FUN_080273c0(uVar11,10);
    local_30 = (char *)FUN_08010acc(param_1,2,0,0x6a,0x168,0xc2);
    FUN_0802da22(local_30,1);
    uVar12 = 0;
    do {
      uVar9 = 0;
      uVar13 = FUN_08006220(local_30,1,(short)(uVar12 % 3) * 0x79 + 4,(short)(uVar12 / 3) * 0x58 + 4
                            ,0x72,0x50);
      local_48[uVar12] = uVar13;
      FUN_080315fa(uVar13,0xff,0);
      FUN_08031594(local_48[uVar12],0xff,0);
      uVar13 = FUN_08027d38(0x27,0x27);
      FUN_0803157c(local_48[uVar12],uVar13,0);
      FUN_0802d97a(local_48[uVar12],DAT_08012054,0);
      uVar8 = FUN_08027fb8();
      pcVar19 = &DAT_08012058;
      uVar14 = CONCAT22(uVar9,uVar8);
      FUN_0802553e(local_48[uVar12],9,0,0,DAT_0801205c,uVar14,&DAT_08012058);
      uVar13 = DAT_08012060;
      iVar2 = DAT_08012030;
      uVar8 = (undefined2)((uint)uVar14 >> 0x10);
      uVar12 = uVar12 + 1 & 0xff;
    } while (uVar12 < 6);
    if (*(char *)(DAT_08012030 + 3) == '\0') {
      uVar9 = FUN_08027fb8();
      pcVar19 = &DAT_08012064;
      uVar14 = CONCAT22(uVar8,uVar9);
      unaff_r6 = FUN_0802553e(param_1,1,0x41,0x70,uVar13,uVar14,&DAT_08012064);
      uVar8 = (undefined2)((uint)uVar14 >> 0x10);
    }
    else if (*(char *)(DAT_08012030 + 3) == '\x01') {
      uVar9 = FUN_08027fb8();
      pcVar19 = &DAT_0801206c;
      uVar14 = CONCAT22(uVar8,uVar9);
      unaff_r6 = FUN_0802553e(param_1,1,0x4b,0x75,uVar10,uVar14,&DAT_0801206c);
      uVar8 = (undefined2)((uint)uVar14 >> 0x10);
    }
    uVar14 = FUN_08031c80(param_1);
    FUN_0802db44(uVar14,1,0xe,0x9c);
    FUN_0802e2d2(uVar14,0x300);
    uVar15 = FUN_08027ac8();
    FUN_0803157c(uVar14,uVar15,0);
    uVar15 = FUN_08027ac8();
    FUN_0803159e(uVar14,uVar15,0);
    uVar15 = FUN_08027ac8();
    FUN_0803157c(uVar14,uVar15,0x40000);
    FUN_080316cc(uVar14,DAT_0801205c,0);
    uVar15 = FUN_08027fb8();
    FUN_080316b4(uVar14,uVar15,0);
    FUN_08031ee0(uVar14,*(undefined4 *)(iVar2 + 0x1c),0);
    FUN_08032014(uVar14,3);
    FUN_080313b2(uVar14,0x9e,0xa4);
    uVar15 = FUN_08027fb8();
    FUN_08031604(uVar14,uVar15,4);
    uVar15 = FUN_08027d38(0xfd,0x9f,10);
    FUN_08031604(uVar14,uVar15,8);
    FUN_08031626(uVar14,4);
    FUN_08031626(uVar14,4,8);
    FUN_0803161c(uVar14,0xff,0);
    uVar15 = FUN_08027d38(0x26,0x26);
    FUN_08031604(uVar14,uVar15,0);
    FUN_0803161c(uVar14,0xff,4);
    FUN_0803161c(uVar14,0xff,8);
    FUN_0802d97a(uVar14,DAT_08012070,0x12,0);
    FUN_0802d97a(uVar14,DAT_08012070,0x14,0);
    FUN_0802d97a(uVar14,DAT_08012070,0x19,0);
    FUN_0802d97a(uVar14,DAT_08012074,0x1c,0);
    FUN_080316d6(uVar14,0xf,0);
    if (*(char *)(iVar2 + 3) == '\0') {
      uVar9 = FUN_08027fb8();
      pcVar19 = &DAT_08012344;
      unaff_r7 = FUN_0802553e(param_1,1,0xe5,0x70,uVar13,CONCAT22(uVar8,uVar9),&DAT_08012344);
    }
    else if (*(char *)(iVar2 + 3) == '\x01') {
      uVar9 = FUN_08027fb8();
      pcVar19 = &DAT_08012350;
      unaff_r7 = FUN_0802553e(param_1,1,0x103,0x75,uVar10,CONCAT22(uVar8,uVar9),&DAT_08012350);
    }
    uVar15 = FUN_08031c80(param_1);
    FUN_0802db44(uVar15,1,0xc4,0x9c);
    FUN_0802e2d2(uVar15,0x300);
    uVar16 = FUN_08027ac8();
    FUN_0803157c(uVar15,uVar16,0);
    uVar16 = FUN_08027ac8();
    FUN_0803159e(uVar15,uVar16,0);
    uVar16 = FUN_08027ac8();
    FUN_0803157c(uVar15,uVar16,0x40000);
    FUN_080316cc(uVar15,DAT_08012354,0);
    uVar16 = FUN_08027fb8();
    FUN_080316b4(uVar15,uVar16,0);
    FUN_08031ee0(uVar15,*(undefined4 *)(iVar2 + 0x18),0);
    FUN_08032014(uVar15,3);
    FUN_080313b2(uVar15,0x9e,0xa4);
    FUN_080316d6(uVar15,0xf,0);
    uVar16 = FUN_08027fb8();
    FUN_08031604(uVar15,uVar16,4);
    uVar16 = FUN_08027d38(0xfd,0x9f,10);
    FUN_08031604(uVar15,uVar16,8);
    FUN_08031626(uVar15,4);
    FUN_08031626(uVar15,4,8);
    FUN_0803161c(uVar15,0xff,0);
    uVar16 = FUN_08027d38(0x26,0x26);
    FUN_08031604(uVar15,uVar16,0);
    FUN_0803161c(uVar15,0xff,4);
    FUN_0803161c(uVar15,0xff,8);
    uVar16 = FUN_08010acc(param_1,1,0,0xb2,4,0x5c,pcVar19,uVar16);
    uVar17 = FUN_08027fb8();
    FUN_0803157c(uVar16,uVar17,0);
    FUN_08031594(uVar16,0xff,0);
    FUN_0802da22(uVar14,0x400);
    FUN_0802da22(uVar15,0x400);
    FUN_0802d97a(uVar15,DAT_08012358,0x12,0);
    FUN_0802d97a(uVar15,DAT_08012358,0x14,0);
    FUN_0802d97a(uVar15,DAT_08012358,0x19,0);
    FUN_0802d97a(uVar15,DAT_0801235c,0x1c,0);
    uVar8 = 0;
    uVar16 = FUN_08006220(param_1,2,0,0x15a,0x15e,0x4b);
    if (*(char *)(iVar2 + 3) == '\0') {
      uVar9 = FUN_08027d38(0xfd,0x9f,10);
      pcVar19 = &DAT_08012360;
      FUN_0802553e(uVar16,9,0,0,uVar13,CONCAT22(uVar8,uVar9),&DAT_08012360);
    }
    else if (*(char *)(iVar2 + 3) == '\x01') {
      uVar9 = FUN_08027d38(0xfd,0x9f,10);
      pcVar19 = s_Wireless_Sync_08012370;
      FUN_0802553e(uVar16,9,0,0,uVar10,CONCAT22(uVar8,uVar9),s_Wireless_Sync_08012370);
    }
    FUN_0802d97a(uVar16,DAT_08012380,0);
    uVar10 = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar16,uVar10,0);
    FUN_08031594(uVar16,0xff,0);
    FUN_08010acc(param_1,7,0,0,0x28,0x1c0,pcVar19,uVar10);
    puVar3 = DAT_08012384;
    DAT_08012384[0xc] = unaff_r5;
    puVar3[1] = local_48[0];
    puVar3[2] = local_48[1];
    puVar3[3] = local_48[2];
    puVar3[4] = local_3c;
    puVar3[5] = local_38;
    puVar3[6] = local_34;
    puVar3[0xd] = uVar11;
    puVar3[7] = local_30;
    *puVar3 = local_2c;
    puVar3[8] = unaff_r6;
    puVar3[9] = uVar14;
    puVar3[0xb] = unaff_r7;
    puVar3[10] = uVar15;
    puVar3[0xe] = uVar16;
    FUN_08031fba(uVar14,*DAT_08012388 - 1,0);
    uVar12 = FUN_08031fba(uVar15,*DAT_0801238c,0);
    return uVar12;
  case 2:
    break;
  case 3:
    local_38 = (char *)0x15e;
    local_34 = 0x50;
    uVar13 = FUN_08006220(param_1,1,9,0x32);
    local_2c = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar13,local_2c,0);
    FUN_08031594(uVar13,0xff,0);
    FUN_08031696(uVar13,0xf,0);
    FUN_0802d97a(uVar13,DAT_08006700,0);
    local_2c = FUN_08027fb8();
    local_30 = &DAT_08006704;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)DAT_08006708;
    FUN_0802553e(uVar13,7,0);
    uVar11 = DAT_08006714;
    uVar10 = DAT_08006710;
    iVar2 = DAT_0800670c;
    if (*(char *)(DAT_0800670c + 3) == '\0') {
      local_2c = FUN_08027fb8();
      local_30 = &DAT_08006718;
      local_34._0_2_ = (undefined2)local_2c;
      local_38 = (char *)uVar10;
      FUN_0802553e(uVar13,7,0x40,0);
    }
    else if (*(char *)(DAT_0800670c + 3) == '\x01') {
      local_2c = FUN_08027fb8();
      local_30 = s_One_Shoot_08006728;
      local_34._0_2_ = (undefined2)local_2c;
      local_38 = (char *)uVar11;
      FUN_0802553e(uVar13,7,0x40,0);
    }
    local_2c = FUN_08027fb8();
    uVar14 = DAT_08006738;
    local_30 = &DAT_08006734;
    local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
    local_38 = (char *)DAT_08006738;
    uVar15 = FUN_0802553e(uVar13,7,0x126,0);
    *DAT_0800673c = uVar15;
    local_38 = (char *)0x15e;
    local_34 = 0x50;
    uVar15 = FUN_08006220(param_1,1,9,0x8c);
    local_2c = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar15,local_2c,0);
    FUN_08031594(uVar15,0xff,0);
    FUN_08031696(uVar15,0xf,0);
    FUN_0802d97a(uVar15,DAT_08006700,0);
    local_2c = FUN_08027fb8();
    local_30 = &DAT_08006740;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)DAT_08006708;
    FUN_0802553e(uVar15,7,0);
    cVar18 = *(char *)(iVar2 + 3);
    if (cVar18 == '\0') {
      local_2c = FUN_08027fb8();
      local_30 = &DAT_08006744;
      local_34._0_2_ = (undefined2)local_2c;
      local_38 = (char *)uVar10;
      FUN_0802553e(uVar15,7,0x40,0);
    }
    else if (cVar18 == '\x01') {
      local_2c = FUN_08027fb8();
      local_30 = s_All_Shoot_08006754;
      local_34._0_2_ = (undefined2)local_2c;
      local_38 = (char *)uVar11;
      FUN_0802553e(uVar15,7,0x40,0);
    }
    local_2c = FUN_08027fb8();
    local_30 = &DAT_08006734;
    local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
    local_38 = (char *)uVar14;
    uVar10 = FUN_0802553e(uVar15,7,0x126,0);
    puVar3 = DAT_0800673c;
    DAT_0800673c[1] = uVar10;
    local_38 = (char *)0x15e;
    local_34 = 0x50;
    uVar10 = FUN_08006220(param_1,1,9,0xe6);
    local_2c = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar10,local_2c,0);
    FUN_08031594(uVar10,0xff,0);
    FUN_08031696(uVar10,0xf,0);
    FUN_0802d97a(uVar10,DAT_08006700,0);
    local_2c = FUN_08027fb8();
    local_30 = &DAT_08006760;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)DAT_08006764;
    FUN_0802553e(uVar10,7,5,0);
    local_2c = FUN_08027fb8();
    local_30 = s_L_858_08006768;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)uVar11;
    FUN_0802553e(uVar10,7,0x40,0);
    local_2c = FUN_08027fb8();
    local_30 = &DAT_08006734;
    local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
    local_38 = (char *)uVar14;
    uVar11 = FUN_0802553e(uVar10,7,0x126,0);
    puVar3[2] = uVar11;
    puVar3[-0x10] = uVar13;
    puVar3[-0xf] = uVar15;
    puVar3[-0xe] = uVar10;
    local_38 = (char *)0x4;
    local_34 = 0x5c;
    uVar10 = FUN_08010acc(param_1,1,0,0xb2);
    local_2c = FUN_08027fb8();
    FUN_0803157c(uVar10,local_2c,0);
    FUN_08031594(uVar10,0xff,0);
    uVar10 = DAT_08006770;
    cVar18 = *(char *)(iVar2 + 3);
    if (cVar18 == '\0') {
      local_2c = FUN_08027d38(0xfd,0x9f,10);
      local_30 = &DAT_08006740;
      local_38 = (char *)uVar10;
      local_34._0_2_ = (undefined2)local_2c;
      FUN_0802553e(param_1,1,0x25,0x18e);
      local_2c = FUN_08027d38(0x86,0x86);
      local_30 = &DAT_08006774;
      local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
      local_38 = (char *)DAT_08006794;
      FUN_0802553e(param_1,1,0x45,0x18e);
    }
    else if (cVar18 == '\x01') {
      local_2c = FUN_08027d38(0xfd,0x9f,10);
      local_30 = &DAT_08006740;
      local_34._0_2_ = (undefined2)local_2c;
      local_38 = (char *)uVar10;
      FUN_0802553e(param_1,1,0x24,0x16c);
      local_2c = FUN_08027d38(0x86,0x86);
      local_30 = s_All_shoot_mode_only_applies_to_g_08006798;
      local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
      local_38 = (char *)DAT_080067c4;
      FUN_0802553e(param_1,1,0x45,0x165);
    }
    if (*DAT_080067c8 == '\x01') {
      FUN_0802da22(puVar3[-0xf],1);
      FUN_0802db44(puVar3[-0xe],1,9,0x8c);
    }
    if (*DAT_080067cc != '\0') {
      FUN_0802e2d2(puVar3[2],1);
      FUN_0802da22(puVar3[1],1);
      uVar12 = FUN_0802da22(*puVar3,1);
      return uVar12;
    }
    uVar12 = (uint)*DAT_080067d0;
    if (uVar12 == 0) {
      FUN_0802e2d2(*puVar3,1);
      FUN_0802da22(puVar3[1],1);
      uVar12 = FUN_0802da22(puVar3[2],1);
      return uVar12;
    }
    if (uVar12 == 1) {
      FUN_0802e2d2(puVar3[1],1);
      FUN_0802da22(*puVar3,1);
      uVar12 = FUN_0802da22(puVar3[2],1);
      return uVar12;
    }
    return uVar12;
  case 4:
    uVar10 = FUN_08031c80(param_1);
    FUN_0802e2d2(uVar10,0x300);
    FUN_0802db44(uVar10,9,0);
    FUN_08031696(uVar10,0xf,0);
    uVar11 = FUN_08027ac8();
    FUN_0803157c(uVar10,uVar11,0);
    uVar11 = FUN_08027ac8();
    FUN_0803159e(uVar10,uVar11,0);
    uVar11 = FUN_08027ac8();
    FUN_0803157c(uVar10,uVar11,0x40000);
    FUN_080316cc(uVar10,DAT_0800d4f0,0);
    uVar11 = FUN_08027fb8();
    FUN_080316b4(uVar10,uVar11,0);
    FUN_08031ee0(uVar10,*(undefined4 *)(DAT_0800d4f4 + 0x24),0);
    FUN_08032014(uVar10,3);
    FUN_080313b2(uVar10,0x154,0xa4);
    uVar11 = FUN_08027fb8();
    FUN_08031604(uVar10,uVar11,4);
    uVar11 = FUN_08027d38(0xfd,0x9f,10);
    FUN_08031604(uVar10,uVar11,8);
    FUN_08031626(uVar10,6,4);
    FUN_08031626(uVar10,6,8);
    FUN_0803161c(uVar10,0xff,0);
    uVar11 = FUN_08027d38(0x26,0x26);
    FUN_08031604(uVar10,uVar11,0);
    FUN_0803161c(uVar10,0xff,4);
    FUN_0803161c(uVar10,0xff,8);
    FUN_0802d97a(uVar10,DAT_0800d4f8,0x12,0);
    FUN_0802d97a(uVar10,DAT_0800d4f8,0x14,0);
    FUN_0802d97a(uVar10,DAT_0800d4f8,0x19,0);
    FUN_0802d97a(uVar10,DAT_0800d4fc,0);
    FUN_080316d6(uVar10,0xf,0);
    *(undefined4 *)(DAT_0800d4f4 + 0x60) = uVar10;
    uVar11 = FUN_08010acc(param_1,1,0,0xb2);
    uVar13 = FUN_08027fb8();
    FUN_0803157c(uVar11,uVar13,0);
    FUN_08031594(uVar11,0xff,0);
    uVar12 = FUN_08031fba(uVar10,*DAT_0800d500,0);
    return uVar12;
  case 5:
    local_38 = (char *)0x15e;
    local_34 = 0x50;
    uVar13 = FUN_08010acc(param_1,1,9,0x32);
    local_2c = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar13,local_2c,0);
    FUN_08031594(uVar13,0xff,0);
    FUN_08031696(uVar13,0xf,0);
    FUN_080315c0(uVar13,2,0);
    FUN_080315b6(uVar13,0);
    local_2c = FUN_08027fb8();
    FUN_0803159e(uVar13,local_2c,0);
    FUN_080315b6(uVar13,0xff,4);
    FUN_0802da22(uVar13,2);
    FUN_0802d97a(uVar13,DAT_08005d18,0);
    uVar11 = DAT_08005d24;
    uVar10 = DAT_08005d20;
    iVar2 = DAT_08005d1c;
    if (*(char *)(DAT_08005d1c + 3) == '\0') {
      local_2c = FUN_08027fb8();
      local_30 = &DAT_08005d28;
      local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
      local_38 = (char *)uVar10;
      unaff_r4 = FUN_0802553e(uVar13,7,0xc,0);
    }
    else if (*(char *)(DAT_08005d1c + 3) == '\x01') {
      local_2c = FUN_08027fb8();
      local_30 = s_Auto_Off_08005d38;
      local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
      local_38 = (char *)uVar11;
      unaff_r4 = FUN_0802553e(uVar13,7,0xc,0);
    }
    uVar14 = FUN_08033224(uVar13);
    FUN_0802db44(uVar14,7,0x102,0);
    FUN_080313b2(uVar14,0x50,0x2c);
    local_2c = FUN_08027ac8();
    FUN_0803157c(uVar14,local_2c,0);
    local_2c = FUN_08027d38(0xfd,0x9f,10);
    FUN_0803157c(uVar14,local_2c,0x20000);
    local_2c = FUN_08027d38(0xfd,0x9f,10);
    FUN_0803157c(uVar14,local_2c,DAT_08005d44);
    FUN_0802e2d2(uVar14,2);
    uVar15 = FUN_08031c80(param_1);
    FUN_0802e2d2(uVar15,0x300);
    FUN_0802db44(uVar15,2,0,0x8d);
    FUN_08031696(uVar15,0xf,0);
    local_2c = FUN_08027ac8();
    FUN_0803157c(uVar15,local_2c,0);
    local_2c = FUN_08027ac8();
    FUN_0803159e(uVar15,local_2c,0);
    local_2c = FUN_08027ac8();
    FUN_0803157c(uVar15,local_2c,0x40000);
    FUN_080316cc(uVar15,DAT_08005d48,0);
    local_2c = FUN_08027fb8();
    FUN_080316b4(uVar15,local_2c,0);
    FUN_08031ee0(uVar15,*(undefined4 *)(iVar2 + 0x28),0);
    FUN_08032014(uVar15,3);
    FUN_080313b2(uVar15,0x156,0xa4);
    local_2c = FUN_08027fb8();
    FUN_08031604(uVar15,local_2c,4);
    local_2c = FUN_08027d38(0xfd,0x9f,10);
    FUN_08031604(uVar15,local_2c,8);
    FUN_08031626(uVar15,6,4);
    FUN_08031626(uVar15,6,8);
    FUN_0803161c(uVar15,0xff,0);
    local_2c = FUN_08027d38(0x26,0x26);
    FUN_08031604(uVar15,local_2c,0);
    FUN_0803161c(uVar15,0xff,4);
    FUN_0803161c(uVar15,0xff,8);
    FUN_0802d97a(uVar15,DAT_08005d4c,0x12,0);
    FUN_0802d97a(uVar15,DAT_08005d4c,0x14,0);
    FUN_0802d97a(uVar15,DAT_08005d4c,0x19,0);
    FUN_0802d97a(uVar15,DAT_08005d50,0);
    puVar3 = DAT_08005d54;
    *DAT_08005d54 = uVar13;
    puVar3[1] = uVar14;
    puVar3[2] = uVar15;
    FUN_080316d6(uVar15,0xf,0);
    if (*(char *)(iVar2 + 3) == '\0') {
      local_2c = FUN_08027fb8();
      local_30 = &DAT_08005d58;
      local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
      local_38 = (char *)uVar10;
      unaff_r5 = FUN_0802553e(param_1,1,0xe3,0xd1);
    }
    else if (*(char *)(iVar2 + 3) == '\x01') {
      local_2c = FUN_08027fb8();
      local_30 = &DAT_08005d60;
      local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
      local_38 = (char *)uVar11;
      unaff_r5 = FUN_0802553e(param_1,1,0xe3,0xd1);
    }
    local_38 = (char *)0x4;
    local_34 = 0x5c;
    uVar10 = FUN_08010acc(param_1,1,0,0xb2);
    local_2c = FUN_08027fb8();
    FUN_0803157c(uVar10,local_2c,0);
    FUN_08031594(uVar10,0xff,0);
    *(undefined4 *)(iVar2 + 0x6c) = uVar15;
    *(undefined4 *)(iVar2 + 0x70) = unaff_r4;
    *(undefined4 *)(iVar2 + 0x74) = unaff_r5;
    if ((int)((uint)*(byte *)(iVar2 + 1) << 0x18) < 0) {
      FUN_0802da8e(uVar14,1);
      FUN_0802e2d2(*(undefined4 *)(iVar2 + 0x6c),1);
      FUN_0802e2d2(*(undefined4 *)(iVar2 + 0x74),1);
    }
    else {
      FUN_0802e34c(uVar14,1);
      FUN_0802da22(*(undefined4 *)(iVar2 + 0x6c),1);
      FUN_0802da22(*(undefined4 *)(iVar2 + 0x74),1);
    }
    uVar12 = FUN_08031fba(*(undefined4 *)(iVar2 + 0x6c),*(byte *)(iVar2 + 1) & 3,0);
    return uVar12;
  case 6:
    local_38 = (char *)0x15e;
    local_34 = 0x50;
    local_30 = (char *)param_3;
    local_2c = param_4;
    uVar13 = FUN_08006220(param_1,1,9,0x32);
    local_2c = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar13,local_2c,0);
    FUN_08031594(uVar13,0xff,0);
    FUN_08031696(uVar13,0xf,0);
    local_2c = FUN_08027fb8();
    uVar10 = DAT_08011560;
    local_30 = s_0_30m_08011558;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)DAT_08011560;
    FUN_0802553e(uVar13,7,0x20,0);
    local_2c = FUN_08027fb8();
    uVar11 = DAT_08011568;
    local_30 = &DAT_08011564;
    local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
    local_38 = (char *)DAT_08011568;
    uVar14 = FUN_0802553e(uVar13,7,0x126,0);
    FUN_0802d97a(uVar13,DAT_0801156c,0);
    local_38 = (char *)0x15e;
    local_34 = 0x50;
    uVar15 = FUN_08006220(param_1,1,9,0x8c);
    local_2c = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar15,local_2c,0);
    FUN_08031594(uVar15,0xff,0);
    FUN_08031696(uVar15,0xf,0);
    local_2c = FUN_08027fb8();
    local_30 = s_1_100m_08011570;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)uVar10;
    FUN_0802553e(uVar15,7,0x20,0);
    local_2c = FUN_08027fb8();
    local_30 = &DAT_08011564;
    local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
    local_38 = (char *)uVar11;
    uVar10 = FUN_0802553e(uVar15,7,0x126,0);
    FUN_0802d97a(uVar15,DAT_0801156c,0);
    local_38 = (char *)0x4;
    local_34 = 0x5c;
    uVar11 = FUN_08010acc(param_1,1,0,0xb2);
    local_2c = FUN_08027fb8();
    FUN_0803157c(uVar11,local_2c,0);
    FUN_08031594(uVar11,0xff,0);
    puVar3 = DAT_08011578;
    *DAT_08011578 = uVar14;
    puVar3[1] = uVar10;
    puVar4 = DAT_0801157c;
    *DAT_0801157c = uVar13;
    puVar4[2] = uVar15;
    if (*DAT_08011580 == '\0') {
      FUN_0802e2d2(uVar10,1);
      uVar12 = FUN_0802da22(*puVar3,1);
      return uVar12;
    }
    if (*DAT_08011580 == '\x01') {
      FUN_0802e2d2(uVar14);
      uVar12 = FUN_0802da22(puVar3[1],1);
      return uVar12;
    }
    return (uint)local_38;
  case 7:
    local_30 = (char *)param_1;
    local_2c = param_2;
    if (*(char *)(DAT_08015fe0 + 3) == '\0') {
      uVar8 = FUN_08027fb8();
      local_2c = CONCAT22(local_2c._2_2_,uVar8);
      local_30 = (char *)uVar12;
      FUN_0802553e(param_1,1,0x2f,0x52);
    }
    else if (*(char *)(DAT_08015fe0 + 3) == '\x01') {
      uVar8 = FUN_08027fb8();
      local_2c = CONCAT22(local_2c._2_2_,uVar8);
      local_30 = (char *)uVar7;
      FUN_0802553e(param_1,1,0x2f,0x52);
    }
    if (*(char *)(iVar6 + 3) == '\0') {
      uVar8 = FUN_08027fb8();
      local_2c = CONCAT22(local_2c._2_2_,uVar8);
      local_30 = (char *)uVar12;
      FUN_0802553e(param_1,1,0xfc,0x52);
    }
    else if (*(char *)(iVar6 + 3) == '\x01') {
      uVar8 = FUN_08027fb8();
      local_2c = CONCAT22(local_2c._2_2_,uVar8);
      local_30 = (char *)uVar7;
      FUN_0802553e(param_1,1,0xfc,0x52);
    }
    uVar10 = FUN_08031c80(param_1);
    FUN_0802e2d2(uVar10,0x300);
    FUN_0802db44(uVar10,1,0x10,0x82);
    FUN_080315c0(uVar10,0);
    uVar11 = FUN_08027fb8();
    FUN_080316b4(uVar10,uVar11,0);
    uVar11 = FUN_08027ac8();
    FUN_0803157c(uVar10,uVar11,0);
    uVar11 = FUN_08027ac8();
    FUN_0803157c(uVar10,uVar11,0x40000);
    FUN_08031594(uVar10,0xff,0);
    FUN_08031696(uVar10,0xf,0);
    FUN_080316cc(uVar10,DAT_08016018,0);
    FUN_08031ee0(uVar10,*(undefined4 *)(iVar6 + 0x2c),0);
    FUN_08032014(uVar10,3);
    FUN_080313b2(uVar10,0xac,0xa4);
    FUN_080316d6(uVar10,0xf,0);
    uVar11 = FUN_08027fb8();
    FUN_08031604(uVar10,uVar11,4);
    uVar11 = FUN_08027d38(0xfd,0x9f,10);
    FUN_08031604(uVar10,uVar11,8);
    FUN_08031626(uVar10,6,4);
    FUN_08031626(uVar10,6,8);
    FUN_0803161c(uVar10,0xff,0);
    uVar11 = FUN_08027d38(0x26,0x26);
    FUN_08031604(uVar10,uVar11,0);
    FUN_0803161c(uVar10,0xff,4);
    FUN_0803161c(uVar10,0xff,8);
    FUN_0802d97a(uVar10,DAT_0801601c,0x12,0);
    FUN_0802d97a(uVar10,DAT_0801601c,0x14,0);
    FUN_0802d97a(uVar10,DAT_0801601c,0x19,0);
    FUN_0802d97a(uVar10,DAT_08016020,0x1c,0);
    uVar11 = FUN_08031c80(param_1);
    FUN_0802e2d2(uVar11,0x300);
    FUN_0802db44(uVar11,1,0xc6,0x82);
    FUN_080315c0(uVar11,0);
    uVar13 = FUN_08027fb8();
    FUN_080316b4(uVar11,uVar13,0);
    uVar13 = FUN_08027ac8();
    FUN_0803157c(uVar11,uVar13,0);
    uVar13 = FUN_08027ac8();
    FUN_0803157c(uVar11,uVar13,0x40000);
    FUN_08031594(uVar11,0xff,0);
    FUN_08031696(uVar11,0xf,0);
    FUN_080316cc(uVar11,DAT_08016018,0);
    FUN_08031ee0(uVar11,*(undefined4 *)(iVar6 + 0x30),0);
    FUN_08032014(uVar11,3);
    FUN_080313b2(uVar11,0x98,0xa4);
    FUN_080316d6(uVar11,0xf,0);
    uVar13 = FUN_08027fb8();
    FUN_08031604(uVar11,uVar13,4);
    uVar13 = FUN_08027d38(0xfd,0x9f,10);
    FUN_08031604(uVar11,uVar13,8);
    FUN_08031626(uVar11,6,4);
    FUN_08031626(uVar11,6,8);
    FUN_0803161c(uVar11,0xff,0);
    uVar13 = FUN_08027d38(0x26,0x26);
    FUN_08031604(uVar11,uVar13,0);
    FUN_0803161c(uVar11,0xff,4);
    FUN_0803161c(uVar11,0xff,8);
    FUN_0802d97a(uVar11,DAT_0801601c,0x12,0);
    FUN_0802d97a(uVar11,DAT_0801601c,0x14,0);
    FUN_0802d97a(uVar11,DAT_0801601c,0x19,0);
    FUN_0802d97a(uVar11,DAT_08016024,0x1c,0);
    local_30 = (char *)0x4;
    local_2c = 0x5c;
    uVar13 = FUN_08010acc(param_1,1,0,0xb2);
    uVar14 = FUN_08027fb8();
    FUN_0803157c(uVar13,uVar14,0);
    FUN_08031594(uVar13,0xff,0);
    iVar2 = DAT_08015fe0;
    *(undefined4 *)(DAT_08015fe0 + 0xbc) = uVar10;
    *(undefined4 *)(iVar2 + 0xc0) = uVar11;
    FUN_08031fba(uVar10,(int)(*DAT_08016028 - 0x46) / 10 + (uint)*DAT_0801602c * 3 & 0xff,0);
    uVar12 = FUN_08031fba(uVar11,*DAT_08016030 != '\x01',0);
    return uVar12;
  case 8:
    uVar10 = FUN_08031c80(param_1);
    FUN_0802e2d2(uVar10,0x300);
    FUN_0802db44(uVar10,9,0);
    FUN_08031696(uVar10,0xf,0);
    uVar11 = FUN_08027ac8();
    FUN_0803157c(uVar10,uVar11,0);
    uVar11 = FUN_08027ac8();
    FUN_0803159e(uVar10,uVar11,0);
    uVar11 = FUN_08027ac8();
    FUN_0803157c(uVar10,uVar11,0x40000);
    FUN_080316cc(uVar10,DAT_08016bc0,0);
    uVar11 = FUN_08027fb8();
    FUN_080316b4(uVar10,uVar11,0);
    FUN_08031ee0(uVar10,*(undefined4 *)(DAT_08016bc4 + 0x38),0);
    FUN_08032014(uVar10,3);
    FUN_080313b2(uVar10,0x154,0xa4);
    uVar11 = FUN_08027fb8();
    FUN_08031604(uVar10,uVar11,4);
    uVar11 = FUN_08027d38(0xfd,0x9f,10);
    FUN_08031604(uVar10,uVar11,8);
    FUN_08031626(uVar10,6,4);
    FUN_08031626(uVar10,6,8);
    FUN_0803161c(uVar10,0xff,0);
    uVar11 = FUN_08027d38(0x26,0x26);
    FUN_08031604(uVar10,uVar11,0);
    FUN_0803161c(uVar10,0xff,4);
    FUN_0803161c(uVar10,0xff,8);
    FUN_0802d97a(uVar10,DAT_08016bc8,0x12,0);
    FUN_0802d97a(uVar10,DAT_08016bc8,0x14,0);
    FUN_0802d97a(uVar10,DAT_08016bc8,0x19,0);
    FUN_0802d97a(uVar10,DAT_08016bcc,0);
    FUN_080316d6(uVar10,0xf,0);
    *(undefined4 *)(DAT_08016bc4 + 100) = uVar10;
    uVar11 = FUN_08010acc(param_1,1,0,0xb2);
    uVar13 = FUN_08027fb8();
    FUN_0803157c(uVar11,uVar13,0);
    FUN_08031594(uVar11,0xff,0);
    uVar12 = FUN_08031fba(uVar10,*DAT_08016bd0,0);
    return uVar12;
  case 9:
    uVar10 = FUN_08031c80(param_1);
    FUN_0802e2d2(uVar10,0x300);
    FUN_0802db44(uVar10,9,0);
    FUN_08031696(uVar10,0xf,0);
    uVar11 = FUN_08027ac8();
    FUN_0803157c(uVar10,uVar11,0);
    uVar11 = FUN_08027ac8();
    FUN_0803159e(uVar10,uVar11,0);
    uVar11 = FUN_08027ac8();
    FUN_0803157c(uVar10,uVar11,0x40000);
    FUN_080316cc(uVar10,DAT_08017430,0);
    uVar11 = FUN_08027fb8();
    FUN_080316b4(uVar10,uVar11,0);
    FUN_08031ee0(uVar10,*(undefined4 *)(DAT_08017434 + 0x3c),0);
    FUN_08032014(uVar10,3);
    FUN_080313b2(uVar10,0x154,0xa4);
    uVar11 = FUN_08027fb8();
    FUN_08031604(uVar10,uVar11,4);
    uVar11 = FUN_08027d38(0xfd,0x9f,10);
    FUN_08031604(uVar10,uVar11,8);
    FUN_08031626(uVar10,6,4);
    FUN_08031626(uVar10,6,8);
    FUN_0803161c(uVar10,0xff,0);
    uVar11 = FUN_08027d38(0x26,0x26);
    FUN_08031604(uVar10,uVar11,0);
    FUN_0803161c(uVar10,0xff,4);
    FUN_0803161c(uVar10,0xff,8);
    FUN_0802d97a(uVar10,DAT_08017438,0x12,0);
    FUN_0802d97a(uVar10,DAT_08017438,0x14,0);
    FUN_0802d97a(uVar10,DAT_08017438,0x19,0);
    FUN_0802d97a(uVar10,DAT_0801743c,0);
    FUN_080316d6(uVar10,0xf,0);
    iVar2 = DAT_08017434;
    *(undefined4 *)(DAT_08017434 + 0xd4) = uVar10;
    FUN_08027fb8();
    uVar11 = FUN_0802553e(param_1,1,0xf7,0xd2);
    *(undefined4 *)(iVar2 + 0xd8) = uVar11;
    uVar11 = FUN_08010acc(param_1,1,0,0xb2);
    uVar13 = FUN_08027fb8();
    FUN_0803157c(uVar11,uVar13,0);
    FUN_08031594(uVar11,0xff,0);
    bVar1 = *DAT_08017448;
    if (bVar1 < 0x19) {
      cVar18 = '\x01';
    }
    else if (bVar1 < 0x1d) {
      cVar18 = '\x02';
    }
    else if (bVar1 < 0x24) {
      cVar18 = '\x03';
    }
    else if (bVar1 < 0x33) {
      cVar18 = '\x04';
    }
    else if (bVar1 < 0x47) {
      cVar18 = '\x05';
    }
    else if (bVar1 < 0x51) {
      cVar18 = '\x06';
    }
    else if (bVar1 < 0x6a) {
      cVar18 = '\a';
    }
    else if (bVar1 < 0x88) {
      cVar18 = '\b';
    }
    else {
      cVar18 = '\t';
    }
    if (*DAT_0801744c != '\0') {
      cVar18 = '\0';
    }
    *DAT_08017450 = cVar18;
    FUN_08031fba(uVar10,cVar18,0);
    if (cVar18 == '\0') {
      uVar12 = FUN_0802da22(*(undefined4 *)(iVar2 + 0xd8),1);
      return uVar12;
    }
    return 4;
  case 10:
    local_48[2] = 0x15e;
    local_3c = 0x50;
    uVar11 = FUN_08010acc(param_1,1,9,0x32);
    local_30 = (char *)FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar11,local_30,0);
    FUN_08031594(uVar11,0xff,0);
    FUN_08031696(uVar11,0xf,0);
    FUN_080315c0(uVar11,2,0);
    FUN_080315b6(uVar11,0);
    FUN_080315b6(uVar11,0xff,2);
    FUN_080315b6(uVar11,0xff,8);
    local_30 = (char *)FUN_08027fb8();
    FUN_0803159e(uVar11,local_30,2);
    local_30 = (char *)FUN_08027d38(0xfd,0x9f,10);
    FUN_0803159e(uVar11,local_30,8);
    local_30 = (char *)FUN_08027fb8();
    local_38 = &DAT_08014358;
    local_3c = CONCAT22(local_3c._2_2_,local_30._0_2_);
    local_48[2] = DAT_0801435c;
    FUN_0802553e(uVar11,7,0xf,0);
    local_48[2] = 0x4f;
    local_3c = 0x104;
    local_38 = (char *)0x4c;
    uVar13 = FUN_08038042(uVar11,7,0x4e,0);
    local_48[2] = 0x104;
    local_3c = 0x14;
    uVar10 = FUN_08010acc(uVar11,7,0x4e,0);
    local_30 = (char *)FUN_08027d38(0x36,0x36);
    FUN_0803157c(uVar10,local_30,0);
    FUN_08031594(uVar10,0xff,0);
    FUN_08031696(uVar10,0xf,0);
    FUN_0802d97a(uVar11,DAT_08014360,0,uVar13);
    local_48[2] = 0;
    local_3c = 0x14;
    local_2c = FUN_08010acc(uVar10,7,0,0);
    local_30 = (char *)FUN_08027fb8();
    FUN_0803157c(local_2c,local_30,0);
    FUN_08031594(local_2c,0xff,0);
    FUN_0802e2d2(uVar10,2);
    FUN_0802e2d2(local_2c,2);
    FUN_08031696(local_2c,0xf,0);
    FUN_0802d97a(uVar13,DAT_08014364,0,local_2c);
    local_48[2] = 0x15e;
    local_3c = 0x50;
    uVar14 = FUN_08010acc(param_1,1,9,0x8c);
    local_30 = (char *)FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar14,local_30,0);
    FUN_08031594(uVar14,0);
    FUN_08031696(uVar14,0xf,0);
    FUN_080315c0(uVar14,2,0);
    FUN_080315b6(uVar14,0);
    uVar10 = DAT_0801436c;
    iVar2 = DAT_08014368;
    if (*(char *)(DAT_08014368 + 3) == '\0') {
      local_30 = (char *)FUN_08027fb8();
      local_38 = &DAT_08014370;
      local_3c = CONCAT22(local_3c._2_2_,local_30._0_2_);
      local_48[2] = uVar10;
      FUN_0802553e(uVar14,7,0xc,0);
    }
    else if (*(char *)(DAT_08014368 + 3) == '\x01') {
      local_30 = (char *)FUN_08027fb8();
      local_38 = s_Standby_Time_08014380;
      local_3c = CONCAT22(local_3c._2_2_,local_30._0_2_);
      local_48[2] = DAT_08014390;
      FUN_0802553e(uVar14,7,0xc,0);
    }
    uVar15 = FUN_08031c80(param_1);
    FUN_0802e2d2(uVar15,0x300);
    FUN_0802db44(uVar15,2,0,0xef);
    FUN_08031696(uVar15,0xf,0);
    local_30 = (char *)FUN_08027ac8();
    FUN_0803157c(uVar15,local_30,0);
    local_30 = (char *)FUN_08027ac8();
    FUN_0803159e(uVar15,local_30,0);
    local_30 = (char *)FUN_08027ac8();
    FUN_0803157c(uVar15,local_30,0x40000);
    FUN_080316cc(uVar15,DAT_08014394,0);
    local_30 = (char *)FUN_08027fb8();
    FUN_080316b4(uVar15,local_30,0);
    FUN_08031ee0(uVar15,*(undefined4 *)(iVar2 + 0x34),0);
    FUN_08032014(uVar15,3);
    FUN_080313b2(uVar15,0x154,0xa4);
    local_30 = (char *)FUN_08027fb8();
    FUN_08031604(uVar15,local_30,4);
    local_30 = (char *)FUN_08027d38(0xfd,0x9f,10);
    FUN_08031604(uVar15,local_30,8);
    FUN_08031626(uVar15,6,4);
    FUN_08031626(uVar15,6,8);
    FUN_0803161c(uVar15,0xff,0);
    local_30 = (char *)FUN_08027d38(0x26,0x26);
    FUN_08031604(uVar15,local_30,0);
    FUN_0803161c(uVar15,0xff,4);
    FUN_0803161c(uVar15,0xff,8);
    FUN_0802d97a(uVar15,DAT_08014398,0x12,0);
    FUN_0802d97a(uVar15,DAT_08014398,0x14,0);
    FUN_0802d97a(uVar15,DAT_08014398,0x19,0);
    FUN_0802d97a(uVar15,DAT_0801439c,0);
    FUN_080316d6(uVar15,0xf,0);
    if (*(char *)(iVar2 + 3) == '\0') {
      local_30 = (char *)FUN_08027fb8();
      local_38 = &DAT_080143a0;
      local_3c = CONCAT22(local_3c._2_2_,local_30._0_2_);
      local_48[2] = uVar10;
      unaff_r4 = FUN_0802553e(param_1,1,0xe3,0x131);
    }
    else if (*(char *)(iVar2 + 3) == '\x01') {
      local_30 = (char *)FUN_08027fb8();
      local_38 = &DAT_080143a8;
      local_3c = CONCAT22(local_3c._2_2_,local_30._0_2_);
      local_48[2] = DAT_08014390;
      unaff_r4 = FUN_0802553e(param_1,1,0xe3,0x131);
    }
    local_48[2] = 4;
    local_3c = 0x5c;
    uVar10 = FUN_08010acc(param_1,1,0,0xb2);
    local_30 = (char *)FUN_08027fb8();
    FUN_0803157c(uVar10,local_30,0);
    FUN_08031594(uVar10,0xff,0);
    *(undefined4 *)(iVar2 + 0x78) = uVar15;
    *(undefined4 *)(iVar2 + 0x7c) = unaff_r4;
    puVar3 = DAT_080143ac;
    DAT_080143ac[1] = uVar14;
    *puVar3 = uVar11;
    puVar3[3] = uVar15;
    if ((int)((uint)*(byte *)(iVar2 + 2) << 0x18) < 0) {
      FUN_0802e2d2(uVar15,1);
      FUN_0802e2d2(*(undefined4 *)(iVar2 + 0x7c),1);
    }
    else {
      FUN_0802da22(uVar15,1);
      FUN_0802da22(*(undefined4 *)(iVar2 + 0x7c),1);
    }
    FUN_08031fba(*(undefined4 *)(iVar2 + 0x78),*(byte *)(iVar2 + 2) & 7,0);
    if ((*(byte *)(iVar2 + 2) & 7) < 2) {
      if (*(char *)(iVar2 + 3) == '\0') {
        FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_080143b0);
      }
      else if (*(char *)(iVar2 + 3) == '\x01') {
        FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_080143b8);
      }
    }
    else if (*(char *)(iVar2 + 3) == '\0') {
      FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_080143a0);
    }
    else if (*(char *)(iVar2 + 3) == '\x01') {
      FUN_0802d33c(*(undefined4 *)(iVar2 + 0x7c),&DAT_080143a8);
    }
    pbVar5 = DAT_080143bc;
    FUN_080316fe(local_2c,(int)((*DAT_080143bc - 0x14) * 0x104) / 0x50 & 0xffff);
    uVar12 = FUN_0802782e(uVar13,*pbVar5 - 0x14,0);
    return uVar12;
  case 0xb:
    local_30 = (char *)0x15e;
    local_2c = 0x50;
    uVar11 = FUN_08006220(param_1,1,9,0x32);
    uVar10 = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar11,uVar10,0);
    FUN_08031594(uVar11,0xff,0);
    FUN_08031696(uVar11,0xf,0);
    uVar8 = FUN_08027fb8();
    local_30 = (char *)DAT_0800d320;
    local_2c._0_2_ = uVar8;
    FUN_0802553e(uVar11,7,0xc,0);
    uVar8 = FUN_08027fb8();
    uVar10 = DAT_0800d328;
    local_2c = CONCAT22(local_2c._2_2_,uVar8);
    local_30 = (char *)DAT_0800d328;
    uVar13 = FUN_0802553e(uVar11,7,0x126,0);
    local_30 = (char *)0x15e;
    local_2c = 0x50;
    uVar14 = FUN_08006220(param_1,1,9,0x8c);
    uVar15 = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar14,uVar15,0);
    FUN_08031594(uVar14,0xff,0);
    FUN_08031696(uVar14,0xf,0);
    uVar8 = FUN_08027fb8();
    local_30 = (char *)DAT_0800d334;
    local_2c._0_2_ = uVar8;
    FUN_0802553e(uVar14,7,0xc,0);
    uVar8 = FUN_08027fb8();
    local_2c = CONCAT22(local_2c._2_2_,uVar8);
    local_30 = (char *)uVar10;
    uVar10 = FUN_0802553e(uVar14,7,0x126,0);
    puVar3 = DAT_0800d338;
    *DAT_0800d338 = uVar13;
    puVar3[1] = uVar10;
    local_30 = (char *)0x4;
    local_2c = 0x5c;
    uVar10 = FUN_08010acc(param_1,1,0,0xb2);
    uVar13 = FUN_08027fb8();
    FUN_0803157c(uVar10,uVar13,0);
    FUN_08031594(uVar10,0xff,0);
    puVar3[-8] = uVar11;
    puVar3[-7] = uVar14;
    FUN_0802d97a(uVar11,DAT_0800d33c,0);
    FUN_0802d97a(uVar14,DAT_0800d33c,0);
    if (*(char *)((int)puVar3 + -0xe1) == '\0') {
      FUN_0802da22(puVar3[1],1);
      uVar12 = FUN_0802e2d2(*puVar3,1);
      return uVar12;
    }
    FUN_0802da22(*puVar3,1);
    uVar12 = FUN_0802e2d2(puVar3[1],1);
    return uVar12;
  case 0xc:
    uVar12 = FUN_080085f4();
    return uVar12;
  case 0xd:
    if (*(char *)(DAT_08007338 + 3) == '\0') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x14,0x40);
    }
    else if (*(char *)(DAT_08007338 + 3) == '\x01') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x14,0x40);
    }
    pcVar19 = DAT_08007358;
    cVar18 = *DAT_08007358;
    if (cVar18 == '\x01') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x78,0x40);
    }
    else if (cVar18 == '\x02') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x78,0x40);
    }
    else if (cVar18 == '\x03') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x78,0x40);
    }
    else if (cVar18 == '\x05') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x78,0x40);
    }
    else if (cVar18 == '\x04') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x78,0x40);
    }
    else if (cVar18 == '\a') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x78,0x40);
    }
    cVar18 = *(char *)(iVar2 + 3);
    if (cVar18 == '\0') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x14,0x68);
    }
    else if (cVar18 == '\x01') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0x14,0x68);
    }
    cVar18 = *pcVar19;
    if (cVar18 == '\x01') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0xa0,0x68);
    }
    else if ((cVar18 == '\x03') || (cVar18 == '\x02')) {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0xa0,0x68);
    }
    else if (cVar18 == '\x05') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0xa0,0x68);
    }
    else if (cVar18 == '\x04') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0xa0,0x68);
    }
    else if (cVar18 == '\a') {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0xa0,0x68);
    }
    else {
      FUN_08027fb8();
      FUN_0802553e(param_1,1,0xa0,0x68);
    }
    uVar10 = FUN_08006220(param_1,2,0,0x16c);
    cVar18 = *(char *)(iVar2 + 3);
    if (cVar18 == '\0') {
      FUN_08027fb8();
      FUN_0802553e(uVar10,9,0);
    }
    else if (cVar18 == '\x01') {
      FUN_08027fb8();
      FUN_0802553e(uVar10,9,0);
    }
    uVar11 = FUN_08027d38(0x27,0x27);
    FUN_0803157c(uVar10,uVar11,0);
    FUN_08031594(uVar10,0xff,0);
    FUN_0802d97a(uVar10,DAT_080073e8,0);
    *(undefined4 *)(DAT_08007338 + 0x68) = uVar10;
    if (*DAT_080073ec == '\x01') {
      FUN_0802e2d2(uVar10,1);
    }
    else {
      FUN_0802da22(uVar10,1);
    }
    uVar10 = FUN_08010acc(param_1,1,0,0xb2);
    uVar11 = FUN_08027fb8();
    FUN_0803157c(uVar10,uVar11,0);
    uVar12 = FUN_08031594(uVar10,0xff,0);
    return uVar12;
  }
  local_38 = (char *)0x15e;
  local_34 = 0x50;
  uVar13 = FUN_08006220(param_1,1,9,0x32);
  local_2c = FUN_08027d38(0x27,0x27);
  FUN_0803157c(uVar13,local_2c,0);
  FUN_08031594(uVar13,0xff,0);
  FUN_08031696(uVar13,0xf,0);
  FUN_0802d97a(uVar13,DAT_08016584,0);
  local_2c = FUN_08027fb8();
  local_30 = &DAT_08016588;
  local_34._0_2_ = (undefined2)local_2c;
  local_38 = (char *)DAT_0801658c;
  FUN_0802553e(uVar13,7,6,0);
  uVar11 = DAT_08016598;
  uVar10 = DAT_08016594;
  iVar2 = DAT_08016590;
  if (*(char *)(DAT_08016590 + 3) == '\0') {
    local_2c = FUN_08027fb8();
    local_30 = &LAB_0801659c;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)uVar10;
    FUN_0802553e(uVar13,7,0x36,0);
  }
  else if (*(char *)(DAT_08016590 + 3) == '\x01') {
    local_2c = FUN_08027fb8();
    local_30 = s_Front_Curtain_080165ac;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)uVar11;
    FUN_0802553e(uVar13,7,0x36,0);
  }
  local_2c = FUN_08027fb8();
  local_30 = &DAT_080165bc;
  local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
  local_38 = (char *)DAT_080165c0;
  uVar14 = FUN_0802553e(uVar13,7,0x126,0);
  *DAT_080165c4 = uVar14;
  local_38 = (char *)0x15e;
  local_34 = 0x50;
  uVar14 = FUN_08006220(param_1,1,9,0x8c);
  local_2c = FUN_08027d38(0x27,0x27);
  FUN_0803157c(uVar14,local_2c,0);
  FUN_08031594(uVar14,0xff,0);
  FUN_08031696(uVar14,0xf,0);
  FUN_0802d97a(uVar14,DAT_08016584,0);
  local_2c = FUN_08027fb8();
  local_30 = &DAT_080165c8;
  local_34._0_2_ = (undefined2)local_2c;
  local_38 = (char *)DAT_0801658c;
  FUN_0802553e(uVar14,7,6,0);
  cVar18 = *(char *)(iVar2 + 3);
  if (cVar18 == '\0') {
    local_2c = FUN_08027fb8();
    local_30 = &DAT_080165cc;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)uVar10;
    FUN_0802553e(uVar14,7,0x36,0);
  }
  else if (cVar18 == '\x01') {
    local_2c = FUN_08027fb8();
    local_30 = s_High_Speed_080165dc;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)uVar11;
    FUN_0802553e(uVar14,7,0x36,0);
  }
  local_2c = FUN_08027fb8();
  local_30 = &DAT_080165bc;
  local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
  local_38 = (char *)DAT_080165c0;
  uVar15 = FUN_0802553e(uVar14,7,0x126,0);
  puVar3 = DAT_080165c4;
  DAT_080165c4[1] = uVar15;
  local_38 = (char *)0x15e;
  local_34 = 0x50;
  uVar15 = FUN_08006220(param_1,1,9,0xe6);
  local_2c = FUN_08027d38(0x27,0x27);
  FUN_0803157c(uVar15,local_2c,0);
  FUN_08031594(uVar15,0xff,0);
  FUN_08031696(uVar15,0xf,0);
  FUN_0802d97a(uVar15,DAT_08016584,0);
  local_2c = FUN_08027fb8();
  local_30 = &DAT_080165e8;
  local_34._0_2_ = (undefined2)local_2c;
  local_38 = (char *)DAT_0801658c;
  FUN_0802553e(uVar15,7,6,0);
  cVar18 = *(char *)(iVar2 + 3);
  if (cVar18 == '\0') {
    local_2c = FUN_08027fb8();
    local_30 = &LAB_080165ec;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)uVar10;
    FUN_0802553e(uVar15,7,0x36,0);
  }
  else if (cVar18 == '\x01') {
    local_2c = FUN_08027fb8();
    local_30 = s_Rear_Curtain_080165fc;
    local_34._0_2_ = (undefined2)local_2c;
    local_38 = (char *)uVar11;
    FUN_0802553e(uVar15,7,0x36,0);
  }
  local_2c = FUN_08027fb8();
  local_30 = &DAT_080165bc;
  local_34 = CONCAT22(local_34._2_2_,(undefined2)local_2c);
  local_38 = (char *)DAT_080165c0;
  uVar10 = FUN_0802553e(uVar15,7,0x126,0);
  puVar3[2] = uVar10;
  if (*DAT_0801660c != '\x01') {
    FUN_0802da22(uVar15,1);
  }
  local_38 = (char *)0x4;
  local_34 = 0x5c;
  uVar10 = FUN_08010acc(param_1,1,0,0xb2);
  local_2c = FUN_08027fb8();
  FUN_0803157c(uVar10,local_2c,0);
  FUN_08031594(uVar10,0xff,0);
  puVar4 = DAT_080165c4;
  DAT_080165c4[-0x10] = uVar13;
  puVar4[-0xf] = uVar14;
  puVar4[-0xe] = uVar15;
  uVar12 = (uint)*DAT_08016610;
  if (uVar12 == 0) {
    FUN_0802e2d2(*puVar3,1);
    FUN_0802da22(puVar3[1],1);
    uVar12 = FUN_0802da22(puVar3[2],1);
    return uVar12;
  }
  if (uVar12 == 1) {
    FUN_0802e2d2(puVar3[1],1);
    FUN_0802da22(*puVar3,1);
    uVar12 = FUN_0802da22(puVar3[2],1);
    return uVar12;
  }
  if (uVar12 == 2) {
    FUN_0802e2d2(puVar3[2],1);
    FUN_0802da22(*puVar3,1);
    uVar12 = FUN_0802da22(puVar3[1],1);
    return uVar12;
  }
  return uVar12;
}

