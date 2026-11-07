// 0x08005d64 FUN_08005d64 size=554

void FUN_08005d64(void)

{
  short sVar1;
  undefined2 uVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  ushort uVar7;
  ushort uVar8;
  ushort uVar9;
  ushort uVar10;
  int iVar11;
  undefined4 uVar12;
  int iVar13;
  int iVar14;
  byte *pbVar15;
  byte bVar16;
  uint uVar17;
  uint uVar18;
  byte bVar19;
  int iVar20;
  int iVar21;
  
  iVar11 = DAT_08005f90;
  bVar16 = *(char *)(DAT_08005f90 + 10) + 1;
  *(byte *)(DAT_08005f90 + 10) = bVar16;
  if (3 < bVar16) {
    *(undefined1 *)(iVar11 + 10) = 0;
    sVar1 = *(short *)(iVar11 + 0x1a);
    if ((sVar1 != 0) && (*(short *)(iVar11 + 0x1a) = sVar1 + -1, sVar1 == 1)) {
      *(undefined1 *)(iVar11 + 0xc) = 1;
    }
    uVar12 = DAT_08005f94;
    FUN_08018c88(DAT_08005f94,1);
    FUN_08018e46(uVar12,1);
    FUN_0801c118(200);
    FUN_0801c31c(2);
    iVar13 = DAT_08005f9c;
    uVar2 = DAT_08005f98[1];
    uVar17 = (uint)*(byte *)(iVar11 + 4);
    *(undefined2 *)(DAT_08005f9c + uVar17 * 2) = *DAT_08005f98;
    iVar21 = iVar13 + 0x10;
    *(undefined2 *)(iVar21 + uVar17 * 2) = uVar2;
    *(char *)(iVar11 + 4) = (char)(uVar17 + 1);
    if (7 < (uVar17 + 1 & 0xff)) {
      *(undefined1 *)(iVar11 + 4) = 0;
    }
    iVar14 = DAT_08005f9c;
    uVar17 = 0;
    iVar20 = DAT_08005f9c + 0x20;
    do {
      *(undefined2 *)(iVar20 + uVar17 * 2) = *(undefined2 *)(iVar13 + uVar17 * 2);
      uVar17 = uVar17 + 1 & 0xff;
    } while (uVar17 < 8);
    uVar17 = 0;
    do {
      for (uVar18 = uVar17; uVar18 < 8; uVar18 = uVar18 + 1 & 0xff) {
        uVar3 = *(ushort *)(iVar20 + uVar17 * 2);
        uVar4 = *(ushort *)(iVar20 + uVar18 * 2);
        if (uVar4 < uVar3) {
          *(ushort *)(iVar20 + uVar18 * 2) = uVar3;
          *(ushort *)(iVar20 + uVar17 * 2) = uVar4;
        }
      }
      uVar17 = uVar17 + 1 & 0xff;
    } while (uVar17 < 8);
    uVar3 = *(ushort *)(iVar14 + 0x22);
    uVar4 = *(ushort *)(iVar14 + 0x24);
    uVar5 = *(ushort *)(iVar14 + 0x28);
    uVar6 = *(ushort *)(iVar14 + 0x26);
    uVar7 = *(ushort *)(iVar14 + 0x2a);
    uVar8 = *(ushort *)(iVar14 + 0x2c);
    uVar17 = 0;
    do {
      *(undefined2 *)(iVar20 + uVar17 * 2) = *(undefined2 *)(iVar21 + uVar17 * 2);
      uVar17 = uVar17 + 1 & 0xff;
    } while (uVar17 < 8);
    uVar17 = 0;
    do {
      for (uVar18 = uVar17; pbVar15 = DAT_08005fa4, uVar18 < 8; uVar18 = uVar18 + 1 & 0xff) {
        uVar9 = *(ushort *)(iVar20 + uVar17 * 2);
        uVar10 = *(ushort *)(iVar20 + uVar18 * 2);
        if (uVar10 < uVar9) {
          *(ushort *)(iVar20 + uVar18 * 2) = uVar9;
          *(ushort *)(iVar20 + uVar17 * 2) = uVar10;
        }
      }
      uVar17 = uVar17 + 1 & 0xff;
    } while (uVar17 < 8);
    uVar17 = (uint)((ulonglong)
                    ((((uint)uVar3 + (uint)uVar4 + (uint)uVar6 + (uint)uVar5 + (uint)uVar7 +
                      (uint)uVar8) / 6) * 1000) /
                   ((ulonglong)
                    (ushort)(*(short *)(iVar14 + 0x22) + *(short *)(iVar14 + 0x24) +
                             *(short *)(iVar14 + 0x26) + *(short *)(iVar14 + 0x28) +
                             *(short *)(iVar14 + 0x2a) + *(short *)(iVar14 + 0x2c)) / 6));
    if (*DAT_08005fa0 == '\0') {
      if (*DAT_08005fac == '\0') {
        uVar18 = (uint)*(ushort *)(DAT_08005fa8 + -0x14 + (uint)*DAT_08005fa4 * 2);
      }
      else {
        uVar18 = *(ushort *)(DAT_08005fa8 + -0x14 + (uint)*DAT_08005fa4 * 2) + 0x14;
      }
      if (((*(char *)(iVar11 + 0xc) == '\0') && (uVar18 < uVar17)) &&
         (bVar16 = *DAT_08005fa4, bVar16 < 4)) {
        bVar19 = *(char *)(iVar11 + 5) + 1;
        *(byte *)(iVar11 + 5) = bVar19;
        if (4 < bVar19) {
          *(undefined1 *)(iVar11 + 5) = 0;
          *pbVar15 = bVar16 + 1;
        }
        *(undefined1 *)(iVar11 + 6) = 0;
      }
      bVar16 = *pbVar15;
      if ((uVar17 < *(ushort *)(DAT_08005fa8 + -10 + (uint)bVar16 * 2)) && (bVar16 != 0)) {
        bVar19 = *(char *)(iVar11 + 6) + 1;
        *(byte *)(iVar11 + 6) = bVar19;
        if (4 < bVar19) {
          *(undefined1 *)(iVar11 + 6) = 0;
          *pbVar15 = bVar16 - 1;
        }
        *(undefined1 *)(iVar11 + 5) = 0;
      }
      else {
        *(undefined1 *)(iVar11 + 6) = 0;
      }
    }
    else {
      *(undefined1 *)(iVar11 + 0xc) = 1;
      if (*(char *)(iVar11 + 3) == '\0') {
        if (*pbVar15 < 4) {
          *(byte *)(iVar11 + 0xb) = *pbVar15;
        }
        else {
          *(undefined1 *)(iVar11 + 0xb) = 3;
        }
      }
      bVar16 = *pbVar15;
      if ((*(ushort *)(DAT_08005fa8 + (uint)bVar16 * 2) < uVar17) && (bVar16 < 3)) {
        *pbVar15 = bVar16 + 1;
      }
    }
    if (*(char *)(iVar11 + 9) == '\0') {
      bVar16 = *(char *)(iVar11 + 8) + 1;
      *(byte *)(iVar11 + 8) = bVar16;
      if ((10 < bVar16) && (*(undefined1 *)(iVar11 + 8) = 0, *pbVar15 == 0)) {
        if (uVar17 < 0x276) {
          bVar16 = *(char *)(iVar11 + 7) + 1;
          *(byte *)(iVar11 + 7) = bVar16;
          if (10 < bVar16) {
            *(undefined1 *)(iVar11 + 7) = 0;
            *DAT_08005fb0 = 2;
            FUN_0801956c();
            *(undefined1 *)(iVar11 + 1) = 1;
            while (*(ushort *)(iVar11 + 0x18) < 800) {
              FUN_0803ba4c();
              FUN_080335bc();
            }
            FUN_08014634();
            FUN_08010c00();
            return;
          }
        }
        else {
          *(undefined1 *)(iVar11 + 7) = 0;
        }
      }
    }
    else {
      *(char *)(iVar11 + 9) = *(char *)(iVar11 + 9) + -1;
    }
  }
  return;
}

