// 0x080fc618 FUN_080fc618 size=1750

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_080fc618(void)

{
  char cVar1;
  bool bVar2;
  char *pcVar3;
  byte *pbVar4;
  int iVar5;
  undefined1 *puVar6;
  byte bVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  uint uVar11;
  
  FUN_080fa538();
  pcVar3 = DAT_080fca18;
  if (*DAT_080fca18 == '\0') {
    return;
  }
  *DAT_080fca1c = 0;
  *pcVar3 = '\0';
  FUN_080f8f0c(&DAT_48000000,2);
  FUN_080f8fca(&DAT_48000000,8);
  FUN_080fb258();
  iVar10 = DAT_080fca2c;
  iVar5 = DAT_080fca28;
  iVar8 = DAT_080fca24;
  pbVar4 = DAT_080fca20;
  bVar7 = *DAT_080fca20;
  if (((bVar7 & 3) == 0) || (bVar7 == 0x2f)) {
    FUN_080f8f0c(&DAT_48000000,8);
LAB_080fc7b0:
    iVar9 = FUN_080f8fb6(&DAT_48000000,4);
    if (iVar9 == 0) {
      *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
    }
    FUN_080fd2b4(0xf);
    iVar10 = FUN_080f8fc0(iVar10,1);
    bVar7 = *(byte *)(iVar5 + 2);
    if (iVar10 != 0) {
      bVar7 = ~bVar7;
    }
    *(byte *)(iVar8 + 0xc) = bVar7;
    goto LAB_080fc71c;
  }
  if (bVar7 == 0xf) {
    *DAT_080fca20 = 0;
    FUN_080f8f0c(&DAT_48000000,2);
    FUN_080f8fca(&DAT_48000000,8);
    FUN_080f9084();
    FUN_080fae5c();
    FUN_080f8fca(&DAT_48000000,2);
    FUN_080fd2b4(DAT_080fca30);
    FUN_080f8f0c(&DAT_48000000,2);
    FUN_080fd2b4(0x32);
    iVar8 = FUN_080fada4();
    if (iVar8 != 0) {
      FUN_080fada4();
    }
    FUN_080fd2b4(0x32);
    FUN_080fd2b4(DAT_080fca34);
    *pbVar4 = 0;
    FUN_080fd98c(0xff);
    goto LAB_080fc71c;
  }
  if (bVar7 == 0x1f) {
    FUN_080f8f0c(&DAT_48000000,8);
    FUN_080fd2b4(0xf);
    iVar9 = FUN_080f8fb6(&DAT_48000000,4);
    if (iVar9 == 0) {
      *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
    }
    FUN_080fd2b4(0xf);
    iVar10 = FUN_080f8fc0(iVar10,1);
    bVar7 = *(byte *)(iVar5 + 2);
    if (iVar10 != 0) {
      bVar7 = ~bVar7;
    }
    *(byte *)(iVar8 + 0xc) = bVar7;
    FUN_080f8fca(&DAT_48000000,2);
    do {
      iVar8 = FUN_080f8fb6(&DAT_48000000,0x20);
    } while (iVar8 != 0);
    do {
      iVar8 = FUN_080f8fb6(&DAT_48000000,0x20);
    } while (iVar8 == 0);
    FUN_080f8fca(&DAT_48000000,2);
    *DAT_080fca20 = 0x2f;
    FUN_080fd98c(0x8e);
    return;
  }
  if (bVar7 == 0x3f) {
    FUN_080f8f0c(&DAT_48000000,8);
    FUN_080fd2b4(0xf);
    goto LAB_080fc7b0;
  }
  if (bVar7 == 0x4f) {
    *DAT_080fca20 = 0;
    FUN_080f8fca(&DAT_48000000,8);
    FUN_080fd2b4(0xf);
    iVar9 = FUN_080f8fb6(&DAT_48000000,4);
    if (iVar9 == 0) {
      *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
    }
    FUN_080fd2b4(0xf);
    iVar10 = FUN_080f8fc0(iVar10,1);
    bVar7 = *(byte *)(iVar5 + 2);
    if (iVar10 != 0) {
      bVar7 = ~bVar7;
    }
    *(byte *)(iVar8 + 0xc) = bVar7;
    FUN_080f8fca(&DAT_48000000,2);
    FUN_080fd2b4(DAT_080fca38);
    FUN_080f8f0c(&DAT_48000000,2);
    FUN_080fd2b4(500);
    iVar8 = FUN_080fada4();
    if (iVar8 != 0) {
      FUN_080fada4();
    }
    *DAT_080fca20 = 0;
    FUN_080fd98c(0xff);
    goto LAB_080fc71c;
  }
  FUN_080f8f0c(&DAT_48000000,2);
  FUN_080f8fca(&DAT_48000000,8);
  FUN_080fd2b4(0xf);
  iVar8 = FUN_080f8fb6(&DAT_48000000,4);
  if (iVar8 == 0) {
    *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
  }
  FUN_080f8f0c(&DAT_48000000,2);
  pbVar4 = DAT_080fca3c;
  if ((*DAT_080fca20 & 0xf) == 10) {
    FUN_080f9084();
    FUN_080fae5c();
    _DAT_48000000 = _DAT_48000000 & 0xfffff3ff;
    _DAT_4800000c = _DAT_4800000c & 0xfffff3ff | 0x400;
    *DAT_080fca40 = 0;
    if (*pbVar4 == 9) {
      FUN_080fabc0();
      iVar8 = DAT_080fca48;
    }
    else {
      iVar8 = FUN_080fabc0();
      if (iVar8 != 0) {
        FUN_080fabc0();
      }
      iVar8 = DAT_080fca44;
    }
    FUN_080fd2b4(iVar8);
    FUN_080f9ed4();
    FUN_080f8fca(&DAT_48000000,2);
    *(undefined1 *)(iVar5 + 2) = 0xff;
    bVar7 = *DAT_080fca20;
    if (((bVar7 == 0x1a) || (bVar7 == 0x2a)) || (bVar7 == 0x4a)) {
      FUN_080fa508();
      FUN_080f90be();
    }
    else {
      FUN_080f90fe();
      FUN_080fa4bc();
    }
    bVar2 = (bool)isCurrentModePrivileged();
    if (bVar2) {
      enableIRQinterrupts(0);
    }
  }
  if ((*DAT_080fca20 & 0xf) != 5) {
    if ((*DAT_080fca20 & 0xf) != 9) goto LAB_080fc71c;
    FUN_080fa508();
    *DAT_080fcd34 = 0;
    FUN_080f9084();
    FUN_080fae5c();
    FUN_080f8f0c(&DAT_48000000,2);
    FUN_080f8fca(&DAT_48000000,8);
    FUN_080fd2b4(0xf);
    iVar8 = FUN_080f8fb6(&DAT_48000000,4);
    if (iVar8 == 0) {
      *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
    }
    *DAT_080fcd48 = '\0';
    if (*DAT_080fcd44 == '\t') {
      if ((((char)*pbVar4 < '\0') && (*DAT_080fcd4c != '\0')) &&
         ((DAT_080fcd4c[1] != '\0' && ((DAT_080fcd4c[2] != '\0' && (DAT_080fcd4c[3] != '\0')))))) {
        cVar1 = DAT_080fcd4c[4];
        goto LAB_080fcba8;
      }
LAB_080fcbb2:
      FUN_080fb1e8();
    }
    else {
      if ((*DAT_080fcd44 != '\x19') || (-1 < (char)*pbVar4)) goto LAB_080fcbb2;
      cVar1 = *DAT_080fcd4c;
LAB_080fcba8:
      if (cVar1 == '\0') goto LAB_080fcbb2;
      *DAT_080fcd48 = '\x01';
    }
    FUN_080fd2b4(DAT_080fcd50);
    FUN_080f8fca(&DAT_48000000,2);
    FUN_080fd2b4(0x1e);
    uVar11 = 0;
    do {
      iVar8 = FUN_080f8fb6(&DAT_48000000,0x20);
      if (iVar8 == 0) {
        uVar11 = DAT_080fcd40;
      }
      FUN_080fd2b4(3);
      uVar11 = uVar11 + 1 & 0xffff;
    } while (uVar11 < DAT_080fcd40);
    bVar7 = *pbVar4;
    if ((int)((uint)bVar7 << 0x18) < 0) {
      iVar8 = DAT_080fcd50 + 100;
    }
    else if (bVar7 < 0x18) {
      do {
        iVar8 = FUN_080f8fb6(&DAT_48000000,0x20);
      } while (iVar8 == 0);
      bVar7 = *pbVar4;
      if (bVar7 == 0x17) {
        iVar8 = DAT_080fcd58;
      }
      else if (bVar7 == 0x12) {
        iVar8 = DAT_080fcd58 + -0x96;
      }
      else if (bVar7 == 0x10) {
        iVar8 = DAT_080fcd5c;
      }
      else {
        iVar8 = DAT_080fcd54;
      }
    }
    else if (bVar7 == 0x26) {
      iVar8 = DAT_080fcd60 + 0x50;
    }
    else {
      iVar8 = DAT_080fcd60;
      if (((bVar7 != 0x1b) && (bVar7 != 0x1e)) && (bVar7 != 0x1f)) {
        if ((bVar7 == 0x27) || (bVar7 == 0x28)) {
          iVar8 = DAT_080fcd60 + 100;
        }
        else if (((bVar7 == 0x32) || (bVar7 == 0x36)) || (bVar7 == 0x29)) {
          FUN_080fd310(200);
          FUN_080f8f0c(&DAT_48000000,2);
          FUN_080fd310(0xd2);
          FUN_080f8fca(&DAT_48000000,2);
          iVar8 = DAT_080fcd54 + -0x19;
        }
        else if (((bVar7 != 0x21) && (bVar7 != 0x22)) && (bVar7 < 0x23)) {
          if (((bVar7 == 0x1a) || (bVar7 == 0x1d)) || (bVar7 == 0x20)) {
            iVar8 = DAT_080fcd64 + 200;
          }
          else {
            iVar8 = DAT_080fcd64;
          }
        }
      }
    }
    FUN_080fd310(iVar8);
    FUN_080fd310(0x32);
    pcVar3 = DAT_080fcd44;
    cVar1 = *DAT_080fcd44;
    if ((cVar1 == '\t') || (cVar1 == '\x19')) {
      if (*DAT_080fcd48 == '\0') goto LAB_080fccda;
    }
    else if ((((cVar1 == ')') || (cVar1 == '9')) || (cVar1 == 'I')) || (cVar1 == 'Y')) {
LAB_080fccda:
      FUN_080f8f0c(iVar10,0x80);
      FUN_080fd294(500);
      FUN_080f8fca(iVar10,0x80);
    }
    FUN_080fd2b4(0x8c);
    FUN_080f8f0c(&DAT_48000000,2);
    FUN_080fd2b4(0x208);
    if (*pbVar4 != 0x10) {
      FUN_080fd2b4(0x28);
    }
    *DAT_080fcd3c = 0;
    *(undefined1 *)(iVar5 + 2) = 0xff;
    FUN_080fa31c();
    *pcVar3 = '\0';
    FUN_080fa508();
    goto LAB_080fc71c;
  }
  FUN_080fa508();
  FUN_080f8f0c(&DAT_48000000,2);
  *DAT_080fca4c = 0;
  FUN_080f9084();
  FUN_080fae5c();
  FUN_080fd2b4(0xf);
  iVar8 = FUN_080f8fb6(&DAT_48000000,4);
  if (iVar8 == 0) {
    *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
  }
  FUN_080f8fca(&DAT_48000000,8);
  FUN_080f9190();
  *DAT_080fca50 = 0;
  if (*DAT_080fca20 == 5) {
    if ((((char)*pbVar4 < '\0') && (*DAT_080fca54 != '\0')) &&
       ((DAT_080fca54[1] != '\0' && ((DAT_080fca54[2] != '\0' && (DAT_080fca54[3] != '\0')))))) {
      cVar1 = DAT_080fca54[4];
      goto LAB_080fc986;
    }
LAB_080fc990:
    FUN_080fb1e8();
  }
  else {
    if ((*DAT_080fca20 != 0x15) || (-1 < (char)*pbVar4)) goto LAB_080fc990;
    cVar1 = *DAT_080fca54;
LAB_080fc986:
    if (cVar1 == '\0') goto LAB_080fc990;
    *DAT_080fca50 = 1;
  }
  FUN_080fd2b4(DAT_080fca58);
  FUN_080f8fca(&DAT_48000000,2);
  FUN_080fd2b4(0x32);
  _DAT_48000000 = _DAT_48000000 & 0xfffff3ff;
  _DAT_4800000c = _DAT_4800000c & 0xfffff3ff | 0x400;
  if (*pbVar4 == 9) {
    FUN_080fd310(DAT_080fca48 + 0xd2);
    pbVar4 = DAT_080fca20;
    bVar7 = *DAT_080fca20;
    if ((((bVar7 == 5) || (bVar7 == 0x15)) || (bVar7 == 0x25)) || (bVar7 == 0x35)) {
      FUN_080f8f0c(iVar10,0x80);
      FUN_080fd294(600);
      FUN_080f8fca(iVar10,0x80);
      FUN_080f8f0c(&DAT_48000000,2);
    }
    FUN_080fd2b4(500);
    *DAT_080fcd34 = 1;
    FUN_080f8fca(&DAT_48000000,2);
    FUN_080fd2b4(DAT_080fcd38);
    puVar6 = DAT_080fcd3c;
    *pbVar4 = 0;
    *puVar6 = 0;
    *(undefined1 *)(iVar5 + 2) = 0xff;
  }
  else {
    bVar2 = false;
    uVar11 = 0;
    do {
      FUN_080fd2b4(3);
      iVar8 = FUN_080f8fb6(&DAT_48000000,0x20);
      if (iVar8 == 0) {
        bVar2 = true;
        uVar11 = DAT_080fcd40;
      }
      uVar11 = uVar11 + 1 & 0xffff;
    } while (uVar11 < DAT_080fcd40);
    if (bVar2) {
      cVar1 = *DAT_080fcd44;
      if ((cVar1 == '\x05') || (cVar1 == '\x15')) {
        if (*DAT_080fcd48 == '\0') goto LAB_080fcb02;
      }
      else {
        if ((cVar1 != '%') && (cVar1 != '5')) goto LAB_080fcac6;
LAB_080fcb02:
        FUN_080f8f0c(iVar10,0x80);
        FUN_080fd294(500);
        FUN_080f8fca(iVar10,0x80);
      }
      FUN_080f8f0c(&DAT_48000000,2);
    }
LAB_080fcac6:
    FUN_080f8f0c(&DAT_48000000,2);
    FUN_080fd2b4(0x208);
    *DAT_080fcd34 = 0;
    puVar6 = DAT_080fcd3c;
    *DAT_080fcd44 = '\0';
    *puVar6 = 0;
    *(undefined1 *)(iVar5 + 2) = 0xff;
  }
  FUN_080fa31c();
  FUN_080f8fca(&DAT_48000000,2);
LAB_080fc71c:
  FUN_080f8fca(&DAT_48000000,2);
  return;
}

