// 0x0800c1e8 FUN_0800c1e8 size=1344

undefined8 FUN_0800c1e8(undefined4 param_1,uint param_2)

{
  byte bVar1;
  char cVar2;
  byte bVar3;
  bool bVar4;
  int iVar5;
  byte *pbVar6;
  byte *pbVar7;
  byte *pbVar8;
  ushort *puVar9;
  int iVar10;
  undefined4 uVar11;
  byte bVar12;
  byte bVar13;
  int iVar14;
  undefined1 uVar15;
  uint uVar16;
  byte bVar17;
  byte bVar18;
  uint uVar19;
  byte bVar20;
  uint uVar21;
  byte bVar22;
  bool bVar23;
  uint local_30;
  
  iVar5 = DAT_0800c5e4;
  bVar4 = false;
  cVar2 = *(char *)(DAT_0800c5e4 + 0x11);
  if ((cVar2 != '\0') && (*(char *)(DAT_0800c5e4 + 0x11) = cVar2 + -1, cVar2 == '\x01')) {
    *(undefined1 *)(iVar5 + 8) = 0;
  }
  pbVar6 = DAT_0800c5e8;
  bVar1 = DAT_0800c5e8[8];
  local_30 = param_2;
  if (*(char *)(iVar5 + 9) != '\0') {
    *(undefined1 *)(iVar5 + 9) = 0;
    bVar20 = *pbVar6;
    *(byte *)(iVar5 + 1) = bVar20;
    bVar17 = pbVar6[1];
    uVar16 = bVar17 & 0xf;
    uVar15 = (undefined1)uVar16;
    if (2 < uVar16) {
      uVar15 = 1;
    }
    *DAT_0800c5ec = uVar15;
    *DAT_0800c5f0 = pbVar6[2];
    *DAT_0800c5f4 = pbVar6[3];
    *DAT_0800c5f8 = pbVar6[5];
    bVar12 = pbVar6[6];
    *(byte *)(iVar5 + 0xd) = bVar12 >> 4;
    bVar12 = bVar12 & 0xf;
    if (10 < bVar12) {
      bVar12 = 1;
    }
    *DAT_0800c5fc = bVar12;
    if ((*(char *)(iVar5 + 0x12) != '\0') &&
       (*(undefined1 *)(iVar5 + 0x12) = 0, bVar12 != *DAT_0800c600)) {
      *DAT_0800c604 = 1;
    }
    bVar3 = *DAT_0800c608;
    bVar18 = pbVar6[7];
    if (1 < bVar18) {
      bVar18 = 0;
    }
    *DAT_0800c608 = bVar18;
    if (bVar3 != bVar18) {
      *(undefined1 *)(iVar5 + 0x14) = 10;
    }
    if (bVar18 == 0) {
      *DAT_0800c60c = 0;
      *(undefined1 *)(iVar5 + 0xc) = 0;
    }
    if ((*DAT_0800c610 != '\0') && (uVar16 == 4)) {
      *DAT_0800c614 = 0;
    }
    bVar3 = DAT_0800c5e8[1] >> 4;
    if (bVar12 == 7) {
      bVar22 = bVar3;
      if (3 < bVar17 >> 4) {
        bVar22 = 0;
      }
      *DAT_0800c618 = bVar22;
    }
    if (bVar18 == 0) {
      uVar11 = 0;
      goto LAB_0800c2e0;
    }
    if (bVar12 == 1) {
      *(byte *)(iVar5 + 7) = pbVar6[0x17];
    }
    pbVar8 = DAT_0800c618;
    pbVar7 = DAT_0800c5e8;
    bVar18 = *DAT_0800c61c;
    uVar19 = (uint)bVar18;
    if (bVar12 == 2) {
      if ((-1 < (int)(uVar19 << 0x18)) && ((bVar17 & 0x3f) >> 4 != (uint)DAT_0800c5e8[-0x16])) {
        bVar4 = true;
      }
      if ((-1 < (int)(uVar19 << 0x18)) && (bVar17 >> 4 < 4)) {
        if ((int)((uint)bVar3 << 0x1e) < 0) {
          if (*DAT_0800c618 != 2) {
            *(byte *)(iVar5 + 0xe) = *DAT_0800c618;
            *pbVar8 = 2;
          }
        }
        else if (*DAT_0800c618 == 2) {
          *DAT_0800c618 = *(byte *)(iVar5 + 0xe);
        }
        else {
          *DAT_0800c618 = bVar3;
        }
      }
    }
    if (*(char *)(iVar5 + 0xb) != '\0') {
      *(undefined1 *)(iVar5 + 9) = 0;
      uVar11 = 0;
      goto LAB_0800c2e0;
    }
    if (1 < bVar20) {
      bVar20 = 0;
    }
    *DAT_0800c620 = bVar20;
    bVar20 = pbVar6[4];
    if ((int)((uint)bVar20 << 0x18) < 0) {
      *DAT_0800c624 = 1;
    }
    else {
      *DAT_0800c624 = 0;
    }
    uVar21 = bVar20 & 0x7f;
    if (uVar21 - 1 < 9) {
      *DAT_0800c62c = *(undefined1 *)(DAT_0800c628 + uVar21);
    }
    pbVar8 = DAT_0800c61c;
    bVar23 = *DAT_0800c630 == '\0';
    do {
      do {
        if (!bVar23) goto LAB_0800c762;
      } while ((bVar12 != 1) && (bVar23 = bVar12 == 5, !bVar23));
      bVar12 = *(byte *)(iVar5 + 6);
      bVar23 = bVar12 == 0;
    } while (!bVar23);
    if ((uVar16 != pbVar7[-0x15]) && ((uVar16 == 2 || (pbVar7[-0x15] == 2)))) {
      bVar4 = true;
    }
    bVar20 = pbVar6[9];
    if ((uint)bVar20 != (uint)pbVar7[-0xe]) {
      bVar4 = true;
    }
    bVar12 = pbVar6[10];
    if (bVar12 != pbVar7[-0xd]) {
      bVar4 = true;
    }
    bVar22 = pbVar6[0xb];
    if (bVar22 != pbVar7[-0xc]) {
      bVar4 = true;
    }
    bVar13 = pbVar6[0xc];
    local_30 = (uint)bVar13;
    if (local_30 != pbVar7[-0xb]) {
      bVar4 = true;
    }
    uVar21 = 0;
    do {
      if (DAT_0800c5e8[uVar21 * 2 + 0xd] != DAT_0800c5e8[uVar21 * 2 + -10]) {
        bVar4 = true;
      }
      if ((DAT_0800c5e8[uVar21 * 2 + 0xd] != 6) &&
         (DAT_0800c5e8[uVar21 * 2 + 0xe] != DAT_0800c5e8[uVar21 * 2 + -9])) {
        bVar4 = true;
      }
      uVar21 = uVar21 + 1 & 0xff;
    } while (uVar21 < 5);
    if ((-1 < (int)(uVar19 << 0x18)) && (bVar17 >> 4 < 4)) {
      if ((int)((uint)bVar3 << 0x1e) < 0) {
        *DAT_0800c618 = 2;
      }
      else {
        *DAT_0800c618 = bVar3;
      }
      if (bVar17 >> 4 == 3) {
        *(undefined1 *)(iVar5 + 0xe) = 1;
      }
      else {
        *(undefined1 *)(iVar5 + 0xe) = 0;
      }
    }
    pbVar6 = DAT_0800c5e8;
    if (uVar16 == 2) {
      *pbVar8 = bVar18 | 0x80;
    }
    else {
      *pbVar8 = bVar18 & 0x7f;
    }
    uVar16 = 0;
    do {
      bVar17 = (byte)(1 << uVar16);
      if (((uint)bVar20 & 1 << uVar16) == 0) {
        *pbVar8 = *pbVar8 & ~bVar17;
      }
      else {
        *pbVar8 = bVar17 | *pbVar8;
      }
      uVar16 = uVar16 + 1 & 0xff;
    } while (uVar16 < 5);
    if (bVar12 < 0x5b) {
      if ((bVar12 <= *DAT_0800c638) && (0x13 < bVar12)) {
        *DAT_0800c634 = bVar12;
      }
    }
    else if (bVar1 == 0xff) {
      *DAT_0800c634 = 0xff;
      *pbVar8 = 0;
    }
    else {
      *DAT_0800c634 = 0;
    }
    *DAT_0800c63c = bVar22;
    if (0xc6 < local_30 - 1) {
      bVar13 = *DAT_0800c640;
    }
    *DAT_0800c640 = bVar13;
    iVar10 = DAT_0800c648;
    puVar9 = DAT_0800c644;
    uVar16 = 0;
    do {
      if (bVar4) {
        bVar17 = pbVar6[uVar16 * 2 + 0xd];
        if (bVar17 == 0) {
          if ((uVar16 < 3) || ((bVar1 != 3 && (*(char *)(iVar5 + 0xc) != '\x01')))) {
            *(undefined1 *)(iVar10 + uVar16 * 6) = 1;
            bVar17 = pbVar6[uVar16 * 2 + 0xe];
            if (bVar17 == 0x12) {
              *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0;
            }
            else if (bVar17 < 0x13) {
              switch(bVar17) {
              case 0:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x1e;
                break;
              case 2:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x1b;
                break;
              case 4:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x17;
                break;
              case 6:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x14;
                break;
              case 8:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x11;
                break;
              case 10:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0xd;
                break;
              case 0xc:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 10;
                break;
              case 0xe:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 7;
                break;
              case 0x10:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 3;
              }
            }
            else {
              switch(bVar17) {
              case 0xee:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x3c;
                break;
              case 0xf0:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x39;
                break;
              case 0xf2:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x35;
                break;
              case 0xf4:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x32;
                break;
              case 0xf6:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x2f;
                break;
              case 0xf8:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x2b;
                break;
              case 0xfa:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x28;
                break;
              case 0xfc:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x25;
                break;
              case 0xfe:
                *(undefined1 *)(iVar10 + uVar16 * 6 + 2) = 0x21;
              }
            }
          }
          else {
            *(undefined1 *)(iVar10 + uVar16 * 6) = 0;
          }
          *puVar9 = (ushort)(1 << uVar16) | *puVar9;
          iVar14 = iVar10 + uVar16 * 6;
          *(byte *)(iVar14 + 3) = *(byte *)(iVar14 + 3) | 0xc0;
        }
        else if (bVar17 == 1) {
          *(undefined1 *)(iVar10 + uVar16 * 6) = 0;
          if (((uVar16 < 3) || (bVar1 != 3)) || (*(char *)(iVar5 + 0xc) != '\0')) {
            bVar17 = pbVar6[uVar16 * 2 + 0xe];
            if (0x50 < bVar17) {
              bVar17 = *(byte *)(iVar10 + uVar16 * 6 + 1);
            }
            *(byte *)(iVar10 + uVar16 * 6 + 1) = bVar17;
          }
          *puVar9 = (ushort)(1 << uVar16) | *puVar9;
          iVar14 = iVar10 + uVar16 * 6;
          *(byte *)(iVar14 + 3) = *(byte *)(iVar14 + 3) | 0xc0;
        }
        else if (bVar17 == 6) {
          iVar14 = iVar10 + uVar16 * 6;
          bVar17 = *(byte *)(iVar14 + 3);
          if ((int)((uint)bVar17 << 0x18) < 0) {
            *(byte *)(iVar14 + 3) = bVar17 & 0xbf;
          }
        }
      }
      uVar16 = uVar16 + 1 & 0xff;
    } while (uVar16 < 5);
  }
LAB_0800c762:
  if (*DAT_0800c7b4 == '\0') {
    *(undefined1 *)(iVar5 + 0xc) = 0;
    *(undefined1 *)(iVar5 + 0x13) = 0;
  }
  else {
    if (bVar1 == 3) {
      *(undefined1 *)(iVar5 + 0xc) = 1;
    }
    else if (bVar1 == 5) {
      *(undefined1 *)(iVar5 + 0xc) = 0;
    }
    if (*DAT_0800c7b8 < 5) {
      *DAT_0800c7b8 = 5;
    }
  }
  if ((*DAT_0800c7bc == '\x03') || (*DAT_0800c7bc == '\x02')) {
    *(undefined1 *)(iVar5 + 0xc) = 1;
  }
  if (bVar4) {
    uVar11 = 1;
  }
  else {
    uVar11 = 0;
  }
LAB_0800c2e0:
  return CONCAT44(local_30,uVar11);
}

