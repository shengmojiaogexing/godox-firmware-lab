// 0x0800981c FUN_0800981c size=2508

void FUN_0800981c(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  uint uVar11;
  undefined4 *puVar12;
  undefined4 *puVar13;
  uint uVar14;
  undefined4 *puVar15;
  undefined4 uVar16;
  undefined1 auStack_40 [12];
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  
  iVar1 = DAT_08009c20;
  *(undefined2 *)(DAT_08009c20 + 0x24) = 0;
  *(undefined2 *)(iVar1 + 0x26) = 0;
  *(undefined2 *)(iVar1 + 0x28) = 0;
  local_28 = param_1;
  FUN_0802e2d2(param_1,0x300);
  uVar5 = FUN_08010acc(local_28,2,0,0,0x170,0x138);
  iVar1 = DAT_08009c20;
  *(undefined4 *)(DAT_08009c20 + 0x8c) = uVar5;
  FUN_08031384(uVar5,2);
  FUN_08031330(*(undefined4 *)(iVar1 + 0x8c),0xc);
  FUN_08031594(*(undefined4 *)(iVar1 + 0x8c),0xff,0x10000);
  uVar5 = FUN_08027f68();
  FUN_0803157c(*(undefined4 *)(iVar1 + 0x8c),uVar5,0x10000);
  FUN_080316e0(*(undefined4 *)(iVar1 + 0x8c),3,0x10000);
  FUN_0802d97a(*(undefined4 *)(iVar1 + 0x8c),DAT_08009c24,0);
  FUN_0803166e(*(undefined4 *)(iVar1 + 0x8c),7,0);
  uVar11 = 0;
  do {
    uVar6 = FUN_08010acc(*(undefined4 *)(DAT_08009c20 + 0x8c),2,4,uVar11 * 0x68,0x168,100);
    FUN_0802d97a(uVar6,DAT_08009c28,0);
    FUN_080315b6(uVar6,0xff,4);
    uVar5 = FUN_08027f68();
    FUN_0803159e(uVar6,uVar5,4);
    FUN_080315c0(uVar6,2,0);
    FUN_080315c0(uVar6,2,4);
    FUN_0802da22(uVar6,0x400);
    uVar5 = FUN_08027c88(0xfd,0x9f,10);
    FUN_0803159e(uVar6,uVar5,8);
    puVar13 = (undefined4 *)(DAT_08009c2c + uVar11 * 0x30);
    *puVar13 = uVar6;
    FUN_08031696(uVar6,0x14,0);
    uVar7 = FUN_08010acc(uVar6,7,0x21,0,0x143,0x5f);
    FUN_0802da22(uVar7,0x400);
    uVar5 = FUN_08027c88(0x27,0x27);
    FUN_0803157c(uVar7,uVar5,0);
    FUN_08031594(uVar7,0xff,0);
    FUN_080315b6(uVar7,0xff,0);
    uVar5 = FUN_08027c88(0x87,0xce,0xfa);
    FUN_0803159e(uVar7,uVar5,2);
    uVar5 = FUN_08027c88(0xfd,0x9f,10);
    FUN_0803159e(uVar7,uVar5,8);
    FUN_08031696(uVar7,0x14,0);
    FUN_0802da22(uVar7,0x4000);
    puVar13[10] = uVar6;
    uVar5 = FUN_08010acc(uVar7,4,0,0,10,8);
    FUN_08031696(uVar5,10,0);
    FUN_0802e2d2(uVar5,2);
    FUN_0802e2d2(uVar5,0x300);
    uVar8 = FUN_08027c88(0xff,0xdc);
    FUN_0803157c(uVar5,uVar8,0);
    FUN_08031594(uVar5,0x14,0);
    puVar13[9] = uVar5;
    uVar8 = FUN_0800ae28();
    uVar4 = 0;
    uVar8 = FUN_0803b8e2(uVar6,7,0x2c,0,uVar8,0x138,0x56);
    puVar13[8] = uVar8;
    FUN_0802d97a(uVar8,DAT_08009c30,0,uVar5);
    uVar3 = FUN_08027f68();
    uVar5 = DAT_08009c38;
    uVar8 = CONCAT22(uVar4,uVar3);
    local_30 = FUN_0802553e(uVar7,7,0x6c,0xfffffffb,DAT_08009c38,uVar8,&DAT_08009c34);
    uVar4 = (undefined2)((uint)uVar8 >> 0x10);
    uVar3 = FUN_08027f68();
    uVar8 = DAT_08009c40;
    uVar10 = CONCAT22(uVar4,uVar3);
    uVar9 = FUN_0802553e(uVar7,2,0xfffffff6,0x14,DAT_08009c40,uVar10,&DAT_08009c3c);
    uVar4 = (undefined2)((uint)uVar10 >> 0x10);
    uVar3 = FUN_08027f68();
    uVar10 = CONCAT22(uVar4,uVar3);
    local_34 = FUN_0802553e(uVar7,7,0xef,0xfffffffb,uVar5,uVar10,&DAT_08009c44);
    uVar4 = (undefined2)((uint)uVar10 >> 0x10);
    FUN_0802db62(local_30,uVar9,0x11,0xffffffff,10);
    FUN_0802db62(local_34,uVar9,0x14,1,8);
    uVar3 = FUN_08027c88(0x2a,0xe2,0x46);
    uVar5 = FUN_0802553e(uVar6,7,0x78,0,uVar8,CONCAT22(uVar4,uVar3),&DAT_08009c44);
    local_2c = FUN_08010acc(uVar6,7,0,0,0x37,0x60);
    uVar4 = 0;
    uVar7 = FUN_08010acc(uVar6,7,0,0,0x164,0x5f);
    FUN_0802da22(uVar7,0x4000);
    uVar10 = FUN_08027a78();
    FUN_0803157c(uVar7,uVar10,0);
    FUN_08031594(uVar7,0x66,0);
    FUN_08031696(uVar7,0x14,0);
    uVar3 = FUN_08027f68();
    uVar10 = CONCAT22(uVar4,uVar3);
    FUN_0802553e(uVar7,9,3,3,uVar8,uVar10,&DAT_08009c4c);
    uVar4 = (undefined2)((uint)uVar10 >> 0x10);
    FUN_0802d97a(uVar7,DAT_08009c50,0,uVar6);
    uVar3 = FUN_08027c88(0x52,0x52);
    uVar8 = FUN_0802553e(local_2c,7,0,0,DAT_08009c58,CONCAT22(uVar4,uVar3),&DAT_08009c54);
    FUN_0802e2d2(uVar8,0x10);
    FUN_0802da22(local_2c,0x4000);
    uVar3 = 0;
    uVar8 = FUN_08010acc(local_2c,2,0xfffffffe,0x48,0x1c,9);
    FUN_08031696(uVar8,6,0);
    if (*DAT_08009c5c == '\0') goto switchD_08009bf2_default;
    switch(uVar11) {
    case 0:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0xff,0);
      FUN_0803157c(uVar8,uVar10,0);
    default:
switchD_08009bf2_default:
      break;
    case 1:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0,0xff);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 2:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0,0,0xff);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 3:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0,0xff);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 4:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0xff,0);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 5:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0xff,0xff,0);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 6:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0xff,0x66,0);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 7:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0,0xc1,0x77);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 8:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0x80,0,0xff);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 9:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0xff,0,0x74);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 10:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0xd8,0xb7,0xff);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 0xb:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0,0xff,0xce);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 0xc:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0,0x99,0xff);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 0xd:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0xc9,0xff,0xad);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 0xe:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0xff,0x80);
      FUN_0803157c(uVar8,uVar10,0);
      break;
    case 0xf:
      FUN_08031594(uVar8,0xff,0);
      uVar10 = FUN_08027c88(0xff,0xb7,0x80);
      FUN_0803157c(uVar8,uVar10,0);
    }
    if (uVar11 < 6) {
      FUN_08004c48(auStack_40,10,&DAT_0800a23c,uVar11 + 0x41);
    }
    else {
      FUN_08004c48(auStack_40,10,&DAT_0800a23c,uVar11 + 0x2a);
    }
    uVar4 = FUN_08027f68();
    uVar16 = CONCAT22(uVar3,uVar4);
    uVar10 = FUN_0802553e(local_2c,9,0xfffffffe,0,DAT_0800a240,uVar16,auStack_40);
    uVar3 = (undefined2)((uint)uVar16 >> 0x10);
    if (*DAT_0800a244 == '\0') {
      FUN_0802db44(uVar10,9,0xfffffffe);
    }
    FUN_0802da22(uVar10,0x4000);
    uVar4 = FUN_08027f68();
    uVar6 = FUN_0802553e(uVar6,1,0x3b,0xc,DAT_0800a24c,CONCAT22(uVar3,uVar4),&DAT_0800a248);
    iVar1 = DAT_0800a250;
    puVar13[0xb] = uVar6;
    puVar13[1] = uVar8;
    puVar13[3] = local_30;
    puVar13[4] = uVar9;
    puVar13[5] = local_34;
    puVar13[6] = uVar5;
    puVar13[7] = uVar7;
    puVar13[2] = uVar10;
    puVar13 = DAT_0800a254;
    uVar11 = uVar11 + 1 & 0xff;
    if (0xf < uVar11) {
      uVar4 = 0;
      uVar5 = FUN_08006220(DAT_0800a254[3],2,0,0,0x168,0x5a);
      *puVar13 = uVar5;
      uVar5 = FUN_08027c88(0x27,0x27);
      FUN_0803157c(*puVar13,uVar5,0);
      FUN_08031594(*puVar13,0xff,0);
      FUN_08031696(*puVar13,0xf,0);
      FUN_080315dc(*puVar13,0x58,0x20);
      FUN_080316e0(*puVar13,0x168,0x20);
      FUN_080315c0(*puVar13,2,0);
      FUN_080315b6(*puVar13,0);
      uVar3 = FUN_08027f68();
      FUN_0802553e(*puVar13,9,0,0,DAT_0800a258,CONCAT22(uVar4,uVar3),&DAT_0800a248);
      FUN_0802d97a(*puVar13,DAT_0800a25c,0,local_28);
      puVar13[-0x10] = *puVar13;
      uVar8 = FUN_08010acc(local_28,2,0,0x142,0x168,0x54);
      FUN_0802e2d2(uVar8,0x10);
      uVar5 = FUN_08010acc(local_28,2,0,0x142,0x168,0x40);
      FUN_0802e2d2(uVar5,2);
      FUN_080315b6(uVar5,0xff,2);
      uVar6 = FUN_08027f68();
      FUN_0803159e(uVar5,uVar6,2);
      FUN_080315c0(uVar5,2,0);
      FUN_080315b6(uVar5,0xff,0);
      uVar6 = FUN_08027f68();
      FUN_0803159e(uVar5,uVar6,2);
      FUN_08031696(uVar5,0xf,0);
      uVar6 = FUN_08027c88(0xfd,0x9f,10);
      FUN_0803159e(uVar5,uVar6,8);
      FUN_0802d97a(uVar5,DAT_0800a260,0);
      FUN_080315c0(uVar8,2,0);
      FUN_080315b6(uVar8,0xff,0);
      FUN_08031696(uVar8,0xf,0);
      puVar13[-0x11] = uVar5;
      uVar4 = 0;
      uVar5 = FUN_08015aa8(uVar8,9,0,0,0x3c,0x15e,0x52);
      thunk_FUN_0802782e(uVar5,0x1e,0);
      FUN_0802d97a(uVar5,DAT_0800a264,0);
      uVar3 = FUN_08027f68();
      uVar5 = DAT_0800a268;
      uVar6 = CONCAT22(uVar4,uVar3);
      FUN_0802553e(uVar8,1,0x10,0x10,DAT_0800a268,uVar6,&DAT_08009c54);
      uVar4 = (undefined2)((uint)uVar6 >> 0x10);
      uVar3 = FUN_08027f68();
      uVar6 = CONCAT22(uVar4,uVar3);
      FUN_0802553e(uVar8,1,0x128,0x10,uVar5,uVar6,&DAT_0800a248);
      uVar4 = (undefined2)((uint)uVar6 >> 0x10);
      uVar3 = FUN_08027f68();
      uVar5 = FUN_0802553e(uVar8,9,0,0xfffffff6,DAT_0800a26c,CONCAT22(uVar4,uVar3),&DAT_0800a248);
      puVar13[-4] = uVar5;
      iVar2 = DAT_0800a270;
      uVar11 = 0;
      do {
        puVar12 = (undefined4 *)(iVar1 + uVar11 * 0x30);
        puVar15 = (undefined4 *)(iVar2 + uVar11 * 6);
        FUN_08007524(*puVar15,*(undefined2 *)(puVar15 + 1),puVar12[8],puVar12[9],puVar12[6],
                     puVar12[3],puVar12[4],puVar12[5],*puVar15);
        if ((int)((uint)*(byte *)((int)puVar15 + 3) << 0x18) < 0) {
          FUN_0802e2d2(*puVar12,1);
          if ((int)((uint)*(byte *)((int)puVar15 + 3) << 0x19) < 0) {
            FUN_0802da22(puVar12[7],1);
          }
          else {
            FUN_0802e2d2(puVar12[7],1);
            FUN_0802da22(puVar12[6],1);
            FUN_0802da22(puVar12[3],1);
            FUN_0802da22(puVar12[4],1);
            FUN_0802da22(puVar12[5],1);
          }
        }
        else {
          FUN_0802da22(*puVar12,1);
        }
        if ((*(char *)((int)puVar13 + -0x7d) == '\0') ||
           (-1 < (int)((uint)*(byte *)(puVar15 + 1) << 0x18))) {
          FUN_0802da22(puVar12[0xb],1);
        }
        else {
          FUN_0802e2d2(puVar12[0xb],1);
        }
        uVar11 = uVar11 + 1 & 0xff;
      } while (uVar11 < 0x10);
      uVar14 = 0;
      uVar11 = 0;
      do {
        if ((int)((uint)*(byte *)(iVar2 + uVar11 * 6 + 3) << 0x18) < 0) {
          FUN_0802db44(*(undefined4 *)(iVar1 + uVar11 * 0x30),2,0,uVar14 * 0x68);
          uVar14 = uVar14 + 1 & 0xff;
        }
        uVar11 = uVar11 + 1 & 0xff;
      } while (uVar11 < 0x10);
      FUN_0802db44(*puVar13,2,0,uVar14 * 0x68 + 2);
      return;
    }
  } while( true );
}

