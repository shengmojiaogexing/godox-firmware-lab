// 0x0801445c FUN_0801445c size=402

void FUN_0801445c(void)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  byte bVar4;
  undefined1 uVar5;
  int iVar6;
  int iVar7;
  char cVar8;
  uint uVar9;
  uint extraout_r2;
  undefined1 *extraout_r3;
  undefined1 *puVar10;
  
  iVar3 = DAT_080145f0;
  cVar8 = '\0';
  cVar1 = *(char *)(DAT_080145f0 + 0xb);
  if ((cVar1 != '\0') && (*(char *)(DAT_080145f0 + 0xb) = cVar1 + -1, cVar1 == '\x01')) {
    *(undefined1 *)(iVar3 + 0xb) = 0;
    puVar10 = DAT_080145f4;
    *DAT_080145f4 = 0xa0;
    puVar10[1] = *DAT_080145f8;
    puVar10[2] = *DAT_080145fc | *(byte *)(iVar3 + 0xe);
    iVar7 = DAT_08014608;
    bVar4 = *DAT_08014604;
    if (*DAT_08014600 == '\x01') {
      puVar10[3] = 4;
    }
    else if ((int)((uint)bVar4 << 0x18) < 0) {
      puVar10[3] = 2;
    }
    else {
      uVar9 = 0;
      do {
        if (*(char *)(DAT_08014608 + uVar9 * 6) == '\x01') {
          cVar8 = cVar8 + '\x01';
        }
        uVar9 = uVar9 + 1 & 0xff;
      } while (uVar9 < 0x10);
      if (cVar8 == '\0') {
        puVar10[3] = 1;
      }
      else {
        puVar10[3] = 0;
      }
    }
    puVar10[4] = *DAT_0801460c;
    puVar10[5] = *DAT_08014610;
    puVar10[6] = *DAT_08014614;
    puVar10[7] = *DAT_08014618;
    puVar10[8] = 0;
    bVar2 = *DAT_0801461c;
    if (bVar2 < 0x19) {
      puVar10[9] = 1;
    }
    else if (bVar2 < 0x1d) {
      puVar10[9] = 2;
    }
    else if (bVar2 < 0x24) {
      puVar10[9] = 3;
    }
    else if (bVar2 < 0x33) {
      puVar10[9] = 4;
    }
    else if (bVar2 < 0x47) {
      puVar10[9] = 5;
    }
    else if (bVar2 < 0x51) {
      puVar10[9] = 6;
    }
    else if (bVar2 < 0x6a) {
      puVar10[9] = 7;
    }
    else if (bVar2 < 0x88) {
      puVar10[9] = 8;
    }
    else {
      puVar10[9] = 9;
    }
    if (*DAT_08014620 != '\0') {
      puVar10[9] = puVar10[9] | 0x80;
    }
    puVar10[10] = bVar4;
    bVar4 = *DAT_08014624;
    if (0x46 < bVar4 - 0x14) {
      bVar4 = 0x5a;
    }
    puVar10[0xb] = bVar4;
    puVar10[0xc] = *DAT_08014628;
    puVar10[0xd] = *DAT_0801462c;
    uVar9 = 0;
    do {
      iVar6 = iVar7 + uVar9 * 6;
      if (*(byte *)(iVar6 + 3) >> 6 == 3) {
        cVar1 = *(char *)(iVar7 + uVar9 * 6);
        if (cVar1 == '\0') {
          puVar10[uVar9 * 2 + 0xe] = 1;
          puVar10[uVar9 * 2 + 0xf] = *(undefined1 *)(iVar6 + 1);
        }
        else if (cVar1 == '\x01') {
          puVar10[uVar9 * 2 + 0xe] = 0;
          uVar5 = FUN_08008560(*(undefined1 *)(iVar6 + 2));
          puVar10[uVar9 * 2 + 0xf] = uVar5;
          uVar9 = extraout_r2;
          puVar10 = extraout_r3;
        }
      }
      else {
        puVar10[uVar9 * 2 + 0xe] = 6;
        puVar10[uVar9 * 2 + 0xf] = 0;
      }
      uVar9 = uVar9 + 1 & 0xff;
    } while (uVar9 < 5);
    iVar7 = FUN_08013688(DAT_080145f4,0x18);
    if (iVar7 == 0) {
      *DAT_08014630 = 0;
    }
    else {
      *(undefined1 *)(iVar3 + 0xb) = 10;
    }
  }
  return;
}

