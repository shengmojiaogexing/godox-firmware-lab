// 0x0800e31c FUN_0800e31c size=4058

void FUN_0800e31c(void)

{
  short sVar1;
  char cVar2;
  byte bVar3;
  bool bVar4;
  bool bVar5;
  char *pcVar6;
  char *pcVar7;
  byte *pbVar8;
  byte *pbVar9;
  undefined2 *puVar10;
  short *psVar11;
  char cVar12;
  undefined2 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  int iVar16;
  uint uVar17;
  int iVar18;
  int iVar19;
  uint uVar20;
  uint uVar21;
  char cVar22;
  uint uVar23;
  undefined4 *puVar24;
  byte bVar25;
  undefined4 *puVar26;
  undefined4 in_stack_ffffffac;
  undefined2 uVar27;
  undefined1 auStack_3c [12];
  int local_30;
  int local_2c;
  int local_28;
  
  pcVar6 = DAT_0800e73c;
  uVar27 = (undefined2)((uint)in_stack_ffffffac >> 0x10);
  bVar25 = 0;
  bVar4 = false;
  bVar5 = false;
  local_30 = 0;
  cVar22 = '\0';
  uVar21 = 0;
  uVar23 = 0;
  if ((*DAT_0800e730 == '\0') && (*DAT_0800e734 != 0)) {
    if ((*DAT_0800e738 != '\0') && (DAT_0800e73c[0x12] == '\0')) {
      DAT_0800e73c[0x12] = '\x01';
      FUN_0802821c();
      FUN_080282b0();
      uVar14 = FUN_0802e40c();
      FUN_08030e2a(uVar14,0,DAT_0800e740);
      FUN_080313b2(uVar14,0x170,0x1c0);
      uVar15 = FUN_08027a8c();
      FUN_0803157c(uVar14,uVar15,0);
      FUN_08031594(uVar14,0xff,0);
      if (*DAT_0800e744 == '\0') {
        uVar13 = FUN_08027f7c();
        FUN_0802553e(uVar14,9,0,0,DAT_0800e758,CONCAT22(uVar27,uVar13),&DAT_0800e748);
      }
      else if (*DAT_0800e744 == '\x01') {
        uVar13 = FUN_08027f7c();
        FUN_0802553e(uVar14,9,0,0,DAT_0800e76c,CONCAT22(uVar27,uVar13),s_Firmware_error_0800e75c);
      }
    }
    if (pcVar6[0xc] == '\0') {
      iVar16 = FUN_080303b8(*DAT_0800e778);
      if ((iVar16 != 0) && (*DAT_0800e77c != 0)) {
        if ((byte)pcVar6[0x14] < 2) {
          pcVar6[0x14] = pcVar6[0x14] + 1;
        }
        else if (*DAT_0800e780 == '\0') {
          pcVar6[0x14] = '\0';
          FUN_0802e428(*DAT_0800e778);
          *DAT_0800e778 = 0;
        }
      }
      cVar12 = *DAT_0800e784;
      if ((cVar12 != '\0') && (*DAT_0800e784 = cVar12 + -1, cVar12 == '\x01')) {
        iVar16 = FUN_080303b8(*DAT_0800e788);
        if (iVar16 != 0) {
          FUN_0802e428(*DAT_0800e788);
        }
        puVar24 = DAT_0800e78c;
        *DAT_0800e788 = *DAT_0800e78c;
        *puVar24 = 0;
      }
      cVar12 = pcVar6[9];
      if ((cVar12 != '\0') && (pcVar6[9] = cVar12 + -1, cVar12 == '\x01')) {
        *DAT_0800e790 = 1;
      }
      if ((pcVar6[10] != '\0') && (cVar12 = pcVar6[9], pcVar6[9] = cVar12 + -1, cVar12 == '\x01')) {
        *DAT_0800e790 = 0;
      }
      pcVar7 = DAT_0800e794;
      cVar12 = *DAT_0800e794;
      if ((uint)(byte)pcVar6[0x10] == (int)cVar12) {
        cVar2 = pcVar6[0x11];
        pcVar6[0x11] = cVar2 + 1U;
        if ((2 < (byte)(cVar2 + 1U)) && ((int)cVar12 != 0)) {
          *pcVar7 = '\0';
        }
      }
      else {
        pcVar6[0x10] = cVar12;
        pcVar6[0x11] = '\0';
      }
      if (*DAT_0800e798 == '\0') {
        bVar25 = 4;
      }
      else if (*DAT_0800e798 == '\x01') {
        bVar25 = 2;
      }
      if (*DAT_0800e79c == '\0') {
        if (*DAT_0800e7a4 != '\0') {
          FUN_0802e2d2(*(undefined4 *)(DAT_0800e7a0 + 0x10),1);
        }
      }
      else {
        iVar16 = FUN_080303b8(*(undefined4 *)(DAT_0800e7a0 + 0x10));
        if ((iVar16 != 0) &&
           (cVar12 = pcVar6[0x15], pcVar6[0x15] = cVar12 + 1U, bVar25 <= (byte)(cVar12 + 1U))) {
          pcVar6[0x15] = '\0';
          cVar12 = pcVar6[0x16];
          pcVar6[0x16] = cVar12 == '\0';
          if (cVar12 == '\0') {
            FUN_0802e2d2(*(undefined4 *)(DAT_0800e7a0 + 0x10),1);
          }
          else {
            FUN_0802da22(*(undefined4 *)(DAT_0800e7a0 + 0x10),1);
          }
        }
      }
      pcVar7 = DAT_0800e7a8;
      if (pcVar6[0x13] == '\0') {
        if (*DAT_0800e7a8 == -0x80) {
          cVar12 = pcVar6[0x18];
          pcVar6[0x18] = cVar12 + 1U;
          if ((3 < (byte)(cVar12 + 1U)) &&
             (FUN_0802eedc(*DAT_0800e7c4,&local_2c), local_28 == 0x1c0)) {
            *pcVar7 = pcVar6[0xe];
          }
        }
        else {
          pcVar6[0x18] = '\0';
        }
        if (*pcVar7 == 'P') {
          cVar12 = pcVar6[0x17];
          pcVar6[0x17] = cVar12 + 1U;
          if ((3 < (byte)(cVar12 + 1U)) && (FUN_0802eedc(*DAT_0800e7c8,&local_2c), local_2c == 0)) {
            *pcVar7 = '\0';
          }
        }
        else {
          pcVar6[0x17] = '\0';
        }
      }
      else {
        pcVar6[0x13] = pcVar6[0x13] + -1;
      }
      if (*DAT_0800e7ac != 0) {
        if (((*DAT_0800e780 == '\0') || (iVar16 = FUN_0802fcb6(*DAT_0800e7b0,8), iVar16 != 0)) &&
           ((*DAT_0800e7b4 == '\0' || (*pcVar7 != '\0')))) {
          if (*DAT_0800e7b8 == '\0') {
            if ((*DAT_0800e7cc == '\0') && (*DAT_0800e7bc == '\0')) {
              *DAT_0800e7bc = '\x01';
              *DAT_0800e7c0 = 1;
            }
            sVar1 = *DAT_0800e7ac;
            *DAT_0800e7ac = sVar1 + -1;
            if ((short)(sVar1 + -1) == 0) {
              *DAT_0800e7bc = '\0';
              *DAT_0800e7c0 = 1;
            }
          }
          else {
            *DAT_0800e7bc = '\0';
            *DAT_0800e7ac = 0;
            *DAT_0800e7c0 = 1;
          }
        }
        else {
          *DAT_0800e7bc = '\0';
          *DAT_0800e7ac = 0;
          *DAT_0800e7c0 = 1;
        }
      }
      if ((pcVar6[0xd] != *DAT_0800e7d0) && (pcVar6[0xd] = *DAT_0800e7d0, *DAT_0800e7ac != 0)) {
        FUN_0802286c(*pcVar7);
      }
      cVar12 = *DAT_0800e780;
      if (cVar12 == '\0') {
        *DAT_0800e7d4 = 0;
        *DAT_0800e7d8 = 0;
      }
      if (((*DAT_0800e7ac == 0) && (*pcVar7 == '\0')) && (cVar12 == '\0')) {
        iVar16 = (int)*DAT_0800e7dc;
        if (iVar16 != 0) {
          if (*(short *)(pcVar6 + 0x1a) == iVar16) {
            if (iVar16 % 0x68 != 0) {
              if (iVar16 % 0x68 < 0x34) {
                FUN_0803121c(*DAT_0800e7e0,(iVar16 / 0x68) * 0x68,1);
              }
              else {
                FUN_0803121c(*DAT_0800e7e0,(iVar16 / 0x68 + 1) * 0x68,1);
              }
            }
            *DAT_0800e7dc = 0;
          }
          else {
            *(short *)(pcVar6 + 0x1a) = *DAT_0800e7dc;
          }
        }
      }
      if ((*DAT_0800e780 == '\0') && (pcVar6[8] != *DAT_0800e7b8)) {
        if (*DAT_0800e7b8 == '\0') {
          uVar14 = FUN_0802ebc8(*(undefined4 *)(DAT_0800e7e4 + 0x14),0);
          uVar15 = FUN_08027cb4(0x27,0x27);
          FUN_0803157c(*(undefined4 *)(DAT_0800e7e4 + 0x14),uVar15,0);
          uVar15 = FUN_08027f7c();
          FUN_080316b4(uVar14,uVar15,0);
          iVar16 = FUN_080303b8(*DAT_0800e7e8);
          if (iVar16 != 0) {
            FUN_0802da22(*DAT_0800e7e8,1);
          }
          *DAT_0800ebf0 = 1;
        }
        pcVar6[8] = *DAT_0800ebf4;
      }
      if (*DAT_0800ebf8 != '\0') {
        pcVar6[0x13] = '\n';
        iVar16 = FUN_080303b8(*DAT_0800ebfc);
        if (iVar16 == 0) {
          if (*pcVar7 == 'P') {
            iVar16 = FUN_080303b8(*DAT_0800ec00);
            if (iVar16 != 0) {
              FUN_0803118e(*DAT_0800ec00,1);
            }
          }
          else if (*DAT_0800ebf4 == '\0') {
            *DAT_0800ec04 = *DAT_0800ec04 == '\0';
          }
        }
        *DAT_0800ebf8 = '\0';
      }
      if (pcVar6[0xb] != *DAT_0800ec04) {
        if (*DAT_0800ec04 == '\0') {
          FUN_0803118e(DAT_0800ec08[1],1);
        }
        else if (*pcVar7 != -0x80) {
          FUN_0803118e(DAT_0800ec08[2],1);
          pcVar6[0xe] = *pcVar7;
          *pcVar7 = -0x80;
          *DAT_0800ec04 = '\x01';
          pcVar6[0xb] = '\x01';
        }
        pcVar6[0xb] = *DAT_0800ec04;
      }
      puVar24 = DAT_0800ec08;
      if ((*DAT_0800ec0c != 0x50) && (iVar16 = FUN_080303b8(*DAT_0800ec08), iVar16 != 0)) {
        FUN_0802e2d2(*puVar24,1);
      }
      if (*pcVar6 != *DAT_0800ec10) {
        *pcVar6 = *DAT_0800ec10;
      }
      if (pcVar6[1] != *DAT_0800ec14) {
        pcVar6[1] = *DAT_0800ec14;
      }
      if (pcVar6[2] != *DAT_0800ec18) {
        pcVar6[2] = *DAT_0800ec18;
      }
      iVar16 = DAT_0800ec24;
      if (pcVar6[3] == *DAT_0800ec1c) {
        if ((pcVar6[3] != '\0' || *DAT_0800ec20 != '\0') &&
           (iVar19 = FUN_080303b8(*(undefined4 *)(DAT_0800ec24 + 0x1c)), iVar19 != 0)) {
          FUN_0802e2d2(*(undefined4 *)(iVar16 + 0x1c),1);
        }
      }
      else {
        pcVar6[3] = *DAT_0800ec1c;
      }
      puVar24 = DAT_0800ec08;
      if (*(char *)(DAT_0800ec28 + 4) != *DAT_0800ec2c) {
        uVar14 = FUN_0802ebc8(DAT_0800ec08[4],0);
        if (*DAT_0800ec2c == '\0') {
          uVar15 = FUN_08027f7c();
          FUN_080316b4(uVar14,uVar15,0);
          uVar15 = FUN_08027cb4(0x27,0x27);
          FUN_0803157c(puVar24[4],uVar15,0);
          FUN_0802d33c(uVar14,&DAT_0800ec30);
        }
        else {
          uVar15 = FUN_08027a8c();
          FUN_080316b4(uVar14,uVar15,0);
          FUN_0802d33c(uVar14,&DAT_0800ec30);
          uVar14 = FUN_08027f7c();
          FUN_0803157c(puVar24[4],uVar14,0);
        }
      }
      if (*(char *)(DAT_0800ec28 + 5) != *DAT_0800ec34) {
        *(char *)(DAT_0800ec28 + 5) = *DAT_0800ec34;
      }
      iVar16 = DAT_0800ec38;
      bVar25 = *(byte *)(DAT_0800ec28 + 6);
      uVar17 = (uint)*DAT_0800ec0c;
      if (bVar25 != uVar17) {
        *DAT_0800ebf0 = 1;
        puVar26 = DAT_0800ec50;
        puVar24 = DAT_0800ec3c;
        iVar19 = DAT_0800ec28;
        pbVar8 = DAT_0800ec0c;
        local_30 = 1;
        if (uVar17 == 0x50) {
          *(undefined1 *)(DAT_0800ec28 + 0xf) = 0;
          uVar21 = 0;
          do {
            if ((int)((uint)*(byte *)(iVar16 + uVar21 * 6 + 3) << 0x18) < 0) {
              cVar22 = cVar22 + '\x01';
            }
            uVar21 = uVar21 + 1 & 0xffff;
          } while (uVar21 < 0x10);
          *(char *)(iVar19 + 0xf) = cVar22;
          FUN_080315fa(*puVar24,0);
          FUN_080315fa(*DAT_0800ec44,0);
          FUN_080315fa(*DAT_0800ec48,0);
          FUN_080315fa(*DAT_0800ec4c,0);
        }
        else if (uVar17 - 1 < 0x10) {
          *DAT_0800ec40 = 0;
          FUN_080315fa(*puVar24,0xff,0);
          FUN_0802e2d2(*puVar24,1);
          uVar21 = (uint)*pbVar8;
          if (uVar21 != 0) {
            uVar21 = uVar21 - 1;
            do {
              if ((uVar21 & 0xffff) == 0) goto LAB_0800ea68;
              uVar21 = (uVar21 & 0xffff) - 1;
            } while (-1 < (int)((uint)*(byte *)(iVar16 + uVar21 * 6 + 3) << 0x18));
            bVar4 = true;
          }
LAB_0800ea68:
          if (bVar4) {
            FUN_080315fa(*DAT_0800ec44,0xff,0);
          }
          else {
            FUN_080315fa(*DAT_0800ec44,0);
          }
          bVar4 = false;
          uVar21 = (uint)*pbVar8;
          if (uVar21 < 0x10) {
            for (; uVar21 < 0x10; uVar21 = uVar21 + 1 & 0xffff) {
              if ((int)((uint)*(byte *)(iVar16 + uVar21 * 6 + 3) << 0x18) < 0) {
                bVar4 = true;
                break;
              }
            }
          }
          if (bVar4) {
            FUN_080315fa(*DAT_0800ec4c,0xff,0);
          }
          else {
            FUN_080315fa(*DAT_0800ec4c,0);
          }
          FUN_080315fa(*DAT_0800ec48,0xff,0);
        }
        else if (uVar17 == 0) {
          if (bVar25 == 0x50) {
            FUN_0802ee0a(*DAT_0800ec50);
            uVar17 = 0;
            do {
              if (((int)((uint)*(byte *)(iVar16 + uVar17 * 6 + 3) << 0x18) < 0) && (uVar21 == 0)) {
                uVar21 = uVar17 + 1 & 0xff;
              }
              uVar17 = uVar17 + 1 & 0xffff;
            } while (uVar17 < 0x10);
            uVar17 = 0;
            uVar20 = 0x10;
            do {
              uVar20 = uVar20 - 1;
              if ((((int)((uint)*(byte *)(iVar16 + uVar20 * 6 + 3) << 0x18) < 0) &&
                  (uVar17 = uVar17 + 1 & 0xff, uVar17 == 2)) && (uVar23 == 0)) {
                uVar23 = uVar20 & 0xff;
              }
              uVar20 = uVar20 & 0xffff;
            } while (uVar20 != 0);
            if (uVar17 - 1 < 2) {
              FUN_0803118e(puVar26[(uVar21 - 1) * 0xc],1);
            }
            else if (*(byte *)(iVar19 + 0xf) < uVar17) {
              FUN_0803118e(*DAT_0800ec54,1);
            }
            else {
              FUN_0803118e(puVar26[uVar23 * 0xc],1);
            }
          }
          FUN_080315fa(*DAT_0800ec44,0);
          FUN_080315fa(*DAT_0800ec48,0);
          FUN_080315fa(*DAT_0800ec4c,0);
          FUN_0802da22(*puVar24,1);
        }
        *(byte *)(DAT_0800ec28 + 6) = *DAT_0800ec0c;
      }
      FUN_0803840c();
      iVar18 = DAT_0800f074;
      pbVar9 = DAT_0800f070;
      iVar19 = DAT_0800ec60;
      pcVar6 = DAT_0800ec5c;
      iVar16 = DAT_0800ec28;
      pbVar8 = DAT_0800ec0c;
      if ((*DAT_0800ec58 == '\x02') || (*DAT_0800ec58 == '\x03')) {
        if ((3 < *DAT_0800f070) &&
           (iVar16 = FUN_080303b8(*(undefined4 *)(DAT_0800f074 + (*DAT_0800f070 - 1) * 0x74 + 0x28))
           , iVar16 != 0)) {
          uVar14 = FUN_08027cb4(0x4a,0x4a);
          FUN_080316b4(*(undefined4 *)(iVar18 + (*pbVar9 - 1) * 0x74 + 0x28),uVar14,0);
        }
      }
      else if (*(char *)(DAT_0800ec28 + 7) != *DAT_0800ec5c) {
        if ((*DAT_0800ec0c - 4 < 0xd) &&
           (iVar18 = FUN_080303b8(*(undefined4 *)(DAT_0800ec60 + (*DAT_0800ec0c - 1) * 0x74 + 0x28))
           , iVar18 != 0)) {
          cVar22 = *pcVar6;
          if (cVar22 == '\0') {
            uVar14 = FUN_08027f7c();
            FUN_080316b4(*(undefined4 *)(iVar19 + (*pbVar8 - 1) * 0x74 + 0x28),uVar14,0);
          }
          else if (cVar22 == '\x01') {
            uVar14 = FUN_08027cb4(0x4a,0x4a);
            FUN_080316b4(*(undefined4 *)(iVar19 + (*pbVar8 - 1) * 0x74 + 0x28),uVar14,0);
          }
        }
        *(char *)(iVar16 + 7) = *pcVar6;
      }
      uVar21 = 0;
      do {
        iVar16 = DAT_0800f078 + uVar21 * 6;
        puVar24 = (undefined4 *)(DAT_0800f07c + uVar21 * 6);
        bVar25 = *(byte *)(iVar16 + 3);
        bVar3 = *(byte *)((int)puVar24 + 3);
        if ((bVar25 & 0xc0) == (bVar3 & 0xc0)) {
          if ((int)((uint)bVar25 << 0x18) < 0) {
            cVar22 = *(char *)(DAT_0800f07c + uVar21 * 6);
            if (*(char *)(DAT_0800f078 + uVar21 * 6) != cVar22) {
              bVar5 = true;
              *(char *)(DAT_0800f078 + uVar21 * 6) = cVar22;
              uVar23 = uVar21 + 1;
              if ((*DAT_0800f070 == uVar23) ||
                 ((*DAT_0800f070 == 0x80 && (*(byte *)(DAT_0800f08c + 0xe) == uVar23)))) {
                if (cVar22 == '\0') {
                  iVar19 = DAT_0800f074 + uVar21 * 0x74;
                  FUN_0802d33c(*(undefined4 *)(iVar19 + 0x28),&DAT_0800f090);
                  if ((((*DAT_0800f094 == '\x01') && (*DAT_0800f070 == uVar23)) && (2 < uVar21)) &&
                     (uVar21 < 5)) {
                    uVar14 = FUN_08027cb4(0x4a,0x4a);
                    FUN_080316b4(*(undefined4 *)(iVar19 + 0x28),uVar14,0);
                  }
                  else {
                    uVar14 = FUN_08027f7c();
                    FUN_080316b4(*(undefined4 *)(iVar19 + 0x28),uVar14,0);
                  }
                }
                else if (cVar22 == '\x01') {
                  iVar19 = DAT_0800f074 + uVar21 * 0x74;
                  FUN_0802d33c(*(undefined4 *)(iVar19 + 0x28),&DAT_0800f098);
                  uVar14 = FUN_08027cb4(0x2a,0xe2,0x46);
                  FUN_080316b4(*(undefined4 *)(iVar19 + 0x28),uVar14,0);
                }
              }
            }
            cVar22 = *(char *)(DAT_0800f078 + uVar21 * 6);
            if (cVar22 == '\0') {
              if (*(char *)(iVar16 + 1) != *(char *)((int)puVar24 + 1)) {
                bVar5 = true;
                *(char *)(iVar16 + 1) = *(char *)((int)puVar24 + 1);
              }
            }
            else if ((cVar22 == '\x01') && (*(char *)(iVar16 + 2) != *(char *)((int)puVar24 + 2))) {
              bVar5 = true;
              *(char *)(iVar16 + 2) = *(char *)((int)puVar24 + 2);
            }
            pbVar8 = DAT_0800f070;
            uVar23 = uVar21 + 1;
            if ((*DAT_0800f070 == uVar23) ||
               ((*DAT_0800f070 == 0x80 && (*(byte *)(DAT_0800f08c + 0xe) == uVar23)))) {
              if (*DAT_0800f09c == *(char *)(DAT_0800f45c + 4)) {
                if ((*DAT_0800f09c != '\0') &&
                   ((*(char *)(iVar16 + 4) != *(char *)(puVar24 + 1) ||
                    ((*(byte *)(iVar16 + 5) & 1) != (*(byte *)((int)puVar24 + 5) & 1))))) {
                  FUN_0800d018(uVar21 & 0xff);
                  bVar25 = *(byte *)(puVar24 + 1);
                  if ((*(byte *)(iVar16 + 4) & 0x80) == (bVar25 & 0x80)) {
                    if ((*(byte *)(iVar16 + 5) & 1) != (*(byte *)((int)puVar24 + 5) & 1)) {
                      *DAT_0800f460 = '\x01';
                    }
                  }
                  else {
                    *DAT_0800f460 = '\x01';
                  }
                  *(byte *)(iVar16 + 4) = bVar25;
                  *(byte *)(iVar16 + 5) = *(byte *)((int)puVar24 + 5) & 1;
                  local_30 = 1;
                }
              }
              else {
                FUN_0800d018(uVar21 & 0xff);
                bVar25 = *(byte *)(puVar24 + 1);
                if ((*(byte *)(iVar16 + 4) & 0x80) == (bVar25 & 0x80)) {
                  if ((*(byte *)(iVar16 + 5) & 1) != (*(byte *)((int)puVar24 + 5) & 1)) {
                    *DAT_0800f460 = '\x01';
                  }
                }
                else {
                  *DAT_0800f460 = '\x01';
                }
                *(byte *)(iVar16 + 4) = bVar25;
                *(byte *)(iVar16 + 5) = *(byte *)((int)puVar24 + 5) & 1;
                local_30 = 1;
              }
            }
            if (bVar5) {
              bVar5 = false;
              iVar16 = DAT_0800f464 + uVar21 * 0x30;
              FUN_08007524(*puVar24,*(undefined2 *)(puVar24 + 1),*(undefined4 *)(iVar16 + 0x20),
                           *(undefined4 *)(iVar16 + 0x24),*(undefined4 *)(iVar16 + 0x18),
                           *(undefined4 *)(iVar16 + 0xc),*(undefined4 *)(iVar16 + 0x10),
                           *(undefined4 *)(iVar16 + 0x14));
              if ((*pbVar8 == uVar23) ||
                 ((*pbVar8 == 0x80 && (*(byte *)(DAT_0800f45c + 0xe) == uVar23)))) {
                iVar16 = DAT_0800f468 + uVar21 * 0x74;
                FUN_08007524(*puVar24,*(undefined2 *)(puVar24 + 1),*(undefined4 *)(iVar16 + 8),
                             *(undefined4 *)(iVar16 + 0xc),*(undefined4 *)(iVar16 + 0x1c),
                             *(undefined4 *)(iVar16 + 0x10),*(undefined4 *)(iVar16 + 0x14),
                             *(undefined4 *)(iVar16 + 0x18));
              }
            }
          }
        }
        else {
          if ((bVar25 & 0x80) != (bVar3 & 0x80)) {
            if ((int)((uint)bVar3 << 0x18) < 0) {
              puVar26 = (undefined4 *)(DAT_0800f080 + uVar21 * 0x30);
              iVar19 = FUN_080303b8(*puVar26);
              if (iVar19 != 0) {
                FUN_0802e2d2(*puVar26,1);
                uVar17 = 0;
                uVar23 = 0;
                do {
                  if ((int)((uint)*(byte *)(DAT_0800f07c + uVar23 * 6 + 3) << 0x18) < 0) {
                    FUN_0802db44(*(undefined4 *)(DAT_0800f080 + uVar23 * 0x30),2,0,uVar17 * 0x68);
                    uVar17 = uVar17 + 1 & 0xff;
                  }
                  uVar23 = uVar23 + 1 & 0xffff;
                } while (uVar23 < 0x10);
                FUN_0802db44(*DAT_0800f084,2,0,uVar17 * 0x68 + 2);
              }
              *(byte *)(iVar16 + 3) = *(byte *)(iVar16 + 3) | 0x80;
            }
            else {
              puVar26 = (undefined4 *)(DAT_0800f080 + uVar21 * 0x30);
              iVar19 = FUN_080303b8(*puVar26);
              if (iVar19 != 0) {
                FUN_0802da22(*puVar26,1);
                uVar17 = 0;
                uVar23 = 0;
                do {
                  if ((int)((uint)*(byte *)(DAT_0800f07c + uVar23 * 6 + 3) << 0x18) < 0) {
                    FUN_0802db44(*(undefined4 *)(DAT_0800f080 + uVar23 * 0x30),2,0,uVar17 * 0x68);
                    uVar17 = uVar17 + 1 & 0xff;
                  }
                  uVar23 = uVar23 + 1 & 0xffff;
                } while (uVar23 < 0x10);
                FUN_0802db44(*DAT_0800f084,2,0,uVar17 * 0x68 + 2);
              }
              *(byte *)(iVar16 + 3) = *(byte *)(iVar16 + 3) & 0x7f;
            }
          }
          bVar25 = *(byte *)((int)puVar24 + 3);
          if ((*(byte *)(iVar16 + 3) & 0x40) != (bVar25 & 0x40)) {
            uVar23 = (uint)*DAT_0800f070;
            if (uVar23 - 1 < 0x10) {
              *DAT_0800f088 = 1;
            }
            if ((int)((uint)bVar25 << 0x19) < 0) {
              if ((uVar23 == uVar21 + 1) ||
                 ((uVar23 == 0x80 && ((uint)*(byte *)(DAT_0800f08c + 0xe) == uVar21 + 1)))) {
                FUN_0802da22(*(undefined4 *)(DAT_0800f074 + uVar21 * 0x74 + 0x40),1);
              }
              iVar19 = DAT_0800f080 + uVar21 * 0x30;
              FUN_0802e2d2(*(undefined4 *)(iVar19 + 0x24),1);
              FUN_0802da22(*(undefined4 *)(iVar19 + 0x1c),1);
              FUN_08007524(*puVar24,*(undefined2 *)(puVar24 + 1),*(undefined4 *)(iVar19 + 0x20),
                           *(undefined4 *)(iVar19 + 0x24),*(undefined4 *)(iVar19 + 0x18),
                           *(undefined4 *)(iVar19 + 0xc),*(undefined4 *)(iVar19 + 0x10),
                           *(undefined4 *)(iVar19 + 0x14));
              *(byte *)(iVar16 + 3) = *(byte *)(iVar16 + 3) | 0x40;
            }
            else {
              if ((uVar23 == uVar21 + 1) ||
                 ((uVar23 == 0x80 && ((uint)*(byte *)(DAT_0800f08c + 0xe) == uVar21 + 1)))) {
                FUN_0802e2d2(*(undefined4 *)(DAT_0800f074 + uVar21 * 0x74 + 0x40),1);
              }
              iVar19 = DAT_0800f080 + uVar21 * 0x30;
              FUN_0802e2d2(*(undefined4 *)(iVar19 + 0x1c),1);
              FUN_0802da22(*(undefined4 *)(iVar19 + 0xc),1);
              FUN_0802da22(*(undefined4 *)(iVar19 + 0x24),1);
              FUN_0802da22(*(undefined4 *)(iVar19 + 0x10),1);
              FUN_0802da22(*(undefined4 *)(iVar19 + 0x14),1);
              FUN_0802da22(*(undefined4 *)(iVar19 + 0x18),1);
              *(byte *)(iVar16 + 3) = *(byte *)(iVar16 + 3) & 0xbf;
            }
          }
        }
        pcVar6 = DAT_0800f46c;
        uVar21 = uVar21 + 1 & 0xffff;
      } while (uVar21 < 0x10);
      *(char *)(DAT_0800f45c + 4) = *DAT_0800f46c;
      if (*DAT_0800f460 != '\0') {
        *DAT_0800f460 = '\0';
        FUN_0802286c(*DAT_0800f470);
      }
      iVar19 = DAT_0800f474;
      iVar16 = DAT_0800f464;
      if (local_30 != 0) {
        uVar21 = 0;
        do {
          if ((*pcVar6 == '\0') || (-1 < (int)((uint)*(byte *)(iVar19 + uVar21 * 6 + 4) << 0x18))) {
            FUN_0802da22(*(undefined4 *)(iVar16 + uVar21 * 0x30 + 0x2c),1);
          }
          else {
            FUN_0802e2d2(*(undefined4 *)(iVar16 + uVar21 * 0x30 + 0x2c),1);
          }
          uVar21 = uVar21 + 1 & 0xffff;
        } while (uVar21 < 0x10);
      }
      puVar24 = DAT_0800f484;
      psVar11 = DAT_0800f480;
      puVar10 = DAT_0800f47c;
      pcVar6 = DAT_0800f478;
      if (*DAT_0800f478 == '\0') {
        FUN_08004c48(auStack_3c,10,&DAT_0800f488);
        iVar16 = FUN_080303b8(*puVar24);
        if (iVar16 != 0) {
          FUN_080316cc(*puVar24,DAT_0800f48c,0);
        }
        *psVar11 = 0;
        *puVar10 = 0;
        *DAT_0800f4ac = 0;
        iVar16 = FUN_080303b8(*puVar24);
        if (iVar16 != 0) {
          FUN_0802d33c(*puVar24,auStack_3c);
        }
      }
      else {
        cVar22 = '\0';
        bVar4 = true;
        uVar21 = 0;
        do {
          if (*(byte *)(DAT_0800f474 + uVar21 * 6 + 3) >> 6 == 3) {
            if (*(char *)(DAT_0800f474 + uVar21 * 6) == '\0') {
              cVar22 = cVar22 + '\x01';
            }
            bVar4 = false;
          }
          uVar21 = uVar21 + 1 & 0xffff;
        } while (uVar21 < 0x10);
        iVar16 = FUN_080303b8(*DAT_0800f484);
        if (iVar16 != 0) {
          if (bVar4) {
            FUN_08004c48(auStack_3c,10,&DAT_0800f488);
            FUN_080316cc(*puVar24,DAT_0800f48c,0);
          }
          else {
            FUN_080316cc(*puVar24,DAT_0800f490,0);
          }
        }
        cVar12 = *DAT_0800f494;
        if ((cVar12 == '\0') && (cVar22 != '\0')) {
          iVar16 = (int)*psVar11;
          if (iVar16 == 0) {
            FUN_08004c48(auStack_3c,10,&DAT_0800f4a0);
          }
          else if (iVar16 < 0) {
            FUN_08004c48(auStack_3c,10,s___d__d_0800f498,-iVar16 / 10,-iVar16 % 10);
          }
          else {
            FUN_08004c48(auStack_3c,10,s___d__d_0800f4a4,iVar16 / 10,iVar16 % 10);
          }
        }
        else {
          iVar16 = (int)*psVar11;
          if (iVar16 == 0) {
            FUN_08004c48(auStack_3c,10,&DAT_0800f4a0);
          }
          else if (iVar16 < 0) {
            iVar19 = -iVar16;
            if ((iVar19 / 3) * -3 == iVar16) {
              cVar12 = '\0';
            }
            else if (iVar19 % 3 == 1) {
              cVar12 = '\x03';
            }
            else if (iVar19 % 3 == 2) {
              cVar12 = '\a';
            }
            FUN_08004c48(auStack_3c,10,s___d__d_0800f498,iVar19 / 3,cVar12);
          }
          else {
            if (iVar16 % 3 == 0) {
              cVar12 = '\0';
            }
            else if (iVar16 % 3 == 1) {
              cVar12 = '\x03';
            }
            else if (iVar16 % 3 == 2) {
              cVar12 = '\a';
            }
            FUN_08004c48(auStack_3c,10,s___d__d_0800f4a4,iVar16 / 3,cVar12);
          }
        }
        cVar22 = *pcVar6;
        *pcVar6 = cVar22 + -1;
        if (cVar22 == '\x01') {
          FUN_08004c48(auStack_3c,10,&DAT_0800f488);
          iVar16 = FUN_080303b8(*puVar24);
          if (iVar16 != 0) {
            FUN_080316cc(*puVar24,DAT_0800f48c,0);
          }
          *psVar11 = 0;
          *puVar10 = 0;
          *DAT_0800f4ac = 0;
        }
        iVar16 = FUN_080303b8(*puVar24);
        if (iVar16 != 0) {
          FUN_0802d33c(*puVar24,auStack_3c);
        }
      }
      puVar24 = DAT_0800f4b4;
      if ((int)((uint)*DAT_0800f4b0 << 0x18) < 0) {
        iVar16 = FUN_080303b8(*DAT_0800f4b4);
        if (iVar16 != 0) {
          FUN_0802e428(*puVar24);
        }
        *puVar24 = 0;
        FUN_0800fe78();
        return;
      }
    }
    else {
      pcVar6[0xc] = pcVar6[0xc] + -1;
      *DAT_0800e770 = 0;
      pcVar6[6] = '\0';
      *DAT_0800e774 = 1;
      pcVar6[0x13] = '\0';
    }
  }
  return;
}

