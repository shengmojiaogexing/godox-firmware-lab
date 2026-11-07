// 0x08014634 FUN_08014634 size=984

bool FUN_08014634(void)

{
  undefined4 uVar1;
  byte *pbVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  char cVar6;
  uint uVar7;
  uint extraout_r2;
  byte *extraout_r3;
  byte *pbVar8;
  ushort uVar9;
  uint uVar10;
  uint *puVar11;
  byte bVar12;
  undefined4 local_50;
  undefined1 local_4c;
  undefined1 local_4b;
  undefined1 local_4a;
  undefined1 local_49;
  char cStack_30;
  undefined4 local_2c;
  undefined1 local_28;
  undefined1 local_27;
  undefined1 local_26;
  undefined1 local_25;
  
  iVar4 = DAT_0801479c;
  cVar6 = '\0';
  *(undefined1 *)(DAT_0801479c + 0xb) = 0;
  pbVar8 = DAT_080147a0;
  *DAT_080147a0 = 0xa0;
  pbVar8[1] = *DAT_080147a4;
  pbVar8[2] = *DAT_080147a8 | *(byte *)(iVar4 + 0xe);
  iVar4 = DAT_080147b4;
  bVar3 = *DAT_080147b0;
  if (*DAT_080147ac == '\x01') {
    pbVar8[3] = 4;
  }
  else if ((int)((uint)bVar3 << 0x18) < 0) {
    pbVar8[3] = 2;
  }
  else {
    uVar7 = 0;
    do {
      if (*(char *)(DAT_080147b4 + uVar7 * 6) == '\x01') {
        cVar6 = cVar6 + '\x01';
      }
      uVar7 = uVar7 + 1 & 0xff;
    } while (uVar7 < 0x10);
    if (cVar6 == '\0') {
      pbVar8[3] = 1;
    }
    else {
      pbVar8[3] = 0;
    }
  }
  pbVar8[4] = *DAT_080147b8;
  pbVar8[5] = *DAT_080147bc;
  pbVar8[6] = *DAT_080147c0;
  pbVar8[7] = *DAT_080147c4;
  pbVar8[8] = 0;
  bVar12 = *DAT_080147c8;
  if (bVar12 < 0x19) {
    pbVar8[9] = 1;
  }
  else if (bVar12 < 0x1d) {
    pbVar8[9] = 2;
  }
  else if (bVar12 < 0x24) {
    pbVar8[9] = 3;
  }
  else if (bVar12 < 0x33) {
    pbVar8[9] = 4;
  }
  else if (bVar12 < 0x47) {
    pbVar8[9] = 5;
  }
  else if (bVar12 < 0x51) {
    pbVar8[9] = 6;
  }
  else if (bVar12 < 0x6a) {
    pbVar8[9] = 7;
  }
  else if (bVar12 < 0x88) {
    pbVar8[9] = 8;
  }
  else {
    pbVar8[9] = 9;
  }
  if (*DAT_080147cc != '\0') {
    pbVar8[9] = pbVar8[9] | 0x80;
  }
  pbVar8[10] = bVar3;
  bVar3 = *DAT_080147d0;
  if (0x46 < bVar3 - 0x14) {
    bVar3 = 0x5a;
  }
  pbVar8[0xb] = bVar3;
  pbVar8[0xc] = *DAT_080147d4;
  pbVar8[0xd] = *DAT_080147d8;
  uVar7 = 0;
  do {
    iVar5 = iVar4 + uVar7 * 6;
    if (*(byte *)(iVar5 + 3) >> 6 == 3) {
      cVar6 = *(char *)(iVar4 + uVar7 * 6);
      if (cVar6 == '\0') {
        pbVar8[uVar7 * 2 + 0xe] = 1;
        pbVar8[uVar7 * 2 + 0xf] = *(byte *)(iVar5 + 1);
      }
      else if (cVar6 == '\x01') {
        pbVar8[uVar7 * 2 + 0xe] = 0;
        bVar3 = FUN_08008560(*(undefined1 *)(iVar5 + 2));
        pbVar8[uVar7 * 2 + 0xf] = bVar3;
        uVar7 = extraout_r2;
        pbVar8 = extraout_r3;
      }
    }
    else {
      pbVar8[uVar7 * 2 + 0xe] = 6;
      pbVar8[uVar7 * 2 + 0xf] = 0;
    }
    pbVar2 = DAT_080147a0;
    uVar7 = uVar7 + 1 & 0xff;
  } while (uVar7 < 5);
  uVar7 = 0x18;
  cVar6 = -0x5f;
  bVar12 = 0;
  FUN_080380c4();
  iVar4 = DAT_08013b4c;
  bVar3 = *pbVar2;
  if (bVar3 == 0xc3) {
    cVar6 = -0x3c;
  }
  else if (bVar3 < 0xc4) {
    if (bVar3 != 0xa0) {
      if (bVar3 == 0xc0) {
        cVar6 = -0x3f;
      }
      else if (bVar3 == 0xc1) {
        cVar6 = -0x3e;
      }
    }
  }
  else if (bVar3 == 0xc5) {
    cVar6 = -0x3a;
  }
  else if (bVar3 == 0xc6) {
    cVar6 = -0x39;
  }
  else if (bVar3 == 200) {
    cVar6 = -0x37;
  }
  if (bVar3 == 0xc0) {
    for (uVar10 = 0; uVar10 < 0x18; uVar10 = uVar10 + 1 & 0xff) {
      *(byte *)(iVar4 + uVar10) = pbVar2[uVar10];
    }
  }
  else {
    for (uVar7 = 1; uVar7 < 0x18; uVar7 = uVar7 + 1 & 0xff) {
      cVar6 = pbVar2[uVar7] + cVar6;
    }
    for (uVar7 = 0; uVar7 < 0x18; uVar7 = uVar7 + 1 & 0xff) {
      *(byte *)(iVar4 + uVar7) = pbVar2[uVar7];
    }
    *(char *)(iVar4 + 0x18) = cVar6;
    uVar7 = 0x19;
    cStack_30 = cVar6;
  }
  FUN_08035c56(0x28);
  FUN_08038400(DAT_08013b50,0);
  do {
    *(undefined1 *)(DAT_08013b54 + 2) = 0;
    FUN_08038128(&local_50);
    uVar1 = DAT_08013b50;
    local_50 = 0;
    local_4c = 0;
    local_4b = 1;
    local_4a = 1;
    local_49 = 1;
    FUN_080382c8(DAT_08013b50,&local_50);
    FUN_08038224(uVar1,0x40,0);
    iVar4 = DAT_08013b58;
    FUN_08022e34(DAT_08013b58,0x400);
    local_25 = 2;
    local_28 = 0;
    local_26 = 1;
    local_2c = 0x400;
    local_27 = 0;
    FUN_08022e50(iVar4,&local_2c);
    iVar4 = DAT_08013b5c;
    uVar9 = 0;
    while ((iVar5 = FUN_08022eb4(iVar4,0x40), iVar5 == 1 && (uVar9 = uVar9 + 1, uVar9 < 0x1389))) {
      FUN_0801c118(1);
    }
    for (uVar10 = 0; uVar10 < uVar7; uVar10 = uVar10 + 1 & 0xff) {
      uVar9 = 0;
      FUN_080381d6(uVar1,*(undefined1 *)(DAT_08013b4c + uVar10));
      do {
        iVar4 = FUN_08038216(uVar1,2);
        if (iVar4 != 0) break;
        uVar9 = uVar9 + 1;
      } while (uVar9 < 2000);
    }
    FUN_08038224(uVar1,0x40,1);
    puVar11 = (uint *)(DAT_08013b5c + 0x14);
    *puVar11 = *puVar11 ^ 0x1000;
    iVar4 = DAT_08013b54;
    *(undefined1 *)(DAT_08013b54 + 4) = 1;
    *(undefined1 *)(iVar4 + 2) = 0;
    *(undefined1 *)(iVar4 + 5) = 0;
    uVar9 = 0;
    do {
      uVar9 = uVar9 + 1;
      FUN_0801c118(1);
    } while (uVar9 < 100);
    *(undefined1 *)(iVar4 + 4) = 0;
    *puVar11 = *puVar11 ^ 0x1000;
    if ((*pbVar2 == *(byte *)(iVar4 + 5)) || (*(byte *)(DAT_08013b4c + 0x48) == *pbVar2)) break;
    local_25 = 2;
    local_28 = 0;
    local_26 = 0;
    local_2c = 0x400;
    local_27 = 1;
    FUN_08022e50(DAT_08013b58,&local_2c);
    bVar12 = bVar12 + 1;
    FUN_0801c118(500);
  } while (bVar12 < 3);
  *(uint *)(DAT_08013b58 + 0x14) = *(uint *)(DAT_08013b58 + 0x14) | 0x400;
  local_25 = 2;
  local_28 = 0;
  local_26 = 0;
  local_2c = 0x400;
  local_27 = 1;
  FUN_08022e50(DAT_08013b58,&local_2c);
  FUN_080380c4();
  FUN_08035c78(0x28,1,0);
  FUN_08038400(DAT_08013b50,1);
  if (2 < bVar12) {
    FUN_080380c4();
  }
  return 2 < bVar12;
}

