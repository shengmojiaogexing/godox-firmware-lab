// 0x0800b120 FUN_0800b120 size=960

void FUN_0800b120(int param_1)

{
  byte bVar1;
  char cVar2;
  char *pcVar3;
  undefined4 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  uint uVar12;
  uint uVar13;
  undefined4 uVar14;
  
  iVar8 = DAT_0800b4e8;
  iVar7 = DAT_0800b4e0;
  uVar13 = 0xff;
  if ((*(char *)(DAT_0800b4e0 + 0xe) == '\0') && (*(char *)(DAT_0800b4e0 + 0xd) == '\0')) {
    uVar12 = 0;
    do {
      if (*(int *)(DAT_0800b4e4 + uVar12 * 0x74) == *(int *)(param_1 + 0xc)) {
        uVar13 = uVar12;
        break;
      }
      uVar12 = uVar12 + 1 & 0xff;
    } while (uVar12 < 0x10);
    if (uVar13 < 0x10) {
      if (*(char *)(param_1 + 8) == '\x04') {
        if (*(char *)(DAT_0800b4e0 + 3) == '\0') {
          uVar9 = FUN_0803205a();
          uVar9 = FUN_08010acc(uVar9,9,0,0,0x170,0x1c0);
          *(undefined4 *)(iVar7 + 0x34) = uVar9;
          uVar9 = FUN_08027a78();
          FUN_0803157c(*(undefined4 *)(iVar7 + 0x34),uVar9,0);
          FUN_08031594(*(undefined4 *)(iVar7 + 0x34),0x99,0);
          FUN_0802d97a(*(undefined4 *)(iVar7 + 0x34),DAT_0800b4ec,0);
          uVar10 = FUN_08010acc(*(undefined4 *)(iVar7 + 0x34),9,0,0x14,0x140,0x118);
          uVar9 = FUN_08027c88(0x27,0x24);
          FUN_0803157c(uVar10,uVar9,0);
          FUN_08031594(uVar10,0xff,0);
          FUN_08031696(uVar10,0x19,0);
          uVar5 = 0;
          uVar11 = FUN_08010acc(uVar10,2,0,0x23,0x118,0x78);
          uVar4 = DAT_0800b4f8;
          uVar9 = DAT_0800b4f4;
          pcVar3 = DAT_0800b4f0;
          if (*DAT_0800b4f0 == '\0') {
            uVar6 = FUN_08027f68();
            FUN_0802553e(uVar11,9,0,0,uVar9,CONCAT22(uVar5,uVar6),&DAT_0800b4fc);
          }
          else if (*DAT_0800b4f0 == '\x01') {
            uVar6 = FUN_08027f68();
            uVar14 = CONCAT22(uVar5,uVar6);
            FUN_0802553e(uVar11,2,0,10,uVar4,uVar14,s_Turn_on_0800b51c);
            uVar6 = (undefined2)((uint)uVar14 >> 0x10);
            uVar5 = FUN_08027f68();
            uVar14 = CONCAT22(uVar6,uVar5);
            FUN_0802553e(uVar11,2,0,0x32,uVar4,uVar14,s_the_modeling_lamp_0800b524);
            uVar6 = (undefined2)((uint)uVar14 >> 0x10);
            uVar5 = FUN_08027f68();
            FUN_0802553e(uVar11,2,0,0x5a,uVar4,CONCAT22(uVar6,uVar5),s_main_switch_0800b538);
          }
          uVar5 = 0;
          uVar11 = FUN_08006220(uVar10,1,0x14,0xc0,0x82,0x40);
          iVar8 = DAT_0800b4e0;
          *(undefined4 *)(DAT_0800b4e0 + 0x84) = uVar11;
          uVar11 = FUN_08027c88(0x52,0x52);
          FUN_0803157c(*(undefined4 *)(iVar8 + 0x84),uVar11,0);
          FUN_08031594(*(undefined4 *)(iVar8 + 0x84),0xcc,0);
          FUN_08031696(*(undefined4 *)(iVar8 + 0x84),0x40,0);
          FUN_080315c0(*(undefined4 *)(iVar8 + 0x84),0);
          FUN_080315b6(*(undefined4 *)(iVar8 + 0x84),0);
          FUN_080315b6(*(undefined4 *)(iVar8 + 0x84),0,4);
          FUN_080315c0(*(undefined4 *)(iVar8 + 0x84),2,4);
          uVar11 = FUN_08027f68();
          FUN_0803159e(*(undefined4 *)(iVar8 + 0x84),uVar11,4);
          cVar2 = *pcVar3;
          if (cVar2 == '\0') {
            uVar6 = FUN_08027f68();
            FUN_0802553e(*(undefined4 *)(iVar8 + 0x84),9,0,0,uVar9,CONCAT22(uVar5,uVar6),
                         &DAT_0800b544);
          }
          else if (cVar2 == '\x01') {
            uVar6 = FUN_08027f68();
            FUN_0802553e(*(undefined4 *)(iVar8 + 0x84),9,0,0,uVar4,CONCAT22(uVar5,uVar6),
                         &DAT_0800b548);
          }
          uVar5 = 0;
          uVar10 = FUN_08006220(uVar10,1,0xaa,0xc0,0x82,0x40);
          *(undefined4 *)(iVar8 + 0x88) = uVar10;
          uVar10 = FUN_08027c88(0x52,0x52);
          FUN_0803157c(*(undefined4 *)(iVar8 + 0x88),uVar10,0);
          FUN_08031594(*(undefined4 *)(iVar8 + 0x88),0xcc,0);
          FUN_08031696(*(undefined4 *)(iVar8 + 0x88),0x40,0);
          FUN_080315c0(*(undefined4 *)(iVar8 + 0x88),0);
          FUN_080315b6(*(undefined4 *)(iVar8 + 0x88),0,4);
          FUN_080315c0(*(undefined4 *)(iVar8 + 0x88),2,4);
          uVar10 = FUN_08027f68();
          FUN_0803159e(*(undefined4 *)(iVar8 + 0x88),uVar10,4);
          cVar2 = *pcVar3;
          if (cVar2 == '\0') {
            uVar6 = FUN_08027f68();
            FUN_0802553e(*(undefined4 *)(iVar8 + 0x88),9,0,0,uVar9,CONCAT22(uVar5,uVar6),
                         &DAT_0800b54c);
          }
          else if (cVar2 == '\x01') {
            uVar6 = FUN_08027f68();
            FUN_0802553e(*(undefined4 *)(iVar8 + 0x88),9,0,0,uVar4,CONCAT22(uVar5,uVar6),
                         &DAT_0800b550);
          }
          FUN_0802d97a(*(undefined4 *)(iVar8 + 0x84),DAT_0800b554,0);
          FUN_0802d97a(*(undefined4 *)(iVar8 + 0x88),DAT_0800b554,0);
          *(undefined1 *)(iVar7 + 2) = 1;
        }
        else {
          iVar7 = DAT_0800b4e8 + uVar13 * 6;
          bVar1 = *(byte *)(iVar7 + 4);
          if (((int)((uint)bVar1 << 0x18) < 0) && ((*(byte *)(iVar7 + 5) & 1) != 0)) {
            *(byte *)(iVar7 + 4) = bVar1 & 0x7f;
          }
          else if (((int)((uint)bVar1 << 0x18) < 0) && ((*(byte *)(iVar7 + 5) & 1) == 0)) {
            *(undefined1 *)(iVar7 + 5) = 1;
          }
          else if (-1 < (int)((uint)bVar1 << 0x18)) {
            *(byte *)(iVar7 + 4) = bVar1 | 0x80;
            *(undefined1 *)(iVar7 + 5) = 0;
          }
        }
      }
      else if ((*(char *)(param_1 + 8) == '\x0e') &&
              (iVar7 = FUN_0802fcb6(*(undefined4 *)(param_1 + 4),8), iVar7 != 0)) {
        iVar8 = iVar8 + uVar13 * 6;
        bVar1 = *(byte *)(iVar8 + 4);
        if (((int)((uint)bVar1 << 0x18) < 0) && ((*(byte *)(iVar8 + 5) & 1) != 0)) {
          *(byte *)(iVar8 + 4) = bVar1 & 0x7f;
        }
        else if (((int)((uint)bVar1 << 0x18) < 0) && ((*(byte *)(iVar8 + 5) & 1) == 0)) {
          *(undefined1 *)(iVar8 + 5) = 1;
        }
        else if (-1 < (int)((uint)bVar1 << 0x18)) {
          *(byte *)(iVar8 + 4) = bVar1 | 0x80;
          *(undefined1 *)(iVar8 + 5) = 0;
        }
      }
    }
  }
  return;
}

