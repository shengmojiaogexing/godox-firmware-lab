// 0x08012540 FUN_08012540 size=786

void FUN_08012540(void)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  char *pcVar4;
  byte *pbVar5;
  char *pcVar6;
  char *pcVar7;
  int iVar8;
  undefined1 *puVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  uint uVar13;
  int iVar14;
  
  pcVar2 = DAT_08012854;
  cVar1 = DAT_08012854[8];
  if ((cVar1 == '\0') || (DAT_08012854[8] = cVar1 + -1, cVar1 != '\x01')) {
    return;
  }
  pcVar2[2] = *DAT_08012858;
  pcVar3 = DAT_0801285c;
  pcVar2[3] = *DAT_0801285c;
  pcVar2[6] = *DAT_08012860;
  pcVar2[4] = *DAT_08012864;
  pcVar2[1] = *DAT_08012868;
  pcVar4 = DAT_0801286c;
  pcVar2[5] = *DAT_0801286c;
  *pcVar2 = *DAT_08012870;
  pbVar5 = DAT_08012874;
  pcVar2[10] = *DAT_08012874;
  pcVar6 = DAT_08012878;
  pcVar2[0xd] = *DAT_08012878;
  pcVar7 = DAT_0801287c;
  pcVar2[0xc] = *DAT_0801287c;
  iVar8 = DAT_08012884;
  iVar14 = DAT_08012880;
  uVar13 = 0;
  do {
    iVar10 = iVar8 + uVar13 * 6;
    iVar12 = iVar14 + uVar13 * 6;
    *(undefined1 *)(iVar12 + 3) = *(undefined1 *)(iVar10 + 3);
    *(undefined1 *)(iVar14 + uVar13 * 6) = *(undefined1 *)(iVar8 + uVar13 * 6);
    *(undefined1 *)(iVar12 + 2) = *(undefined1 *)(iVar10 + 2);
    *(undefined1 *)(iVar12 + 1) = *(undefined1 *)(iVar10 + 1);
    *(byte *)(iVar12 + 5) = *(byte *)(iVar10 + 5) & 1;
    *(undefined1 *)(iVar12 + 4) = *(undefined1 *)(iVar10 + 4);
    uVar13 = uVar13 + 1 & 0xff;
  } while (uVar13 < 0x10);
  FUN_0801184c(0x50,0xb5);
  FUN_0801c118(800);
  FUN_0801184c(0x50,0xb7,*DAT_08012888);
  FUN_0801c118(800);
  if ((int)((uint)*pbVar5 << 0x18) < 0) {
    FUN_0801184c(0x50,0xb1,2);
    FUN_0801c118(800);
    FUN_0801184c(0x50,0xbf,*pcVar6);
    FUN_0801c118(800);
    FUN_0801184c(0x50,0xbe,*pcVar7);
    FUN_0801c118(800);
    puVar9 = DAT_0801288c;
    uVar13 = 0;
    do {
      if ((1 << uVar13 & (uint)*pbVar5) == 0) {
        FUN_0801184c(uVar13 + 10 & 0xff,0xbd,0xff);
      }
      else {
        FUN_0801184c(uVar13 + 10 & 0xff,0xbd,*puVar9);
      }
      FUN_0801c118(800);
      uVar13 = uVar13 + 1 & 0xff;
    } while (uVar13 < 5);
  }
  else {
    uVar13 = 0;
    do {
      if (uVar13 < 6) {
        uVar11 = uVar13 + 10;
      }
      else {
        uVar11 = uVar13 - 6;
      }
      uVar11 = uVar11 & 0xff;
      iVar14 = iVar8 + uVar13 * 6;
      if ((int)((uint)*(byte *)(iVar14 + 3) << 0x19) < 0) {
        cVar1 = *(char *)(iVar8 + uVar13 * 6);
        if (cVar1 == '\0') {
          FUN_0801184c(uVar11,0xb1,1);
          FUN_0801c118(800);
          FUN_0801184c(uVar11,0xbc,*(undefined1 *)(iVar14 + 1));
        }
        else if (cVar1 == '\x01') {
          FUN_0801184c(uVar11,0xb1,0);
          FUN_0801c118(800);
          FUN_0801184c(uVar11,0xbc,0xff);
        }
      }
      else {
        FUN_0801184c(uVar11,0xb1,1);
        FUN_0801c118(800);
        FUN_0801184c(uVar11,0xbc,0xff);
      }
      FUN_0801c118(800);
      uVar13 = uVar13 + 1 & 0xff;
    } while (uVar13 < 0x10);
  }
  if (*DAT_08012870 == '\0') {
    FUN_080118b0(0x80);
    FUN_0801c118(800);
  }
  else {
    FUN_080118b0(0x81);
    FUN_0801c118(800);
  }
  if (*DAT_08012868 == '\0') {
    FUN_080118b0(0x88);
    FUN_0801c118(800);
  }
  else {
    FUN_080118b0(0x89);
    FUN_0801c118(800);
    uVar13 = 0;
    do {
      if (uVar13 < 6) {
        uVar11 = uVar13 + 10;
      }
      else {
        uVar11 = uVar13 - 6;
      }
      uVar11 = uVar11 & 0xff;
      iVar14 = iVar8 + uVar13 * 6;
      if (((int)((uint)*(byte *)(iVar14 + 3) << 0x18) < 0) &&
         ((int)((uint)*(byte *)(iVar14 + 4) << 0x18) < 0)) {
        if ((*(byte *)(iVar14 + 5) & 1) == 0) {
          FUN_0801184c(uVar11,0xd3,1);
          FUN_0801c118(800);
          FUN_0801184c(uVar11,0xd6,0);
          FUN_0801c118(800);
          FUN_0801184c(uVar11,0xd1,*(byte *)(iVar14 + 4) & 0x7f);
        }
        else {
          FUN_0801184c(uVar11,0xd3,1);
          FUN_0801c118(800);
          FUN_0801184c(uVar11,0xd6,1);
        }
      }
      else {
        FUN_0801184c(uVar11,0xd3,0);
        FUN_0801c118(800);
        FUN_0801184c(uVar11,0xd1,0);
      }
      FUN_0801c118(800);
      uVar13 = uVar13 + 1 & 0xff;
    } while (uVar13 < 0x10);
  }
  FUN_0801184c(0x50,0xb2,*pcVar3);
  FUN_0801c118(800);
  FUN_0801184c(0x50,0xc0,*DAT_08012864);
  FUN_0801c118(800);
  if (*pcVar4 == '\0') {
    FUN_080118b0(0x40);
    FUN_0801c118(800);
  }
  else if (*pcVar4 == '\x01') {
    FUN_080118b0(0x41);
    FUN_0801c118(800);
  }
  FUN_0801c118(800);
  return;
}

