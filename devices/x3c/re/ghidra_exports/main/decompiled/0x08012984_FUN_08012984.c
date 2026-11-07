// 0x08012984 FUN_08012984 size=2174

void FUN_08012984(void)

{
  byte bVar1;
  bool bVar2;
  char *pcVar3;
  char *pcVar4;
  byte *pbVar5;
  byte *pbVar6;
  uint *puVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined1 *puVar10;
  char *pcVar11;
  undefined1 uVar12;
  int iVar13;
  uint uVar14;
  uint uVar15;
  undefined4 unaff_r4;
  bool bVar16;
  undefined4 uVar17;
  
  uVar17 = DAT_080129b4;
  iVar13 = FUN_08038216(DAT_080129b4,1);
  if (iVar13 == 0) {
    return;
  }
  FUN_080381da(uVar17,1);
  uVar12 = FUN_080381d0(uVar17);
  iVar13 = DAT_080129b8;
  *(undefined1 *)(DAT_080129b8 + 0x11) = uVar12;
  bVar1 = *(byte *)(iVar13 + 0x11);
  bVar2 = false;
  uVar17 = 0;
  *DAT_08013168 = 0;
  pbVar5 = DAT_08013170;
  pcVar3 = DAT_0801316c;
  uVar15 = (uint)(byte)DAT_0801316c[2];
  if (uVar15 == 0) {
    *DAT_08013170 = bVar1;
    pcVar3[2] = '\x01';
    if (pcVar3[4] != '\0') {
      pcVar3[5] = bVar1;
      pcVar3[2] = '\0';
    }
    puVar9 = DAT_08013180;
    puVar8 = DAT_0801317c;
    puVar7 = DAT_08013178;
    uVar15 = (uint)*DAT_08013174;
    if (bVar1 == 0xb7) {
      pcVar3[3] = '\x03';
      pcVar3[0x15] = -0x48;
      *puVar7 = uVar15;
      *puVar9 = *puVar8;
      return;
    }
    if (bVar1 < 0xb8) {
      if (bVar1 == 0xb3) {
        pcVar3[3] = '\x03';
        pcVar3[0x15] = -0x4c;
        *puVar7 = uVar15;
        *puVar9 = *puVar8;
        return;
      }
      if (bVar1 < 0xb4) {
        if (bVar1 == 0xa0) {
          pcVar3[2] = '\0';
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          return;
        }
        if (bVar1 == 0xb0) {
          pcVar3[3] = '\x1a';
          pcVar3[0x15] = -0x4f;
          return;
        }
        if (bVar1 == 0xb1) {
          pcVar3[3] = '\x03';
          pcVar3[0x15] = -0x4e;
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          return;
        }
        if (bVar1 == 0xb2) {
          pcVar3[3] = '\x04';
          pcVar3[0x15] = -0x4d;
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          return;
        }
      }
      else {
        if (bVar1 == 0xb4) {
          pcVar3[3] = '\x03';
          pcVar3[0x15] = -0x4b;
          *puVar9 = *puVar8;
          return;
        }
        if (bVar1 == 0xb5) {
          pcVar3[3] = '\x03';
          pcVar3[0x15] = -0x4a;
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          return;
        }
        if (bVar1 == 0xb6) {
          pcVar3[3] = '\t';
          pcVar3[0x15] = -0x49;
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          return;
        }
      }
    }
    else {
      if (bVar1 == 199) {
        pcVar3[3] = '\x03';
        pcVar3[0x15] = -0x38;
        *puVar7 = uVar15;
        *puVar9 = *puVar8;
        return;
      }
      if (bVar1 < 200) {
        if (bVar1 == 0xb8) {
          pcVar3[3] = '\x03';
          pcVar3[0x15] = -0x47;
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          return;
        }
        if (bVar1 == 0xb9) {
          pcVar3[3] = '\x03';
          pcVar3[0x15] = -0x46;
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          return;
        }
        if (bVar1 == 0xc2) {
          pcVar3[3] = '\x03';
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          pcVar3[0x15] = -0x3d;
          return;
        }
        if (bVar1 == 0xc4) {
          pcVar3[3] = '\x03';
          pcVar3[0x15] = -0x3b;
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          return;
        }
      }
      else {
        if (bVar1 == 0xc9) {
          pcVar3[3] = '\x03';
          pcVar3[0x15] = -0x36;
          *puVar7 = uVar15;
          *puVar9 = *puVar8;
          return;
        }
        if (bVar1 == 0xcf) {
          *DAT_08013190 = *DAT_08013190 ^ 0x1000;
          FUN_080129bc();
          pcVar3[2] = '\0';
          *DAT_08013194 = 1;
        }
        else if (bVar1 == 0xd0) {
          FUN_08012ad8();
          pcVar3[2] = '\0';
          pcVar3[0xb] = '\0';
          *DAT_08013184 = 0;
          *DAT_08013188 = 0;
          *DAT_0801318c = 0x14;
          return;
        }
      }
    }
    pcVar3[2] = '\0';
    return;
  }
  uVar14 = uVar15 + 1 & 0xff;
  DAT_0801316c[2] = (char)(uVar15 + 1);
  pbVar5[uVar15] = bVar1;
  if (uVar14 != (byte)pcVar3[3]) {
    return;
  }
  for (uVar15 = 1; (int)uVar15 < (int)(uVar14 - 1); uVar15 = uVar15 + 1 & 0xff) {
    pcVar3[0x15] = pbVar5[uVar15] + pcVar3[0x15];
  }
  bVar16 = pbVar5[(byte)pcVar3[3] - 1] == pcVar3[0x15];
LAB_08012fac:
  if (bVar16) {
    if ((*pcVar3 != '\0') || (*pbVar5 != 0xc4)) {
      FUN_080381d6(DAT_08013198,*pbVar5);
    }
    pcVar11 = DAT_080131a8;
    pbVar6 = DAT_080131a4;
    iVar13 = DAT_080131a0;
    puVar10 = DAT_0801319c;
    pcVar4 = DAT_0801316c;
    switch(*pbVar5) {
    case 0xb0:
      goto switchD_08012fe2_caseD_b0;
    case 0xb1:
      pcVar3[8] = '\x01';
      pcVar11 = DAT_080131b4;
      if ((*DAT_080131b4 == '\x02') && ((int)(pbVar5[1] - 10) < 3)) {
        pcVar4[pbVar5[1] + 0xe] = '\0';
      }
      if (-1 < (int)((uint)*pbVar6 << 0x18)) {
        uVar15 = (uint)pbVar5[1];
        if (uVar15 == 0x50) {
          uVar15 = 0;
          do {
            if ((*(byte *)(iVar13 + uVar15 * 6 + 3) >> 6 == 3) &&
               (*(char *)(iVar13 + uVar15 * 6) == '\x01')) {
              bVar2 = true;
              FUN_0801184c(uVar15 + 10 & 0xff,0xb1,0);
              FUN_0801c118(200);
            }
            uVar15 = uVar15 + 1 & 0xff;
          } while (uVar15 < 5);
        }
        else if ((*(char *)(iVar13 + (uVar15 - 10) * 6) == '\x01') &&
                (*(byte *)(iVar13 + (uVar15 - 10) * 6 + 3) >> 6 == 3)) {
          FUN_0801184c(uVar15,0xb1,0,puVar10,uVar17,unaff_r4);
          FUN_0801c118(200);
          bVar2 = true;
        }
        if (bVar2) {
          FUN_0801184c(pbVar5[1],0xc0,*pcVar11);
          FUN_0801c118(200);
          if (*pcVar11 == '\x01') {
            uVar17 = FUN_0800c0cc();
          }
          pcVar4 = DAT_080131b8;
          bVar1 = pbVar5[1];
          if (bVar1 == 0x50) {
            uVar15 = 0;
            do {
              if ((*(byte *)(iVar13 + uVar15 * 6 + 3) >> 6 == 3) &&
                 (*(char *)(iVar13 + uVar15 * 6) == '\x01')) {
                if ((*pcVar11 == '\x01') &&
                   (((int)((uint)pbVar5[1] << 0x18) < 0 && (*pcVar4 == '\x01')))) {
                  FUN_0801184c(uVar15 + 10 & 0xff,0xc1,uVar17);
                  FUN_0801c118(200);
                }
                FUN_0801184c(uVar15 + 10 & 0xff,0xb4,3);
                FUN_0801c118(200);
              }
              uVar15 = uVar15 + 1 & 0xff;
            } while (uVar15 < 5);
          }
          else {
            if (((*pcVar11 == '\x01') && ((int)((uint)bVar1 << 0x18) < 0)) &&
               (*DAT_080131b8 == '\x01')) {
              FUN_0801184c(bVar1 & 0x7f,0xc1,uVar17);
              FUN_0801c118(200);
            }
            FUN_0801184c(pbVar5[1],0xb4,3);
          }
          pcVar3[10] = pbVar5[1];
          break;
        }
      }
      goto switchD_08012fe2_caseD_ba;
    case 0xb2:
      if ((*DAT_080135fc == '\x02') && ((int)(pbVar5[1] - 10) < 3)) {
        DAT_0801316c[pbVar5[1] + 0xe] = '\0';
      }
      FUN_0801184c(0x50,0xb3,0,puVar10,uVar17,unaff_r4);
      FUN_0801c118(200);
      pcVar3[8] = '\x01';
      pcVar3[0x10] = '\x01';
      uVar15 = (uint)pbVar5[1];
      if (uVar15 == 0x50) {
        if (*DAT_080135fc == '\x03') {
          uVar15 = 0;
          do {
            if ((*(byte *)(iVar13 + uVar15 * 6 + 3) >> 6 == 3) &&
               (*(char *)(iVar13 + uVar15 * 6) == '\x01')) {
              FUN_0801184c(uVar15 + 10 & 0xff,0xb1,0);
              bVar2 = true;
              FUN_0801c118(0x96);
            }
            uVar15 = uVar15 + 1 & 0xff;
          } while (uVar15 < 3);
        }
        else if (*DAT_080135fc == '\x02') {
          uVar15 = 0;
          do {
            if (*(byte *)(iVar13 + uVar15 * 6 + 3) >> 6 == 3) {
              if (*(char *)(iVar13 + uVar15 * 6) == '\x01') {
                FUN_0801184c(uVar15 + 10 & 0xff,0xb1,0);
                bVar2 = true;
              }
              else {
                FUN_0801184c(uVar15 + 10 & 0xff,0xb1,1);
              }
              FUN_0801c118(200);
            }
            uVar15 = uVar15 + 1 & 0xff;
          } while (uVar15 < 3);
        }
        else {
          uVar15 = 0;
          do {
            if ((*(byte *)(iVar13 + uVar15 * 6 + 3) >> 6 == 3) &&
               (*(char *)(iVar13 + uVar15 * 6) == '\x01')) {
              FUN_0801184c(uVar15 + 10 & 0xff,0xb1,0);
              bVar2 = true;
              FUN_0801c118(200);
            }
            uVar15 = uVar15 + 1 & 0xff;
          } while (uVar15 < 5);
        }
      }
      else if ((*(char *)(iVar13 + (uVar15 - 10) * 6) == '\x01') &&
              (*(byte *)(iVar13 + (uVar15 - 10) * 6 + 3) >> 6 == 3)) {
        FUN_0801184c(uVar15,0xb1,0);
        bVar2 = true;
        FUN_0801c118(200);
      }
      if (bVar2) {
        FUN_0801184c(pbVar5[1],0xc0,*DAT_080135fc);
        FUN_0801c118(200);
        FUN_0801184c(pbVar5[1],0xb9,pbVar5[2]);
        FUN_0801c118(200);
        FUN_0801c118(200);
        if (pbVar5[1] == 0x50) {
          FUN_0801184c(0x50,0xb4,3);
          FUN_0801c118(200);
        }
        else {
          FUN_0801184c(pbVar5[1],0xb4,3);
        }
        pcVar3[10] = pbVar5[1];
      }
      break;
    case 0xb3:
      FUN_0801184c(0x50,0xb4,8,DAT_0801319c,uVar17,unaff_r4);
      FUN_0801c118(200);
      FUN_0801184c(0x50,0xb7,*DAT_08013600);
      puVar10 = DAT_08013604;
      *DAT_08013604 = 0;
      puVar10[1] = 0;
      puVar10[2] = 0;
      if ((*DAT_080135fc == '\x02') && (*DAT_08013608 == '\x01')) {
        FUN_0800bddc();
      }
      break;
    case 0xb4:
      FUN_080118b0(0x1a);
      FUN_0801c118(200);
      break;
    case 0xb5:
      pcVar3[8] = '\x02';
      if (-1 < (int)((uint)*pbVar6 << 0x18)) {
        uVar15 = (uint)pbVar5[1];
        if (uVar15 == 0x50) {
          uVar15 = 0;
          do {
            if ((*(byte *)(iVar13 + uVar15 * 6 + 3) >> 6 == 3) &&
               (*(char *)(iVar13 + uVar15 * 6) == '\x01')) {
              FUN_0801184c(uVar15 + 10 & 0xff,0xb1,0);
              bVar2 = true;
              FUN_0801c118(200);
            }
            uVar15 = uVar15 + 1 & 0xff;
          } while (uVar15 < 5);
        }
        else if ((*(char *)(iVar13 + (uVar15 - 10) * 6) == '\x01') &&
                (*(byte *)(iVar13 + (uVar15 - 10) * 6 + 3) >> 6 == 3)) {
          FUN_0801184c(uVar15,0xb1,0,puVar10,uVar17,unaff_r4);
          bVar2 = true;
          FUN_0801c118(200);
        }
        FUN_0801c118(200);
        if (bVar2) {
          FUN_0801184c(pbVar5[1],0xc0,*DAT_080135fc);
          FUN_0801c118(200);
          FUN_0801184c(pbVar5[1],0xb4,5);
          bVar1 = pbVar5[1];
          pcVar3[10] = bVar1;
          if ((int)(bVar1 - 10) < 3) {
            pcVar4[bVar1 + 0xe] = '$';
          }
        }
      }
      break;
    case 0xb6:
      pcVar3[0x10] = '\x01';
      if ((int)((uint)*pbVar6 << 0x18) < 0) {
        *pcVar11 = '\x01';
      }
      pcVar3[8] = '\0';
      FUN_0801184c(0x50,0xc0,*DAT_080135fc,puVar10,uVar17,unaff_r4);
      FUN_0801c118(200);
      if ((*DAT_080135fc == '\x01') || (*DAT_080135fc == '\x02')) {
        FUN_0801184c(0x50,0xb6,*DAT_0801360c);
        FUN_0801c118(200);
      }
      if (pbVar5[1] == 1) {
        FUN_0801184c(0x50,0xb3,1);
      }
      else {
        FUN_0801184c(0x50,0xb3,0);
      }
      pcVar3[0xf] = pbVar5[1];
      FUN_0801c118(200);
      if ((*DAT_080135fc != '\x02') && (*DAT_08013608 == '\x01')) {
        FUN_0800bddc();
      }
      uVar15 = 0;
      do {
        if ((*(char *)(DAT_08013610 + uVar15 * 6) == '\x01') &&
           (*(byte *)(DAT_08013610 + uVar15 * 6 + 3) >> 6 == 3)) {
          FUN_0801184c(uVar15 + 10 & 0xff,0xb9,pbVar5[uVar15 + 3]);
          FUN_0801c118(200);
          *(ushort *)(DAT_08013604 + uVar15 * 2 + 4) = (ushort)pbVar5[uVar15 + 3];
        }
        uVar15 = uVar15 + 1 & 0xff;
      } while (uVar15 < 5);
      if ((int)((uint)*pbVar6 << 0x18) < 0) {
        uVar15 = 0;
        do {
          if ((1 << uVar15 & (uint)*pbVar6) != 0) {
            FUN_0801184c(uVar15 + 10 & 0xff,0xb4,2);
            FUN_0801c118(200);
          }
          uVar15 = uVar15 + 1 & 0xff;
        } while (uVar15 < 5);
      }
      else {
        FUN_0801184c(0x50,0xb4,2);
        FUN_0801c118(200);
      }
      if ((pbVar5[2] == 1) && (pbVar5[1] == 1)) {
        *DAT_08013614 = 0x28;
        FUN_080170d8();
      }
      break;
    case 0xb7:
      pcVar3[0x10] = '\0';
      pcVar3[8] = '\0';
      pcVar3[10] = '\0';
      if (*pcVar11 != '\0') {
        *pcVar11 = '\0';
        FUN_080118b0(0x21);
        FUN_0801c118(100);
        FUN_080118b0(0x21);
      }
      break;
    case 0xb8:
      FUN_0801184c(0x50,0xb4,8,DAT_0801319c,uVar17,unaff_r4);
      if (*DAT_08013670 != '\0') {
        FUN_0801184c(0x50,0xc0,*DAT_08013674);
        FUN_0801c118(200);
        FUN_0800bddc();
      }
      break;
    case 0xb9:
      FUN_080118b0(0x29);
      break;
    default:
      goto switchD_08012fe2_caseD_ba;
    case 0xc2:
      *DAT_08013678 = pbVar5[1];
      *puVar10 = 0;
      break;
    case 0xc4:
      *DAT_0801367c = 1;
      break;
    case 199:
      *DAT_08013684 = 1;
      break;
    case 0xc9:
      *DAT_0801319c = 0;
      *DAT_08013680 = pbVar5[1];
    }
    goto LAB_0801366a;
  }
  goto switchD_08012fe2_caseD_ba;
switchD_08012fe2_caseD_b0:
  pcVar3[9] = '\x01';
  pbVar6 = DAT_08013170;
  uVar15 = 0;
  do {
    pbVar6[uVar15 + 0x33] = pbVar5[uVar15 + 1];
    uVar15 = uVar15 + 1 & 0xff;
  } while (uVar15 < 0x18);
  iVar13 = FUN_0800c1e8();
  bVar16 = false;
  if (iVar13 == 1) {
    *DAT_080131ac = *DAT_080131ac + 2000;
    *DAT_080131b0 = *DAT_080131b0 + 5;
switchD_08012fe2_caseD_ba:
LAB_0801366a:
    pcVar3[2] = '\0';
    return;
  }
  goto LAB_08012fac;
}

