// 0x08007a38 FUN_08007a38 size=448

void FUN_08007a38(void)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  byte *pbVar4;
  char *pcVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 uVar8;
  
  pbVar4 = DAT_08007bfc;
  iVar3 = DAT_08007bf8;
  bVar1 = *(byte *)(DAT_08007bf8 + 1);
  *DAT_08007bfc = bVar1;
  if (2 < bVar1) {
    *pbVar4 = 0;
  }
  pcVar5 = DAT_08007c04;
  pbVar4 = DAT_08007c00;
  bVar1 = *(byte *)(iVar3 + 3);
  *DAT_08007c00 = bVar1 & 3;
  if ((int)((uint)bVar1 * 0x20000000) < 0) {
    if (*pcVar5 != '\0') goto LAB_08007a72;
    *pcVar5 = '\x01';
  }
  else {
    if (*pcVar5 != '\x01') goto LAB_08007a72;
    *pcVar5 = '\0';
  }
  FUN_08007868();
LAB_08007a72:
  *DAT_08007c08 = *(undefined1 *)(iVar3 + 4);
  *DAT_08007c0c = *(undefined1 *)(iVar3 + 5);
  puVar6 = DAT_08007c10;
  cVar2 = *(char *)(iVar3 + 2);
  if (cVar2 == '\0') {
    *pbVar4 = *pbVar4 & 0xe7;
    *puVar6 = 0;
  }
  else if (cVar2 == '\x01') {
    *pbVar4 = (*pbVar4 & 0xe7) + 0x10;
    *puVar6 = 1;
  }
  else if (cVar2 == '\x02') {
    *DAT_08007c10 = 2;
    *pbVar4 = (*pbVar4 & 0xe7) + 8;
  }
  *DAT_08007c14 = *(undefined1 *)(iVar3 + 6);
  *DAT_08007c18 = *(undefined1 *)(iVar3 + 7);
  *DAT_08007c1c = *(undefined1 *)(iVar3 + 8);
  cVar2 = *(char *)(iVar3 + 9);
  *DAT_08007c20 = cVar2;
  if (cVar2 < '\0') {
    *DAT_08007c24 = 0;
  }
  else {
    *DAT_08007c24 = 1;
    *DAT_08007c28 = cVar2;
  }
  *DAT_08007c2c = *(undefined1 *)(iVar3 + 10);
  *DAT_08007c30 = *(undefined1 *)(iVar3 + 0xb);
  *DAT_08007c34 = *(undefined1 *)(iVar3 + 0xc);
  pbVar4 = DAT_08007c38;
  bVar1 = *(byte *)(iVar3 + 0xd);
  *DAT_08007c38 = bVar1;
  if (bVar1 < 200) {
    if (bVar1 == 0) {
      *pbVar4 = 1;
    }
  }
  else {
    *pbVar4 = 199;
  }
  FUN_08003660();
  puVar7 = DAT_08007c40;
  puVar6 = DAT_08007c3c;
  iVar3 = DAT_08007bf8;
  cVar2 = *(char *)(DAT_08007bf8 + 0xe);
  if (cVar2 == '\0') {
    *DAT_08007c40 = 0;
    uVar8 = FUN_08007350(*(undefined1 *)(iVar3 + 0xf));
    *puVar6 = uVar8;
  }
  else if (cVar2 == '\x01') {
    *DAT_08007c40 = 1;
    *DAT_08007c44 = *(undefined1 *)(iVar3 + 0xf);
  }
  else if (cVar2 == '\x06') {
    *DAT_08007c40 = 6;
  }
  iVar3 = DAT_08007bf8;
  cVar2 = *(char *)(DAT_08007bf8 + 0x10);
  if (cVar2 == '\0') {
    puVar7[1] = 0;
    uVar8 = FUN_08007350(*(undefined1 *)(iVar3 + 0x11));
    puVar6[1] = uVar8;
  }
  else if (cVar2 == '\x01') {
    puVar7[1] = 1;
    DAT_08007c44[1] = *(undefined1 *)(iVar3 + 0x11);
  }
  else if (cVar2 == '\x06') {
    puVar7[1] = 6;
  }
  iVar3 = DAT_08007bf8;
  cVar2 = *(char *)(DAT_08007bf8 + 0x12);
  if (cVar2 == '\0') {
    puVar7[2] = 0;
    uVar8 = FUN_08007350(*(undefined1 *)(iVar3 + 0x13));
    puVar6[2] = uVar8;
  }
  else if (cVar2 == '\x01') {
    puVar7[2] = 1;
    DAT_08007c44[2] = *(undefined1 *)(iVar3 + 0x13);
  }
  else if (cVar2 == '\x06') {
    puVar7[2] = 6;
  }
  iVar3 = DAT_08007bf8;
  cVar2 = *(char *)(DAT_08007bf8 + 0x14);
  if (cVar2 == '\0') {
    puVar7[3] = 0;
    uVar8 = FUN_08007350(*(undefined1 *)(iVar3 + 0x15));
    puVar6[3] = uVar8;
  }
  else if (cVar2 == '\x01') {
    puVar7[3] = 1;
    DAT_08007c44[3] = *(undefined1 *)(iVar3 + 0x15);
  }
  else if (cVar2 == '\x06') {
    puVar7[3] = 6;
  }
  iVar3 = DAT_08007bf8;
  cVar2 = *(char *)(DAT_08007bf8 + 0x16);
  if (cVar2 == '\0') {
    puVar7[4] = 0;
    uVar8 = FUN_08007350(*(undefined1 *)(iVar3 + 0x17));
    puVar6[4] = uVar8;
    return;
  }
  if (cVar2 == '\x01') {
    puVar7[4] = 1;
    DAT_08007c44[4] = *(undefined1 *)(iVar3 + 0x17);
    return;
  }
  if (cVar2 == '\x06') {
    puVar7[4] = 6;
  }
  return;
}

