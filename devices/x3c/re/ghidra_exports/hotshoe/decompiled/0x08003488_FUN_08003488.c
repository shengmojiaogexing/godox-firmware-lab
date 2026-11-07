// 0x08003488 FUN_08003488 size=422

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08003488(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  char *pcVar2;
  undefined1 *puVar3;
  char *pcVar4;
  byte *pbVar5;
  short *psVar6;
  short sVar7;
  int iVar8;
  uint uVar9;
  undefined4 uVar10;
  
  uVar10 = DAT_08003634;
  pcVar2 = DAT_08003630;
  cVar1 = *DAT_08003630;
  if (((cVar1 != '\n') && (cVar1 != '*')) && (cVar1 != 'J')) {
    if (cVar1 == '\x1a') {
      FUN_08002f0c(&DAT_48000000,2,param_3,param_4,param_4);
      FUN_08004508();
      FUN_08002f0c(uVar10,0x80);
      FUN_080072b4(DAT_08003638);
      FUN_08002fca(uVar10,0x80);
      *DAT_0800363c = 0;
      FUN_08003190();
      do {
        iVar8 = FUN_08002fb6(&DAT_48000000,0x20);
      } while (iVar8 == 0);
      iVar8 = FUN_080047c0();
      if (iVar8 != 0) {
        FUN_080047c0();
      }
      _DAT_48000000 = _DAT_48000000 & 0xfffff3ff;
      _DAT_4800000c = _DAT_4800000c & 0xfffff3ff | 0x400;
      FUN_08003190();
      FUN_08002f0c(&DAT_48000000,2);
      *pcVar2 = '\0';
      FUN_08002ebc();
      *DAT_08003640 = 0;
      *DAT_08003644 = 0;
      FUN_0800431c();
      FUN_08002fca(&DAT_48000000,2);
      FUN_080044bc();
    }
    return;
  }
  FUN_08002f0c(&DAT_48000000,2,param_3,param_4,param_4);
  pbVar5 = DAT_0800364c;
  pcVar4 = DAT_08003648;
  *DAT_08003648 = '\0';
  if ((*pbVar5 & 3) == 2) {
    FUN_08002f0c(uVar10,0x80);
    if (*DAT_08003654 != 0xc) {
      uVar10 = switchD_0800735a::caseD_ed();
      sVar7 = FUN_080020e8(uVar10,10);
      *DAT_08003658 = sVar7 + 2;
      if ((short)(sVar7 + 2) == 0) {
        *pcVar4 = '\0';
      }
      else {
        *pcVar4 = '\x02';
      }
      goto LAB_080035e6;
    }
    *pcVar4 = '\x03';
  }
  else {
    if ((*DAT_08003650 == '\x02') && (*DAT_08003654 - 0x10 < 0x51)) {
      uVar9 = FUN_08002fd0();
      psVar6 = DAT_08003658;
      *DAT_08003658 = (short)uVar9;
      if (10 < uVar9) {
        *pcVar4 = '\x01';
        sVar7 = FUN_080020e8(uVar9,10);
        *psVar6 = sVar7;
        goto LAB_080035d8;
      }
      *pcVar4 = '\0';
      FUN_08007330(uVar9);
    }
    else {
      *pcVar4 = '\0';
    }
    FUN_08002f0c(uVar10,0x80);
    FUN_080072b4(DAT_08003638);
    FUN_08002fca(uVar10,0x80);
    if ((*pcVar4 == '\x02') || (*pcVar4 == '\0')) {
LAB_080035e6:
      FUN_080072b4(1000);
      if ((*pcVar4 == '\0') && (iVar8 = FUN_080047c0(), iVar8 != 0)) {
        FUN_080047c0();
      }
      goto LAB_0800360e;
    }
  }
LAB_080035d8:
  if (*pcVar2 == '\n') {
    uVar10 = DAT_0800365c;
  }
  else {
    if (*pcVar2 != '*') goto LAB_0800360e;
    uVar10 = 8000;
  }
  FUN_080072b4(uVar10);
LAB_0800360e:
  FUN_08002ebc();
  puVar3 = DAT_08003640;
  *pcVar2 = '\0';
  *puVar3 = 0;
  *DAT_08003644 = 0xff;
  FUN_0800431c();
  FUN_08002fca(&DAT_48000000,2);
  return;
}

