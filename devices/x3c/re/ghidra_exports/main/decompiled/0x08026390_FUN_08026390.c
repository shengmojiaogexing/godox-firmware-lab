// 0x08026390 FUN_08026390 size=702

void FUN_08026390(void)

{
  int iVar1;
  byte *pbVar2;
  int iVar3;
  byte *pbVar4;
  byte bVar5;
  char cVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  
  iVar1 = DAT_08026650;
  uVar9 = 0;
  bVar5 = *(byte *)(DAT_08026650 + 4);
  if ((0x20 < bVar5) || (bVar5 == 0)) {
    bVar5 = 0x15;
  }
  *DAT_08026654 = bVar5;
  bVar5 = *(byte *)(iVar1 + 5);
  if (99 < bVar5) {
    bVar5 = 0;
  }
  *DAT_08026658 = bVar5;
  bVar5 = *(byte *)(DAT_08026650 + 6);
  if (*DAT_0802665c == '\x02') {
    if (1 < bVar5) {
      bVar5 = 0;
    }
    *DAT_08026660 = bVar5;
  }
  else {
    if (2 < bVar5) {
      bVar5 = 0;
    }
    *DAT_08026660 = bVar5;
  }
  pbVar2 = DAT_08026664;
  bVar5 = *(byte *)(iVar1 + 7);
  if (1 < bVar5) {
    bVar5 = 0;
  }
  *DAT_08026664 = bVar5;
  bVar5 = *(byte *)(iVar1 + 8);
  if (4 < bVar5) {
    bVar5 = 0;
  }
  *DAT_08026668 = bVar5;
  bVar5 = *(byte *)(iVar1 + 9);
  if (0x50 < bVar5 - 0x14) {
    bVar5 = 100;
  }
  *DAT_0802666c = bVar5;
  *DAT_08026670 = *(undefined1 *)(iVar1 + 10);
  *DAT_08026674 = *(undefined1 *)(iVar1 + 0xc);
  *DAT_08026678 = *(undefined1 *)(iVar1 + 0xd);
  uVar11 = (uint)*(byte *)(iVar1 + 0x7a);
  if (0x3a < uVar11 - 0x3d) {
    uVar11 = 0x50;
  }
  *DAT_0802667c = (byte)uVar11;
  iVar3 = DAT_08026680;
  do {
    iVar7 = iVar1 + uVar9 * 6;
    *(undefined1 *)(iVar3 + uVar9 * 6) = *(undefined1 *)(iVar7 + 0xe);
    if (4 < uVar9) {
      *(undefined1 *)(iVar3 + uVar9 * 6) = 0;
    }
    bVar5 = *(byte *)(iVar7 + 0xf);
    if (uVar11 < bVar5) {
      bVar5 = 0x50;
    }
    iVar10 = iVar3 + uVar9 * 6;
    *(byte *)(iVar10 + 1) = bVar5;
    bVar5 = *(byte *)(iVar7 + 0x10);
    if (0x3c < bVar5) {
      bVar5 = 0x1e;
    }
    *(byte *)(iVar10 + 2) = bVar5;
    *(undefined1 *)(iVar10 + 3) = *(undefined1 *)(iVar7 + 0x11);
    bVar5 = *(byte *)(iVar7 + 0x12);
    if (0x5a < (bVar5 & 0x7f) - 10) {
      bVar5 = 0x8a;
    }
    *(byte *)(iVar10 + 4) = bVar5;
    bVar5 = *(byte *)(iVar7 + 0x13);
    if (1 < bVar5) {
      bVar5 = 0;
    }
    *(byte *)(iVar10 + 5) = bVar5;
    uVar9 = uVar9 + 1 & 0xff;
  } while (uVar9 < 0x10);
  bVar5 = *(byte *)(iVar1 + 0x72);
  if (1 < bVar5) {
    bVar5 = 0;
  }
  *DAT_08026684 = bVar5;
  bVar5 = *(byte *)(iVar1 + 0x73);
  if (1 < bVar5) {
    bVar5 = 0;
  }
  *DAT_08026688 = bVar5;
  *DAT_0802668c = CONCAT11(*(undefined1 *)(iVar1 + 0x74),*(undefined1 *)(iVar1 + 0x75));
  bVar5 = *(byte *)(iVar1 + 0x76);
  if (1 < bVar5) {
    bVar5 = 0;
  }
  *DAT_08026690 = bVar5;
  pbVar4 = DAT_08026694;
  bVar5 = *(byte *)(iVar1 + 0x77);
  if (-1 < (int)((uint)bVar5 << 0x18)) {
    bVar5 = 0;
  }
  *DAT_08026694 = bVar5;
  uVar9 = *(byte *)(iVar1 + 0x78) & 0x7f;
  if (4 < uVar9) {
    uVar9 = 0;
  }
  *DAT_08026698 = (byte)uVar9 | 0x80;
  *DAT_080266a0 = *(undefined2 *)(DAT_0802669c + (*pbVar4 & 0x7f) * 2);
  *DAT_080266a8 = (uint)*(ushort *)(DAT_080266a4 + uVar9 * 2);
  bVar5 = *(byte *)(iVar1 + 0x79);
  if (1 < bVar5) {
    bVar5 = 0;
  }
  *DAT_080266ac = bVar5;
  bVar5 = *(byte *)(iVar1 + 0x7b);
  if (1 < bVar5) {
    bVar5 = 0;
  }
  *DAT_080266b0 = bVar5;
  if ((bVar5 != 0) && (bVar5 == 1)) {
    uVar9 = 0;
    do {
      iVar7 = iVar3 + uVar9 * 6;
      bVar5 = *(byte *)(iVar7 + 1);
      uVar8 = (uint)bVar5;
      if (uVar8 % 10 < 3) {
        *(char *)(iVar7 + 1) = ((char)(uVar8 / 10) + (char)(uVar8 / 10 << 2)) * '\x02';
      }
      else if (uVar8 % 10 < 7) {
        *(byte *)(iVar7 + 1) = (bVar5 / 10) * '\n' + '\x03';
      }
      else {
        *(byte *)(iVar7 + 1) = (bVar5 / 10) * '\n' + '\a';
      }
      uVar9 = uVar9 + 1 & 0xff;
    } while (uVar9 < 0x10);
  }
  bVar5 = *(byte *)(iVar1 + 0x7c);
  if (1 < bVar5) {
    bVar5 = 0;
  }
  *DAT_080266b4 = bVar5;
  bVar5 = *(byte *)(iVar1 + 0x7e);
  if (1 < bVar5) {
    bVar5 = 0;
  }
  *pbVar2 = bVar5;
  bVar5 = *(byte *)(iVar1 + 0xb);
  if ((uVar11 < bVar5) || (bVar5 < 0x14)) {
    *DAT_080266b8 = (byte)uVar11;
  }
  else {
    *DAT_080266b8 = bVar5;
  }
  bVar5 = *(byte *)(iVar1 + 0x7f);
  if (7 < bVar5) {
    bVar5 = 0;
  }
  *DAT_080266bc = bVar5;
  pbVar4 = DAT_080266c0;
  bVar5 = *(byte *)(iVar1 + 0x80);
  *DAT_080266c0 = bVar5;
  *DAT_080266c4 = *(undefined1 *)(iVar1 + 0x81);
  cVar6 = *(char *)(iVar1 + 0x82);
  *DAT_080266c8 = cVar6;
  if (cVar6 == '\x01') {
    *pbVar2 = 0;
  }
  *DAT_080266cc = *(undefined1 *)(iVar1 + 0x83);
  if (0xb0 < bVar5 - 0x18) {
    *pbVar4 = 0x18;
  }
  cVar6 = *(char *)(iVar1 + 0x84);
  if ((cVar6 != -0x80) && (cVar6 != -0x7f)) {
    cVar6 = '\0';
  }
  *DAT_080266d0 = cVar6;
  bVar5 = *(byte *)(iVar1 + 0x85);
  if (1 < bVar5) {
    bVar5 = 0;
  }
  *DAT_080266d4 = bVar5;
  bVar5 = *(byte *)(iVar1 + 0x86);
  if (1 < bVar5) {
    bVar5 = 1;
  }
  *DAT_080266d8 = bVar5;
  bVar5 = *(byte *)(iVar1 + 0x88);
  if (4 < bVar5) {
    bVar5 = 4;
  }
  *DAT_080266dc = bVar5;
  return;
}

