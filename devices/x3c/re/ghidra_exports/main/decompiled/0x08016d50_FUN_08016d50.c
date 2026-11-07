// 0x08016d50 FUN_08016d50 size=592

void FUN_08016d50(void)

{
  byte bVar1;
  char cVar2;
  short sVar3;
  byte *pbVar4;
  short *psVar5;
  char *pcVar6;
  undefined2 *puVar7;
  int *piVar8;
  int *piVar9;
  uint *puVar10;
  int *piVar11;
  int iVar12;
  int iVar13;
  uint uVar14;
  bool bVar15;
  
  iVar12 = FUN_08039916(DAT_08016fa0,1);
  if (iVar12 != 1) {
    return;
  }
  FUN_0801a638();
  pbVar4 = DAT_08016fa4;
  bVar1 = *DAT_08016fa4;
  *DAT_08016fa4 = bVar1 + 1;
  bVar15 = 9 < (byte)(bVar1 + 1);
  do {
    if (!bVar15) goto LAB_08016f8c;
    if (*DAT_08016fa8 != 0) {
      *DAT_08016fa8 = *DAT_08016fa8 + -1;
    }
    iVar12 = DAT_08016fb0;
    sVar3 = *DAT_08016fac;
    if ((sVar3 != 0) && (*DAT_08016fac = sVar3 + -1, sVar3 == 1)) {
      *(uint *)(iVar12 + 0x14) = *(uint *)(iVar12 + 0x14) & 0xffffbfff;
    }
    *pbVar4 = 0;
    FUN_080334ec(1);
    iVar13 = FUN_080220d4();
    if (iVar13 == 0) {
      pbVar4[7] = 0;
    }
    else {
      bVar1 = pbVar4[7];
      pbVar4[7] = bVar1 + 1;
      if (2 < bVar1) {
        FUN_0801afe8();
      }
    }
    bVar1 = pbVar4[1];
    pbVar4[1] = bVar1 + 1;
    bVar15 = 4 < (byte)(bVar1 + 1);
  } while (!bVar15);
  pbVar4[4] = 1;
  if (*DAT_08016fb4 != '\0') {
    *DAT_08016fb4 = *DAT_08016fb4 + -1;
  }
  if (*DAT_08016fb8 != '\0') {
    *DAT_08016fb8 = *DAT_08016fb8 + -1;
  }
  bVar1 = pbVar4[5];
  pbVar4[5] = bVar1 + 1;
  if (4 < (byte)(bVar1 + 1)) {
    pbVar4[5] = 0;
    if (*DAT_08016fbc != '\0') {
      *DAT_08016fbc = *DAT_08016fbc + -1;
    }
  }
  pbVar4[1] = 0;
  if (*DAT_08016fc0 != '\0') {
    *DAT_08016fc0 = *DAT_08016fc0 + -1;
  }
  psVar5 = DAT_08016fc4;
  if (*DAT_08016fc4 != 0) {
    *DAT_08016fc4 = *DAT_08016fc4 + -1;
  }
  if (*DAT_08016fc8 != '\0') {
    *DAT_08016fcc = *DAT_08016fcc + 1;
  }
  puVar7 = DAT_08016fd4;
  pcVar6 = DAT_08016fd0;
  if (*DAT_08016fd0 == '\0') {
    iVar13 = FUN_0803a9f2(DAT_08016fd8);
    if (iVar13 == 3) {
      bVar1 = pbVar4[8];
      pbVar4[8] = bVar1 + 1;
      if (0x14 < (byte)(bVar1 + 1)) {
        *pcVar6 = '\x01';
        *puVar7 = 2000;
      }
    }
    else {
      *pcVar6 = '\0';
      pbVar4[8] = 0;
    }
  }
  else {
    iVar13 = FUN_08022eb4(iVar12,1);
    if (((iVar13 == 0) && (bVar1 = pbVar4[8], bVar1 != 0)) && (pbVar4[8] = bVar1 - 1, bVar1 == 1)) {
      *pcVar6 = '\0';
    }
  }
  if (*DAT_08016fdc != '\0') {
    bVar1 = pbVar4[3];
    pbVar4[3] = bVar1 + 1;
    if (0x1d < (byte)(bVar1 + 1)) {
      pbVar4[3] = 0;
      pbVar4[6] = 1;
    }
    iVar12 = FUN_08022eb4(iVar12,0x100);
    if (iVar12 == 0) {
      bVar1 = pbVar4[9];
      pbVar4[9] = bVar1 + 1;
      if ((byte)(bVar1 + 1) == 200) {
        *DAT_08016ffc = *DAT_08016ffc + '\x01';
      }
    }
    else {
      pbVar4[9] = 0;
    }
  }
  iVar12 = FUN_08017184();
  piVar11 = DAT_08016fec;
  puVar10 = DAT_08016fe8;
  piVar9 = DAT_08016fe4;
  piVar8 = DAT_08016fe0;
  if (iVar12 != 0) {
    *DAT_08016fe0 = *DAT_08016fec;
    *piVar9 = *piVar11;
    *puVar10 = (uint)*DAT_08016ff0;
    *puVar7 = 2000;
  }
  bVar1 = pbVar4[2];
  pbVar4[2] = bVar1 + 1;
  if (199 < (byte)(bVar1 + 1)) {
    pbVar4[2] = 0;
    puVar7 = DAT_08016ff8;
    cVar2 = *DAT_08016ff4;
    if (cVar2 == '\a') {
      if (*piVar9 == 0) {
        bVar1 = pbVar4[0xc];
        if ((bVar1 != 0) && (pbVar4[0xc] = bVar1 - 1, bVar1 == 1)) {
          *puVar7 = 400;
        }
      }
      else {
        pbVar4[0xc] = 0x14;
      }
    }
    bVar1 = pbVar4[0xb];
    if ((bVar1 != 0) && (pbVar4[0xb] = bVar1 - 1, bVar1 == 1)) {
      pbVar4[0xb] = 0x1e;
      *DAT_08017000 = 1;
    }
    if (((((int)((uint)*DAT_08017004 << 0x18) < 0) && (*DAT_08017008 == '\0')) && (cVar2 != '\a'))
       && (iVar12 = *piVar8, iVar12 != 0)) {
      iVar12 = iVar12 + -1;
      *piVar8 = iVar12;
      *puVar7 = 0;
      if (iVar12 == 0) {
        if (*psVar5 == 0) {
          FUN_08006064();
        }
        *DAT_0801700c = 1;
        *puVar7 = 400;
      }
    }
    if (*piVar9 != 0) {
      *piVar9 = *piVar9 + -1;
    }
    if ((((int)((uint)*DAT_08017010 << 0x18) < 0) && (*puVar10 != 0)) &&
       (uVar14 = *puVar10 - 1, *puVar10 = uVar14, uVar14 == 0)) {
      FUN_08035fd0();
    }
    if (((*DAT_08017014 != '\0') && (*(short *)(pbVar4 + 0x12) != 0)) &&
       (*(short *)(pbVar4 + 0x12) = *(short *)(pbVar4 + 0x12) + -1, *(short *)(pbVar4 + 0x12) == 0))
    {
      FUN_08035fd0();
    }
  }
LAB_08016f8c:
  FUN_08006e5c();
  FUN_08039910(DAT_08016fa0,1);
  return;
}

