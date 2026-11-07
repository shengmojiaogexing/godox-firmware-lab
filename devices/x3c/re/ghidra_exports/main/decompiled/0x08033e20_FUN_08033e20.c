// 0x08033e20 FUN_08033e20 size=1136

void FUN_08033e20(void)

{
  byte bVar1;
  char cVar2;
  bool bVar3;
  char *pcVar4;
  undefined1 *puVar5;
  ushort *puVar6;
  char *pcVar7;
  undefined1 *puVar8;
  undefined2 *puVar9;
  uint *puVar10;
  char *pcVar11;
  char *pcVar12;
  int iVar13;
  uint uVar14;
  byte bVar15;
  byte bVar16;
  ushort uVar17;
  bool bVar18;
  
  bVar15 = 0;
  bVar16 = 0;
  bVar3 = false;
  uVar17 = 0;
  bVar18 = false;
  FUN_08035d38(0x8000000,0x4800);
  FUN_080393c4();
  *DAT_08034234 = '\x01';
  FUN_080354e4();
  FUN_0800cfa4();
  FUN_08022e34(DAT_08034238,0x200);
  FUN_08022e34(DAT_0803423c,4);
  FUN_08035280();
  FUN_08035a40();
  FUN_08023b00();
  FUN_080112e8();
  bVar1 = *DAT_08034240;
  *DAT_08034244 = bVar1;
  if (bVar1 < 9) {
    if (bVar1 != 1) {
      *DAT_0803424c = 1;
    }
    *DAT_08034250 = bVar1;
  }
  else {
    *DAT_08034248 = 0;
  }
  FUN_0802c924();
  FUN_08031a38();
  FUN_08031a84();
  FUN_08010c8c();
  FUN_0800cfe4();
  FUN_08022e38(DAT_08034238,0x200);
  FUN_08022e38(DAT_0803423c,4);
  puVar5 = DAT_08034254;
  *DAT_08034254 = 1;
  FUN_08022e38(DAT_08034238,0x200);
  FUN_08022e38(DAT_0803423c,4);
  FUN_080393c4();
  FUN_0801c0fe(4);
  FUN_0800830c();
  FUN_08036c54();
  iVar13 = FUN_08022eb4(DAT_08034238,0x1000);
  if ((iVar13 == 0) || (iVar13 = FUN_08022eb4(DAT_08034258,0x200), iVar13 == 0)) {
    FUN_0801c0fe(0x14);
    iVar13 = FUN_08022eb4(DAT_08034238,0x1000);
    if ((iVar13 == 0) || (iVar13 = FUN_08022eb4(DAT_08034258,0x200), iVar13 == 0)) {
      FUN_08006f58();
      FUN_08025a04();
      *DAT_0803425c = '\0';
      FUN_08012390();
      FUN_080119dc();
    }
  }
  FUN_080359b4();
  FUN_08035280();
  FUN_080353a0();
  iVar13 = FUN_08022eb4(DAT_08034238,0x8000);
  if (iVar13 == 0) {
    FUN_0801c0fe(0x14);
    iVar13 = FUN_08022eb4(DAT_08034238,0x8000);
    if (iVar13 == 0) {
      *DAT_08034260 = 0;
      bVar18 = true;
    }
  }
  if (*DAT_0803425c != '\0') {
    FUN_08025a04();
  }
  FUN_080374b0();
  FUN_08022e34(DAT_08034258,0x4000);
  FUN_08006f58();
  FUN_0803ba4c();
  puVar6 = DAT_08034264;
  if (bVar18) {
    FUN_08023f88();
    DAT_08034268[3] = '2';
    FUN_0803ba4c();
    FUN_08015b38(0xffff);
    FUN_0801c0fe(0x5dc);
    FUN_0800ba08();
    if ((int)((uint)*DAT_0803426c << 0x18) < 0) {
      FUN_0800fe78();
    }
    else {
      FUN_080345b4(0);
    }
    FUN_080358fc();
    FUN_080335bc();
    FUN_080335bc();
    FUN_080335bc();
    FUN_080335bc();
    puVar9 = DAT_08034278;
    puVar8 = DAT_08034274;
    if (*DAT_08034270 == '\0') {
      if (*DAT_08034250 == 7) {
        *DAT_0803427c = 3;
        *puVar8 = 1;
        FUN_0801445c();
        FUN_0803ba4c();
        FUN_0801c0fe(500);
        *puVar9 = 1;
        FUN_0802007c();
        *puVar6 = 2000;
      }
      else {
        *DAT_08034274 = 1;
        *DAT_0803427c = 2;
        FUN_0801445c();
        FUN_0803ba4c();
        FUN_0801c0fe(1000);
        *puVar9 = 1;
        FUN_0802007c();
      }
    }
    bVar3 = true;
    *puVar5 = 0;
  }
  *DAT_08034284 = (uint)*DAT_08034280;
  puVar10 = DAT_08034288;
  *DAT_0803428c = *DAT_08034288;
  *DAT_08034290 = *puVar10;
  *DAT_08034294 = 1;
  *DAT_0803427c = 0;
  *DAT_08034298 = 0;
  FUN_0803a7e4();
  if (!bVar3) {
    if (*DAT_08034270 == '\0') {
      FUN_08005fb4();
    }
    if (*DAT_0803429c != '\0') {
      FUN_08023bc0();
    }
    FUN_08006134();
    pcVar4 = DAT_08034268;
    pcVar4[4] = -4;
    pcVar4[5] = '\b';
    do {
      FUN_0803ba4c();
      FUN_080335bc();
    } while (*(short *)(pcVar4 + 4) != 0);
    FUN_0800ba08();
    if (*DAT_080342a0 != '\0') {
      FUN_08025618();
      pcVar4[6] = 'p';
      pcVar4[7] = '\x17';
      *DAT_080342a4 = 0;
      *pcVar4 = '\0';
      while (*(short *)(pcVar4 + 6) != 0) {
        FUN_0803ba4c();
        FUN_080335bc();
        if (*DAT_080342a8 != '\0') {
          *DAT_080342a8 = '\0';
          iVar13 = FUN_08022eb4(DAT_08034258,0x100);
          if (iVar13 == 0) {
            uVar17 = uVar17 + 1;
            if (200 < uVar17) {
              FUN_08035fd0();
            }
          }
          else {
            uVar17 = 0;
            *puVar5 = 0;
          }
          if ((*DAT_080342a4 != 0) && (*pcVar4 == '\0')) {
            *pcVar4 = (char)*DAT_080342a4;
            FUN_0800d340();
          }
        }
      }
      if (*DAT_080342ac == '\0') {
        *DAT_080342b0 = 1;
      }
      FUN_08025840();
      pcVar4[6] = '2';
      pcVar4[7] = '\0';
      do {
        FUN_0803ba4c();
        FUN_080335bc();
      } while (*(short *)(pcVar4 + 6) != 0);
      *DAT_080342a0 = '\0';
    }
    FUN_080358fc();
    if ((int)((uint)*DAT_0803426c << 0x18) < 0) {
      FUN_0800fe78();
    }
    else {
      FUN_080345b4(0);
    }
  }
  FUN_08023f88();
  *DAT_080342b4 = 0x28;
  FUN_08035944();
  *DAT_080342b8 = 1;
  *DAT_080342bc = 0;
  pcVar12 = DAT_080342c0;
  pcVar7 = DAT_08034268;
  pcVar4 = DAT_08034234;
  do {
    do {
      FUN_0803ba4c();
      if (((*pcVar4 == '\0') || (*pcVar12 != '\0')) || (pcVar7[1] != '\0')) {
        pcVar7[1] = '\0';
        FUN_080335bc();
      }
      pcVar11 = DAT_080342a8;
    } while (*DAT_080342a8 == '\0');
    bVar16 = bVar16 + 1;
    if (4 < bVar16) {
      bVar16 = 0;
      if (pcVar7[3] != '\0') {
        pcVar7[3] = pcVar7[3] + -1;
      }
    }
    if (((*DAT_08034270 != '\0') && (*pcVar4 == '\x01')) &&
       (cVar2 = pcVar7[2], pcVar7[2] = cVar2 + 1U, 100 < (byte)(cVar2 + 1U))) {
      pcVar7[2] = '\0';
      pcVar7[1] = '\x01';
    }
    if (*pcVar12 != '\0') {
      *pcVar12 = *pcVar12 + -1;
    }
    if (*DAT_080342c4 != 0) {
      *DAT_080342c4 = *DAT_080342c4 + -1;
    }
    *pcVar11 = '\0';
    FUN_0803ba4c();
    FUN_080089d0();
    FUN_08013bac();
    iVar13 = FUN_0800c1e8();
    if (iVar13 == 1) {
      *puVar6 = *puVar6 + 2000;
      *DAT_08034290 = *DAT_08034290 + 5;
    }
    if (*DAT_080342c8 != '\0') {
      if (*DAT_080342cc == '\0') {
        *DAT_080342c8 = '\0';
      }
      else {
        uVar14 = *DAT_0803428c;
        if (uVar14 < 3) {
          uVar14 = 2;
        }
        *DAT_0803428c = uVar14;
        uVar17 = *puVar6;
        if (uVar17 < 0x15) {
          uVar17 = 0x14;
        }
        *puVar6 = uVar17;
      }
    }
    FUN_0802007c();
    FUN_08013ce4();
    FUN_080118e8(2,*DAT_0803432c + -1,*DAT_08034330);
    FUN_080391c8();
    bVar15 = bVar15 + 1;
    if (4 < bVar15) {
      bVar15 = 0;
      FUN_08012910();
      FUN_08012890();
      FUN_08012540();
      iVar13 = FUN_08022eb4(DAT_08034334,0x400);
      if (iVar13 == 1) {
        FUN_0801445c();
      }
      FUN_08005d64();
      FUN_0800cc30();
      FUN_0801fffc();
    }
  } while( true );
}

