// 0x0802007c FUN_0802007c size=502

void FUN_0802007c(void)

{
  short sVar1;
  char *pcVar2;
  char *pcVar3;
  undefined1 *puVar4;
  byte *pbVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined4 uVar8;
  undefined4 *puVar9;
  ushort uVar10;
  undefined4 uVar11;
  
  pcVar2 = DAT_08020274;
  sVar1 = *(short *)(DAT_08020274 + 8);
  if ((sVar1 != 0) && (*(short *)(DAT_08020274 + 8) = sVar1 + -1, pcVar3 = DAT_08020278, sVar1 == 1)
     ) {
    if (*DAT_08020278 == '\0') {
      FUN_08011888();
    }
    FUN_0801a234(0x24);
    disableIRQinterrupts();
    FUN_0800a874();
    FUN_080353a0();
    enableIRQinterrupts();
    FUN_0801bd04(DAT_0802027c,1);
    *pcVar2 = '\0';
    pcVar2[6] = '\x01';
    *DAT_08020280 = 0;
    FUN_080393c4();
    puVar4 = DAT_08020284;
    *DAT_08020284 = 1;
    pcVar2[1] = '\x01';
    *DAT_08020288 = 0;
    FUN_08010b90();
    do {
      FUN_0803ba4c();
      pcVar2[2] = '\0';
      FUN_08036074(1);
      FUN_08036020(0);
      if (((*pcVar2 != '\0') || (pcVar2[3] != '\0')) || (pcVar2[2] != '\0')) break;
    } while (pcVar2[5] == '\0');
    FUN_080171c0();
    pbVar5 = DAT_0802028c;
    if (-1 < (int)((uint)*DAT_0802028c << 0x18)) {
      pcVar2[3] = '\0';
    }
    if (((int)((uint)*pbVar5 << 0x18) < 0) && (pcVar2[3] != '\0')) {
      FUN_08023f88();
      FUN_0801c0fe(0x32);
      FUN_08035fd0();
    }
    puVar6 = DAT_08020290;
    *DAT_08020290 = 0;
    pcVar2[1] = '\0';
    FUN_080393c4();
    pcVar2[0xe] = '\0';
    pcVar2[0xf] = '\0';
    FUN_0801c118(0xf0);
    FUN_080354e4();
    FUN_0800cfe4();
    if (*pcVar3 == '\0') {
      FUN_08011af8();
    }
    uVar8 = DAT_0802029c;
    uVar11 = DAT_08020298;
    puVar7 = DAT_08020294;
    if (pcVar2[2] == '\0' && pcVar2[5] == '\0') {
      *(uint *)(DAT_080202a8 + 0x10) = (*(uint *)(DAT_080202a8 + 0x10) & 0xfffffff8) + 1;
      disableIRQinterrupts();
      *puVar7 = 0;
      *puVar4 = 0;
      FUN_08036c54();
      FUN_080398fa(uVar11,0);
      FUN_080398ba(uVar11,99,0x11f);
      FUN_080398fa(uVar11,1);
      FUN_080398fa(uVar8,0);
      FUN_080398ba(uVar8,9,0x11f);
      FUN_080398fa(uVar8,1);
      enableIRQinterrupts();
      uVar11 = FUN_0803c384(*DAT_080202ac);
      FUN_0803c4a4(uVar11,1);
      uVar10 = FUN_0803c344();
      FUN_0801a234((uint)uVar10 % 0xff + 0x37 & 0xff);
      FUN_080061fc();
      puVar9 = DAT_080202b0;
      *DAT_080202a0 = *DAT_080202b0;
      *DAT_080202b4 = *puVar9;
      *puVar6 = 0;
      *DAT_08020280 = 2000;
      FUN_0803a7e4();
      FUN_08023f88();
    }
    else {
      *puVar4 = 1;
      *DAT_080202a0 = 5;
      disableIRQinterrupts();
      FUN_080393c4();
      *puVar7 = 1;
      *puVar4 = 1;
      FUN_080398fa(uVar11,0);
      FUN_0801bd04(DAT_080202a4,0);
      FUN_080398ba(uVar11,99,0x2f);
      FUN_080398fa(uVar11,1);
      FUN_080398fa(uVar8,0);
      FUN_080398ba(uVar8,9,0x2f);
      FUN_080398fa(uVar8,1);
      enableIRQinterrupts();
    }
    *DAT_080202b8 = 0x14;
    FUN_0801c118(0xf0);
    pcVar2[5] = '\0';
    pcVar2[0xc] = '\0';
    pcVar2[0xd] = '\0';
  }
  return;
}

