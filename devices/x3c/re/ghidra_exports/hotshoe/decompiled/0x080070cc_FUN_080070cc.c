// 0x080070cc FUN_080070cc size=372

undefined4 FUN_080070cc(void)

{
  char cVar1;
  undefined1 *puVar2;
  char *pcVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  byte bVar6;
  char cVar7;
  undefined1 uVar8;
  byte bVar9;
  uint uVar10;
  
  puVar2 = DAT_08007240;
  *DAT_08007240 = 0xb0;
  pcVar3 = DAT_08007244;
  puVar2[1] = 0;
  if (*pcVar3 == '\x01') {
    puVar2[1] = 1;
  }
  pcVar3 = DAT_0800724c;
  uVar10 = (uint)*DAT_08007248;
  bVar6 = (byte)(uVar10 & 3);
  puVar2[2] = bVar6;
  cVar7 = *pcVar3;
  if (cVar7 == '\x01') {
    puVar2[2] = bVar6 | 4;
  }
  if ((int)(uVar10 << 0x1b) < 0) {
    if ((uVar10 & 3) != 2) {
      bVar6 = puVar2[2];
      bVar9 = 0x10;
      goto LAB_0800711c;
    }
LAB_0800710c:
    bVar6 = puVar2[2] & 0xf;
  }
  else {
    if (-1 < (int)(uVar10 << 0x1c)) goto LAB_0800710c;
    bVar6 = puVar2[2];
    bVar9 = 0x20;
LAB_0800711c:
    bVar6 = bVar6 | bVar9;
  }
  puVar2[2] = bVar6;
  puVar2[3] = *DAT_08007250;
  puVar2[4] = *DAT_08007254;
  if (*DAT_08007258 == '\0') {
    bVar6 = *DAT_08007268 | 0x80;
  }
  else {
    bVar6 = *DAT_0800725c;
  }
  puVar2[5] = bVar6;
  puVar2[6] = *DAT_08007260;
  puVar2[7] = 1;
  cVar1 = *DAT_08007264;
  if (cVar7 == '\x01') {
    puVar2[8] = 0;
  }
  else if (cVar7 == '\0') {
    puVar2[8] = cVar1;
  }
  puVar4 = DAT_0800726c;
  if (cVar1 == '\0') {
    *DAT_0800726c = 5;
  }
  puVar2[9] = *puVar4;
  puVar2[10] = *DAT_08007270;
  puVar2[0xb] = *DAT_08007274;
  puVar2[0xc] = *DAT_08007278;
  cVar7 = *DAT_0800727c;
  if (cVar7 == '\0') {
    cVar7 = '\x01';
  }
  puVar2[0xd] = cVar7;
  puVar4 = DAT_08007288;
  puVar2[0x18] = *DAT_08007280;
  puVar5 = DAT_0800728c;
  cVar7 = *DAT_08007284;
  puVar2[0xe] = cVar7;
  if (cVar7 == '\0') {
    uVar8 = switchD_08007376::caseD_f5(*puVar4);
LAB_080071b2:
    puVar2[0xf] = uVar8;
  }
  else if (cVar7 == '\x01') {
    uVar8 = *puVar5;
    goto LAB_080071b2;
  }
  cVar7 = DAT_08007284[1];
  puVar2[0x10] = cVar7;
  if (cVar7 == '\0') {
    uVar8 = switchD_08007376::caseD_f5(puVar4[1]);
LAB_080071ce:
    puVar2[0x11] = uVar8;
  }
  else if (cVar7 == '\x01') {
    uVar8 = puVar5[1];
    goto LAB_080071ce;
  }
  cVar7 = DAT_08007284[2];
  puVar2[0x12] = cVar7;
  if (cVar7 == '\0') {
    uVar8 = switchD_08007376::caseD_f5(puVar4[2]);
LAB_080071ea:
    puVar2[0x13] = uVar8;
  }
  else if (cVar7 == '\x01') {
    uVar8 = puVar5[2];
    goto LAB_080071ea;
  }
  cVar7 = DAT_08007284[3];
  puVar2[0x14] = cVar7;
  if (cVar7 == '\0') {
    uVar8 = switchD_08007376::caseD_f5(puVar4[3]);
LAB_08007206:
    puVar2[0x15] = uVar8;
  }
  else if (cVar7 == '\x01') {
    uVar8 = puVar5[3];
    goto LAB_08007206;
  }
  cVar7 = DAT_08007284[4];
  puVar2[0x16] = cVar7;
  if (cVar7 == '\0') {
    uVar8 = switchD_08007376::caseD_f5(puVar4[4]);
  }
  else {
    if (cVar7 != '\x01') goto LAB_08007224;
    uVar8 = puVar5[4];
  }
  puVar2[0x17] = uVar8;
LAB_08007224:
  uVar10 = 0;
  do {
    if (*(char *)(DAT_08007290 + uVar10) != puVar2[uVar10]) {
      return 1;
    }
    uVar10 = uVar10 + 1 & 0xff;
  } while (uVar10 < 0x19);
  return 0;
}

