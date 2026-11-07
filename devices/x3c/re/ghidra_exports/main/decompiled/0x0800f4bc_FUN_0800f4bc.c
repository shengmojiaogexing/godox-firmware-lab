// 0x0800f4bc FUN_0800f4bc size=2078

void FUN_0800f4bc(undefined4 param_1)

{
  char *pcVar1;
  undefined4 *puVar2;
  byte *pbVar3;
  byte *pbVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  undefined2 uVar7;
  undefined2 uVar8;
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
  int iVar20;
  uint uVar21;
  undefined1 auStack_34 [12];
  undefined4 local_28;
  
  *DAT_0800f8c0 = 1;
  *DAT_0800f8c4 = 0;
  local_28 = param_1;
  uVar9 = FUN_08010acc(param_1,1,0x1a,0x4b,0x13c,0xb);
  uVar10 = FUN_08010acc(uVar9,7,0,0,1,8);
  FUN_0802d97a(uVar9,DAT_0800f8c8,0);
  uVar11 = FUN_08027f90();
  FUN_0803157c(uVar10,uVar11,0);
  FUN_08031594(uVar10,0xff,0);
  FUN_08031696(uVar9,8,0);
  FUN_08031696(uVar10,8,0);
  FUN_080315c0(uVar9,1,0);
  uVar11 = FUN_08027ce0(0x27,0x27);
  FUN_0803157c(uVar9,uVar11,0);
  FUN_08031594(uVar9,0xff,0);
  uVar11 = FUN_08010acc(local_28,2,0,0,0x154,0x60);
  FUN_08031696(uVar11,0xf,0);
  FUN_080315c0(uVar11,2,0);
  FUN_080315b6(uVar11,0);
  FUN_080315b6(uVar11,0xff,4);
  FUN_080315b6(uVar11,0xff,8);
  uVar9 = FUN_08027f90();
  FUN_0803159e(uVar11,uVar9,4);
  uVar9 = FUN_08027ce0(0xfd,0x9f,10);
  FUN_0803159e(uVar11,uVar9,8);
  FUN_0802d97a(uVar11,DAT_0800f8c8,0);
  uVar8 = 0;
  uVar12 = FUN_08015aa8(local_28,2,0,10,*DAT_0800f8cc / 10 - 2 & 0xffff,0x154,0x52,uVar9);
  FUN_0802d97a(uVar12,DAT_0800f8d0,0,uVar10);
  uVar7 = FUN_08027f90();
  uVar9 = DAT_0800f8d8;
  uVar13 = CONCAT22(uVar8,uVar7);
  FUN_0802553e(local_28,1,0x1d,0x13,DAT_0800f8d8,uVar13,&DAT_0800f8d4);
  uVar8 = (undefined2)((uint)uVar13 >> 0x10);
  uVar7 = FUN_08027f90();
  uVar13 = CONCAT22(uVar8,uVar7);
  FUN_0802553e(local_28,3,0xffffffe3,0x13,uVar9,uVar13,&DAT_0800f8dc);
  uVar8 = (undefined2)((uint)uVar13 >> 0x10);
  uVar7 = FUN_08027f90();
  uVar9 = CONCAT22(uVar8,uVar7);
  uVar13 = FUN_0802553e(local_28,2,0,0x10,DAT_0800f8e4,uVar9,&DAT_0800f8e0);
  uVar8 = (undefined2)((uint)uVar9 >> 0x10);
  uVar7 = FUN_08027f90();
  uVar9 = DAT_0800f8ec;
  uVar15 = CONCAT22(uVar8,uVar7);
  uVar14 = FUN_0802553e(local_28,2,0,0xb,DAT_0800f8ec,uVar15,&DAT_0800f8e8);
  uVar7 = (undefined2)((uint)uVar15 >> 0x10);
  FUN_0802db62(uVar13,uVar14,0x11,0xffffffff,10);
  uVar15 = FUN_08031c80(local_28);
  FUN_080315c0(uVar15,0);
  FUN_0802db44(uVar15,1,10,0x68);
  uVar16 = FUN_08027f90();
  FUN_080316b4(uVar15,uVar16,0);
  uVar16 = FUN_08027aa0();
  FUN_0803157c(uVar15,uVar16,0);
  uVar16 = FUN_08027aa0();
  FUN_0803157c(uVar15,uVar16,0x40000);
  FUN_080315b6(uVar15,0,4);
  FUN_080315b6(uVar15,0,8);
  FUN_08031682(uVar15,0x28,0);
  FUN_08031594(uVar15,0,0x40000);
  FUN_08031594(uVar15,0xff,0);
  FUN_08031696(uVar15,0xf,0);
  FUN_080316cc(uVar15,uVar9,0);
  FUN_08031ee0(uVar15,DAT_0800f8f0,0);
  FUN_08032014(uVar15,3);
  FUN_080316d6(uVar15,0xf,0);
  FUN_080313b2(uVar15,0xa8,0xa9);
  FUN_0802d97a(uVar15,DAT_0800f8f4,0);
  FUN_0802d97a(uVar15,DAT_0800f8f8,0x12,0);
  FUN_0802d97a(uVar15,DAT_0800f8f8,0x14,0);
  FUN_0802d97a(uVar15,DAT_0800f8f8,0x19,0);
  FUN_080316aa(uVar15,2,0);
  uVar9 = FUN_08027f90();
  FUN_08031604(uVar15,uVar9,4);
  uVar9 = FUN_08027ce0(0xfd,0x9f,10);
  FUN_08031604(uVar15,uVar9,8);
  FUN_08031626(uVar15,6,4);
  FUN_08031626(uVar15,6,8);
  FUN_0803161c(uVar15,0xff,0);
  uVar9 = FUN_08027ce0(0x26,0x26);
  FUN_08031604(uVar15,uVar9,0);
  FUN_0803161c(uVar15,0xff,4);
  FUN_0803161c(uVar15,0xff,8);
  uVar9 = DAT_0800f900;
  pcVar1 = DAT_0800f8fc;
  if (*DAT_0800f8fc == '\0') {
    uVar8 = FUN_08027f90();
    uVar16 = CONCAT22(uVar7,uVar8);
    FUN_0802553e(local_28,1,0x85,0xb2,uVar9,uVar16,&DAT_0800f904);
    uVar7 = (undefined2)((uint)uVar16 >> 0x10);
  }
  else if (*DAT_0800f8fc == '\x01') {
    uVar8 = FUN_08027f90();
    uVar16 = CONCAT22(uVar7,uVar8);
    FUN_0802553e(local_28,1,0x88,0xb2,DAT_0800f914,uVar16,s_Times_0800f90c);
    uVar7 = (undefined2)((uint)uVar16 >> 0x10);
  }
  uVar16 = FUN_08031c80(local_28);
  FUN_0802db44(uVar16,1,0xbf,0x68);
  FUN_080315c0(uVar16,0);
  uVar17 = FUN_08027f90();
  FUN_080316b4(uVar16,uVar17,0);
  uVar17 = FUN_08027aa0();
  FUN_0803157c(uVar16,uVar17,0);
  uVar17 = FUN_08027aa0();
  FUN_0803157c(uVar16,uVar17,0x40000);
  FUN_08031594(uVar16,0,0x40000);
  uVar17 = FUN_08027f90();
  FUN_08031604(uVar16,uVar17,4);
  uVar17 = FUN_08027ce0(0xfd,0x9f,10);
  FUN_08031604(uVar16,uVar17,8);
  FUN_08031626(uVar16,6,4);
  FUN_08031626(uVar16,6,8);
  FUN_0803161c(uVar16,0xff,0);
  uVar17 = FUN_08027ce0(0x26,0x26);
  FUN_08031604(uVar16,uVar17,0);
  FUN_0803161c(uVar16,0xff,4);
  FUN_0803161c(uVar16,0xff,8);
  FUN_080316aa(uVar16,2,0);
  FUN_08031682(uVar16,0x28,0);
  FUN_08031594(uVar16,0xff,0);
  FUN_08031696(uVar16,0xf,0);
  FUN_080316cc(uVar16,DAT_0800fd38,0);
  FUN_08031ee0(uVar16,*(undefined4 *)(DAT_0800fd3c + 8),0);
  FUN_08032014(uVar16,3);
  FUN_080316d6(uVar16,0xf,0);
  FUN_080313b2(uVar16,0xa8,0xa9);
  FUN_0802d97a(uVar16,DAT_0800fd40,0);
  FUN_0802d97a(uVar16,DAT_0800fd44,0x12,0);
  FUN_0802d97a(uVar16,DAT_0800fd44,0x14,0);
  FUN_0802d97a(uVar16,DAT_0800fd44,0x19,0);
  if (*pcVar1 == '\0') {
    uVar8 = FUN_08027f90();
    FUN_0802553e(local_28,1,0x13b,0xb2,uVar9,CONCAT22(uVar7,uVar8),&DAT_0800fd48);
  }
  else if (*pcVar1 == '\x01') {
    uVar8 = FUN_08027f90();
    FUN_0802553e(local_28,1,0x143,0xb2,DAT_0800fd54,CONCAT22(uVar7,uVar8),&DAT_0800fd50);
  }
  uVar9 = FUN_08010acc(local_28,2,0,0x122,0x168,0x74);
  puVar2 = DAT_0800fd58;
  uVar21 = 0;
  do {
    uVar8 = 0;
    uVar17 = FUN_08006220(uVar9,7,(int)(short)((short)uVar21 * 0x94),0,0x8c,0x74);
    uVar18 = FUN_08027ce0(0x27,0x27);
    FUN_0803157c(uVar17,uVar18,0);
    FUN_08031594(uVar17,0xff,0);
    FUN_08031696(uVar17,0xf,0);
    FUN_08004c48(auStack_34,10,&DAT_0800fd5c,uVar21 + 0x41);
    uVar7 = FUN_08027f90();
    FUN_0802553e(uVar17,9,0,0xfffffffb,DAT_0800fd38,CONCAT22(uVar8,uVar7),auStack_34);
    uVar18 = FUN_08010acc(uVar17,2,0,0x5a,0x21,8);
    uVar19 = FUN_08027f90();
    FUN_0803157c(uVar18,uVar19,0);
    FUN_08031594(uVar18,0xff,0);
    FUN_08031696(uVar18,4,0);
    puVar2[uVar21 + 7] = uVar17;
    FUN_0802d97a(uVar17,DAT_0800fd60,0);
    uVar21 = uVar21 + 1 & 0xff;
  } while (uVar21 < 5);
  *puVar2 = uVar11;
  puVar2[1] = uVar10;
  puVar2[2] = uVar12;
  puVar2[3] = uVar13;
  puVar2[4] = uVar14;
  puVar2[6] = uVar16;
  puVar2[5] = uVar15;
  pbVar4 = DAT_0800fd68;
  pbVar3 = DAT_0800fd64;
  FUN_0802782e(uVar12,(int)((uint)*DAT_0800fd64 - (uint)*DAT_0800fd68) / 10,0);
  FUN_0800ff7c(*pbVar4,auStack_34);
  FUN_0802d33c(puVar2[4],auStack_34);
  if (*DAT_0800fd6c == '\0') {
    FUN_0802e2d2(puVar2[3],1);
  }
  else if (*DAT_0800fd6c == '\x01') {
    FUN_0802da22(puVar2[3],1);
  }
  FUN_0802db62(puVar2[3],puVar2[4],0x11,0xffffffff,10);
  puVar5 = DAT_0800fd70;
  uVar9 = FUN_0800adf6(*DAT_0800fd70);
  FUN_08031fba(puVar2[5],uVar9,0);
  puVar6 = DAT_0800fd74;
  uVar9 = FUN_0800adc4(*DAT_0800fd74);
  FUN_08031fba(puVar2[6],uVar9,0);
  iVar20 = (uint)*pbVar3 - (uint)*pbVar4;
  uVar21 = ((short)iVar20 * 0x13c) / (int)(*pbVar3 - 0x14) & 0xffff;
  FUN_0802782e(puVar2[2],iVar20 / 10 & 0xff,0);
  if (uVar21 < 2) {
    uVar21 = 1;
  }
  FUN_080316fe(puVar2[1],uVar21);
  pbVar3 = DAT_0800fd78;
  uVar21 = 0;
  do {
    uVar9 = FUN_0802ebc8(puVar2[uVar21 + 7],1);
    uVar10 = FUN_0802ebc8(puVar2[uVar21 + 7],0);
    if ((1 << uVar21 & (uint)*pbVar3) == 0) {
      uVar11 = FUN_08027aa0();
      FUN_0803157c(uVar9,uVar11,0);
      uVar9 = FUN_08027ce0(0x37,0x37);
      FUN_080316b4(uVar10,uVar9,0);
    }
    else {
      if (*DAT_0800fd7c != '\0') {
        switch(uVar21) {
        case 0:
          uVar11 = FUN_08027ce0(0xff,0);
          FUN_0803157c(uVar9,uVar11,0);
          break;
        case 1:
          uVar11 = FUN_08027ce0(0,0xff);
          FUN_0803157c(uVar9,uVar11,0);
          break;
        case 2:
          uVar11 = FUN_08027ce0(0,0,0xff);
          FUN_0803157c(uVar9,uVar11,0);
          break;
        case 3:
          uVar11 = FUN_08027ce0(0,0xff);
          FUN_0803157c(uVar9,uVar11,0);
          break;
        case 4:
          uVar11 = FUN_08027ce0(0xff,0);
          FUN_0803157c(uVar9,uVar11,0);
        }
      }
      uVar9 = FUN_08027f90();
      FUN_080316b4(uVar10,uVar9,0);
    }
    uVar21 = uVar21 + 1 & 0xff;
  } while (uVar21 < 5);
  *DAT_0800fd80 = *pbVar3;
  *DAT_0800fd84 = *pbVar4;
  *DAT_0800fd88 = *puVar5;
  *DAT_0800fd8c = *puVar6;
  return;
}

