// 0x080103c8 FUN_080103c8 size=1574

void FUN_080103c8(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  short sVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  byte *pbVar9;
  undefined4 *puVar10;
  byte *pbVar11;
  char *pcVar12;
  short *psVar13;
  char *pcVar14;
  undefined4 *puVar15;
  byte *pbVar16;
  byte *pbVar17;
  byte *pbVar18;
  byte bVar19;
  undefined2 uVar20;
  undefined4 uVar21;
  undefined4 uVar22;
  int iVar23;
  int iVar24;
  undefined4 uVar25;
  uint uVar26;
  undefined1 auStack_30 [12];
  
  pbVar9 = DAT_080107d0;
  if (*DAT_080107cc == '\x01') {
    if (DAT_080107d0[6] == 0) {
      if ((*DAT_080107d4 != '\0') && (*DAT_080107d8 == '\0')) {
        *DAT_080107d8 = '\x01';
        FUN_0802821c();
        FUN_080282b0();
        uVar21 = FUN_0802e40c();
        FUN_08030e2a(uVar21,0,DAT_080107dc);
        FUN_080313b2(uVar21,0x170,0x1c0);
        uVar22 = FUN_08027ab4();
        FUN_0803157c(uVar21,uVar22,0);
        FUN_08031594(uVar21,0xff,0);
        if (*DAT_080107e0 == '\0') {
          uVar20 = FUN_08027fa4();
          FUN_0802553e(uVar21,9,0,0,DAT_080107f4,uVar20,&DAT_080107e4);
        }
        else if (*DAT_080107e0 == '\x01') {
          uVar20 = FUN_08027fa4();
          FUN_0802553e(uVar21,9,0,0,DAT_08010808,uVar20,s_Firmware_error_080107f8);
        }
      }
      puVar10 = DAT_0801080c;
      iVar23 = FUN_080303b8(*DAT_0801080c);
      if ((iVar23 != 0) && (*DAT_08010814 != 0)) {
        if (pbVar9[7] < 2) {
          pbVar9[7] = pbVar9[7] + 1;
        }
        else if (*DAT_08010810 == '\0') {
          pbVar9[7] = 0;
          FUN_0802e428(*puVar10);
          *puVar10 = 0;
        }
      }
      if ((int)((uint)*DAT_08010818 << 0x18) < 0) {
        if (*DAT_08010810 == '\0') {
          *DAT_0801081c = 0;
        }
        pcVar12 = DAT_08010828;
        iVar23 = DAT_08010824;
        if ((*DAT_08010820 != '\0') &&
           (*DAT_08010820 = '\0', pbVar11 = DAT_0801082c, *pcVar12 == '\0')) {
          if (*DAT_0801082c == 1) {
            iVar24 = FUN_080303b8(*(undefined4 *)(iVar23 + 4));
            if (iVar24 != 0) {
              FUN_0803118e(*(undefined4 *)(iVar23 + 4),1);
            }
            *pbVar11 = 0;
          }
          else if (*DAT_0801082c == 0) {
            iVar24 = FUN_080303b8(*(undefined4 *)(iVar23 + 8));
            if (iVar24 != 0) {
              FUN_0803118e(*(undefined4 *)(iVar23 + 8),1);
            }
            *pbVar11 = 1;
          }
        }
        puVar10 = DAT_08010838;
        pcVar14 = DAT_08010834;
        psVar13 = DAT_08010830;
        sVar5 = *DAT_08010830;
        if (sVar5 != 0) {
          if (*DAT_08010810 == '\0') {
            if (*pcVar12 == '\0') {
              if ((*DAT_08010840 == '\0') && (*DAT_08010834 == '\0')) {
                *DAT_08010834 = '\x01';
                pbVar9[4] = 1;
              }
              *psVar13 = sVar5 + -1;
              if (sVar5 == 1) {
                *pcVar14 = '\0';
                pbVar9[4] = 1;
                iVar24 = FUN_0802b744(*DAT_0801083c);
                if ((iVar24 != 0) && (iVar24 = FUN_080303b8(puVar10[1]), iVar24 != 0)) {
                  uVar21 = FUN_08027fa4();
                  FUN_0803157c(puVar10[1],uVar21,0);
                }
              }
            }
            else {
              *DAT_08010834 = '\0';
              pbVar9[4] = 1;
              *psVar13 = 0;
            }
          }
          else {
            *DAT_08010834 = '\0';
            *psVar13 = 0;
            pbVar9[4] = 1;
            iVar24 = FUN_0802b744(*DAT_0801083c);
            if ((iVar24 != 0) && (iVar24 = FUN_080303b8(puVar10[1]), iVar24 != 0)) {
              uVar21 = FUN_08027fa4();
              FUN_0803157c(puVar10[1],uVar21,0);
            }
          }
        }
        if ((*DAT_08010810 == '\0') && (*DAT_08010844 != *pcVar12)) {
          if (*pcVar12 == '\0') {
            iVar24 = FUN_080303b8(*(undefined4 *)(iVar23 + 0x14));
            if (iVar24 != 0) {
              uVar21 = FUN_0802ebc8(*(undefined4 *)(iVar23 + 0x14),0);
              uVar22 = FUN_08027d0c(0x27,0x27);
              FUN_0803157c(*(undefined4 *)(iVar23 + 0x14),uVar22,0);
              uVar22 = FUN_08027fa4();
              FUN_080316b4(uVar21,uVar22,0);
            }
            iVar24 = FUN_080303b8(*DAT_08010848);
            if (iVar24 != 0) {
              FUN_0802da22(*DAT_08010848,1);
            }
            pbVar9[4] = 1;
          }
          *DAT_08010844 = *pcVar12;
        }
        bVar19 = *DAT_0801082c;
        if (pbVar9[5] != bVar19) {
          pbVar9[4] = 1;
          pbVar9[5] = bVar19;
        }
        if (pbVar9[4] != 0) {
          pbVar9[4] = 0;
          *DAT_0801084c = 0;
          puVar15 = DAT_0801083c;
          FUN_0802b798(*DAT_0801083c);
          if (*DAT_0801082c == 0) {
            if ((*pcVar12 == '\0') && (*pcVar14 != '\0')) {
              FUN_0802b8b2(*puVar15,0);
              FUN_0802b584(*puVar15,*puVar10);
              FUN_0802b584(*puVar15,puVar10[5]);
              FUN_0802b584(*puVar15,puVar10[6]);
              uVar26 = 0;
              do {
                FUN_0802b584(*puVar15,puVar10[uVar26 + 7]);
                uVar26 = uVar26 + 1 & 0xff;
              } while (uVar26 < 0x10);
            }
          }
          else if ((*DAT_0801082c == 1) && (*pcVar14 != '\0')) {
            FUN_0802b8b2(*puVar15,0);
            iVar24 = FUN_080303b8(puVar10[1]);
            if (iVar24 != 0) {
              uVar21 = FUN_08027fa4();
              FUN_0803157c(puVar10[1],uVar21,0);
            }
            FUN_0802b584(*puVar15,*(undefined4 *)(iVar23 + 0xc));
            FUN_0802b584(*puVar15,*(undefined4 *)(iVar23 + 0x10));
            FUN_0802b584(*puVar15,*(undefined4 *)(iVar23 + 0x14));
            FUN_0802b584(*puVar15,*(undefined4 *)(iVar23 + 0x18));
            FUN_0802b584(*puVar15,*(undefined4 *)(iVar23 + 0x1c));
          }
        }
        if (*DAT_08010850 != *DAT_08010854) {
          *DAT_08010850 = *DAT_08010854;
        }
        if (*DAT_08010858 != *DAT_0801085c) {
          *DAT_08010858 = *DAT_0801085c;
        }
        if (*DAT_08010860 != *DAT_08010864) {
          *DAT_08010860 = *DAT_08010864;
        }
        if (*DAT_08010868 != *DAT_0801086c) {
          *DAT_08010868 = *DAT_0801086c;
        }
        if (*DAT_08010870 != *DAT_08010874) {
          *DAT_08010870 = *DAT_08010874;
        }
        if (*DAT_08010878 != *DAT_0801087c) {
          *DAT_08010878 = *DAT_0801087c;
        }
        if (*DAT_08010880 != *DAT_08010884) {
          *DAT_08010880 = *DAT_08010884;
        }
        FUN_0803840c();
        pbVar11 = DAT_08010818;
        bVar19 = *pbVar9;
        bVar1 = *DAT_08010818;
        if (bVar19 != bVar1) {
          *pbVar9 = bVar1;
        }
        pbVar16 = DAT_08010888;
        bVar2 = pbVar9[1];
        bVar6 = *DAT_08010888;
        if (bVar2 != bVar6) {
          pbVar9[1] = bVar6;
        }
        pbVar17 = DAT_0801088c;
        bVar3 = pbVar9[2];
        bVar7 = *DAT_0801088c;
        if (bVar3 != bVar7) {
          pbVar9[2] = bVar7;
        }
        pbVar18 = DAT_08010890;
        bVar4 = pbVar9[3];
        bVar8 = *DAT_08010890;
        if (bVar4 != bVar8) {
          pbVar9[3] = bVar8;
        }
        if (bVar4 != bVar8 || (bVar3 != bVar7 || (bVar2 != bVar6 || bVar19 != bVar1))) {
          FUN_0800ff7c(bVar6,auStack_30);
          iVar23 = FUN_080303b8(puVar10[4]);
          if (iVar23 != 0) {
            FUN_0802d33c(puVar10[4],auStack_30);
          }
          iVar23 = FUN_080303b8(puVar10[3]);
          if (iVar23 != 0) {
            if (*DAT_08010abc == '\0') {
              FUN_0802e2d2(puVar10[3],1);
            }
            else if (*DAT_08010abc == '\x01') {
              FUN_0802da22(puVar10[3],1);
            }
          }
          iVar23 = FUN_080303b8(puVar10[4]);
          if ((iVar23 != 0) && (iVar23 = FUN_080303b8(puVar10[3]), iVar23 != 0)) {
            FUN_0802db62(puVar10[3],puVar10[4],0x11,0xffffffff,10);
          }
          bVar19 = FUN_0801007c(*pbVar16,*pbVar18,*pbVar17);
          *pbVar17 = bVar19;
          iVar23 = FUN_080303b8(puVar10[5]);
          if (iVar23 != 0) {
            uVar21 = FUN_0800adf6(*pbVar17);
            FUN_08031fba(puVar10[5],uVar21,1);
          }
          iVar23 = FUN_080303b8(puVar10[6]);
          if (iVar23 != 0) {
            uVar21 = FUN_0800adc4(*pbVar18);
            FUN_08031fba(puVar10[6],uVar21,1);
          }
          iVar23 = (uint)*DAT_08010ac0 - (uint)*pbVar16;
          uVar26 = ((short)iVar23 * 0x13c) / (int)(*DAT_08010ac0 - 0x14) & 0xffff;
          FUN_0802782e(puVar10[2],iVar23 / 10 & 0xff,0);
          iVar23 = FUN_080303b8(puVar10[1]);
          if (iVar23 != 0) {
            if (uVar26 < 2) {
              uVar26 = 1;
            }
            FUN_080316fe(puVar10[1],uVar26);
          }
          uVar26 = 0;
          do {
            iVar23 = FUN_080303b8(puVar10[uVar26 + 7]);
            if (iVar23 != 0) {
              uVar21 = FUN_0802ebc8(puVar10[uVar26 + 7],1);
              uVar22 = FUN_0802ebc8(puVar10[uVar26 + 7],0);
              iVar23 = FUN_080303b8(uVar21);
              if ((iVar23 != 0) && (iVar23 = FUN_080303b8(uVar22), iVar23 != 0)) {
                if ((1 << uVar26 & (uint)*pbVar11) == 0) {
                  uVar25 = FUN_08027ab4();
                  FUN_0803157c(uVar21,uVar25,0);
                  uVar21 = FUN_08027d0c(0x37,0x37);
                  FUN_080316b4(uVar22,uVar21,0);
                }
                else {
                  if (*DAT_08010ac4 == '\0') {
                    uVar25 = FUN_08027fa4();
                    FUN_0803157c(uVar21,uVar25,0);
                  }
                  else {
                    switch(uVar26) {
                    case 0:
                      uVar25 = FUN_08027d0c(0xff,0);
                      FUN_0803157c(uVar21,uVar25,0);
                      break;
                    case 1:
                      uVar25 = FUN_08027d0c(0,0xff);
                      FUN_0803157c(uVar21,uVar25,0);
                      break;
                    case 2:
                      uVar25 = FUN_08027d0c(0,0,0xff);
                      FUN_0803157c(uVar21,uVar25,0);
                      break;
                    case 3:
                      uVar25 = FUN_08027d0c(0,0xff);
                      FUN_0803157c(uVar21,uVar25,0);
                      break;
                    case 4:
                      uVar25 = FUN_08027d0c(0xff,0);
                      FUN_0803157c(uVar21,uVar25,0);
                    }
                  }
                  uVar21 = FUN_08027fa4();
                  FUN_080316b4(uVar22,uVar21,0);
                }
              }
            }
            uVar26 = uVar26 + 1 & 0xff;
          } while (uVar26 < 0x10);
        }
      }
      else {
        FUN_080345b4(300);
      }
    }
    else {
      DAT_080107d0[6] = DAT_080107d0[6] - 1;
    }
  }
  return;
}

