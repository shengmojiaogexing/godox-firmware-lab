// 0x08006618 FUN_08006618 size=1750

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08006618(void)

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
  
  FUN_08004538();
  pcVar3 = DAT_08006a18;
  if (*DAT_08006a18 == '\0') {
    return;
  }
  *DAT_08006a1c = 0;
  *pcVar3 = '\0';
  FUN_08002f0c(&DAT_48000000,2);
  FUN_08002fca(&DAT_48000000,8);
  FUN_08005258();
  iVar10 = DAT_08006a2c;
  iVar5 = DAT_08006a28;
  iVar8 = DAT_08006a24;
  pbVar4 = DAT_08006a20;
  bVar7 = *DAT_08006a20;
  if (((bVar7 & 3) == 0) || (bVar7 == 0x2f)) {
    FUN_08002f0c(&DAT_48000000,8);
LAB_080067b0:
    iVar9 = FUN_08002fb6(&DAT_48000000,4);
    if (iVar9 == 0) {
      *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
    }
    FUN_080072b4(0xf);
    iVar10 = FUN_08002fc0(iVar10,1);
    bVar7 = *(byte *)(iVar5 + 2);
    if (iVar10 != 0) {
      bVar7 = ~bVar7;
    }
    *(byte *)(iVar8 + 0xc) = bVar7;
    goto LAB_0800671c;
  }
  if (bVar7 == 0xf) {
    *DAT_08006a20 = 0;
    FUN_08002f0c(&DAT_48000000,2);
    FUN_08002fca(&DAT_48000000,8);
    FUN_08003084();
    FUN_08004e5c();
    FUN_08002fca(&DAT_48000000,2);
    FUN_080072b4(DAT_08006a30);
    FUN_08002f0c(&DAT_48000000,2);
    FUN_080072b4(0x32);
    iVar8 = FUN_08004da4();
    if (iVar8 != 0) {
      FUN_08004da4();
    }
    FUN_080072b4(0x32);
    FUN_080072b4(DAT_08006a34);
    *pbVar4 = 0;
    FUN_0800798c(0xff);
    goto LAB_0800671c;
  }
  if (bVar7 == 0x1f) {
    FUN_08002f0c(&DAT_48000000,8);
    FUN_080072b4(0xf);
    iVar9 = FUN_08002fb6(&DAT_48000000,4);
    if (iVar9 == 0) {
      *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
    }
    FUN_080072b4(0xf);
    iVar10 = FUN_08002fc0(iVar10,1);
    bVar7 = *(byte *)(iVar5 + 2);
    if (iVar10 != 0) {
      bVar7 = ~bVar7;
    }
    *(byte *)(iVar8 + 0xc) = bVar7;
    FUN_08002fca(&DAT_48000000,2);
    do {
      iVar8 = FUN_08002fb6(&DAT_48000000,0x20);
    } while (iVar8 != 0);
    do {
      iVar8 = FUN_08002fb6(&DAT_48000000,0x20);
    } while (iVar8 == 0);
    FUN_08002fca(&DAT_48000000,2);
    *DAT_08006a20 = 0x2f;
    FUN_0800798c(0x8e);
    return;
  }
  if (bVar7 == 0x3f) {
    FUN_08002f0c(&DAT_48000000,8);
    FUN_080072b4(0xf);
    goto LAB_080067b0;
  }
  if (bVar7 == 0x4f) {
    *DAT_08006a20 = 0;
    FUN_08002fca(&DAT_48000000,8);
    FUN_080072b4(0xf);
    iVar9 = FUN_08002fb6(&DAT_48000000,4);
    if (iVar9 == 0) {
      *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
    }
    FUN_080072b4(0xf);
    iVar10 = FUN_08002fc0(iVar10,1);
    bVar7 = *(byte *)(iVar5 + 2);
    if (iVar10 != 0) {
      bVar7 = ~bVar7;
    }
    *(byte *)(iVar8 + 0xc) = bVar7;
    FUN_08002fca(&DAT_48000000,2);
    FUN_080072b4(DAT_08006a38);
    FUN_08002f0c(&DAT_48000000,2);
    FUN_080072b4(500);
    iVar8 = FUN_08004da4();
    if (iVar8 != 0) {
      FUN_08004da4();
    }
    *DAT_08006a20 = 0;
    FUN_0800798c(0xff);
    goto LAB_0800671c;
  }
  FUN_08002f0c(&DAT_48000000,2);
  FUN_08002fca(&DAT_48000000,8);
  FUN_080072b4(0xf);
  iVar8 = FUN_08002fb6(&DAT_48000000,4);
  if (iVar8 == 0) {
    *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
  }
  FUN_08002f0c(&DAT_48000000,2);
  pbVar4 = DAT_08006a3c;
  if ((*DAT_08006a20 & 0xf) == 10) {
    FUN_08003084();
    FUN_08004e5c();
    _DAT_48000000 = _DAT_48000000 & 0xfffff3ff;
    _DAT_4800000c = _DAT_4800000c & 0xfffff3ff | 0x400;
    *DAT_08006a40 = 0;
    if (*pbVar4 == 9) {
      FUN_08004bc0();
      iVar8 = DAT_08006a48;
    }
    else {
      iVar8 = FUN_08004bc0();
      if (iVar8 != 0) {
        FUN_08004bc0();
      }
      iVar8 = DAT_08006a44;
    }
    FUN_080072b4(iVar8);
    FUN_08003ed4();
    FUN_08002fca(&DAT_48000000,2);
    *(undefined1 *)(iVar5 + 2) = 0xff;
    bVar7 = *DAT_08006a20;
    if (((bVar7 == 0x1a) || (bVar7 == 0x2a)) || (bVar7 == 0x4a)) {
      FUN_08004508();
      FUN_080030be();
    }
    else {
      FUN_080030fe();
      FUN_080044bc();
    }
    bVar2 = (bool)isCurrentModePrivileged();
    if (bVar2) {
      enableIRQinterrupts(0);
    }
  }
  if ((*DAT_08006a20 & 0xf) != 5) {
    if ((*DAT_08006a20 & 0xf) != 9) goto LAB_0800671c;
    FUN_08004508();
    *DAT_08006d34 = 0;
    FUN_08003084();
    FUN_08004e5c();
    FUN_08002f0c(&DAT_48000000,2);
    FUN_08002fca(&DAT_48000000,8);
    FUN_080072b4(0xf);
    iVar8 = FUN_08002fb6(&DAT_48000000,4);
    if (iVar8 == 0) {
      *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
    }
    *DAT_08006d48 = '\0';
    if (*DAT_08006d44 == '\t') {
      if ((((char)*pbVar4 < '\0') && (*DAT_08006d4c != '\0')) &&
         ((DAT_08006d4c[1] != '\0' && ((DAT_08006d4c[2] != '\0' && (DAT_08006d4c[3] != '\0')))))) {
        cVar1 = DAT_08006d4c[4];
        goto LAB_08006ba8;
      }
LAB_08006bb2:
      FUN_080051e8();
    }
    else {
      if ((*DAT_08006d44 != '\x19') || (-1 < (char)*pbVar4)) goto LAB_08006bb2;
      cVar1 = *DAT_08006d4c;
LAB_08006ba8:
      if (cVar1 == '\0') goto LAB_08006bb2;
      *DAT_08006d48 = '\x01';
    }
    FUN_080072b4(DAT_08006d50);
    FUN_08002fca(&DAT_48000000,2);
    FUN_080072b4(0x1e);
    uVar11 = 0;
    do {
      iVar8 = FUN_08002fb6(&DAT_48000000,0x20);
      if (iVar8 == 0) {
        uVar11 = DAT_08006d40;
      }
      FUN_080072b4(3);
      uVar11 = uVar11 + 1 & 0xffff;
    } while (uVar11 < DAT_08006d40);
    bVar7 = *pbVar4;
    if ((int)((uint)bVar7 << 0x18) < 0) {
      iVar8 = DAT_08006d50 + 100;
    }
    else if (bVar7 < 0x18) {
      do {
        iVar8 = FUN_08002fb6(&DAT_48000000,0x20);
      } while (iVar8 == 0);
      bVar7 = *pbVar4;
      if (bVar7 == 0x17) {
        iVar8 = DAT_08006d58;
      }
      else if (bVar7 == 0x12) {
        iVar8 = DAT_08006d58 + -0x96;
      }
      else if (bVar7 == 0x10) {
        iVar8 = DAT_08006d5c;
      }
      else {
        iVar8 = DAT_08006d54;
      }
    }
    else if (bVar7 == 0x26) {
      iVar8 = DAT_08006d60 + 0x50;
    }
    else {
      iVar8 = DAT_08006d60;
      if (((bVar7 != 0x1b) && (bVar7 != 0x1e)) && (bVar7 != 0x1f)) {
        if ((bVar7 == 0x27) || (bVar7 == 0x28)) {
          iVar8 = DAT_08006d60 + 100;
        }
        else if (((bVar7 == 0x32) || (bVar7 == 0x36)) || (bVar7 == 0x29)) {
          FUN_08007310(200);
          FUN_08002f0c(&DAT_48000000,2);
          FUN_08007310(0xd2);
          FUN_08002fca(&DAT_48000000,2);
          iVar8 = DAT_08006d54 + -0x19;
        }
        else if (((bVar7 != 0x21) && (bVar7 != 0x22)) && (bVar7 < 0x23)) {
          if (((bVar7 == 0x1a) || (bVar7 == 0x1d)) || (bVar7 == 0x20)) {
            iVar8 = DAT_08006d64 + 200;
          }
          else {
            iVar8 = DAT_08006d64;
          }
        }
      }
    }
    FUN_08007310(iVar8);
    FUN_08007310(0x32);
    pcVar3 = DAT_08006d44;
    cVar1 = *DAT_08006d44;
    if ((cVar1 == '\t') || (cVar1 == '\x19')) {
      if (*DAT_08006d48 == '\0') goto LAB_08006cda;
    }
    else if ((((cVar1 == ')') || (cVar1 == '9')) || (cVar1 == 'I')) || (cVar1 == 'Y')) {
LAB_08006cda:
      FUN_08002f0c(iVar10,0x80);
      FUN_08007294(500);
      FUN_08002fca(iVar10,0x80);
    }
    FUN_080072b4(0x8c);
    FUN_08002f0c(&DAT_48000000,2);
    FUN_080072b4(0x208);
    if (*pbVar4 != 0x10) {
      FUN_080072b4(0x28);
    }
    *DAT_08006d3c = 0;
    *(undefined1 *)(iVar5 + 2) = 0xff;
    FUN_0800431c();
    *pcVar3 = '\0';
    FUN_08004508();
    goto LAB_0800671c;
  }
  FUN_08004508();
  FUN_08002f0c(&DAT_48000000,2);
  *DAT_08006a4c = 0;
  FUN_08003084();
  FUN_08004e5c();
  FUN_080072b4(0xf);
  iVar8 = FUN_08002fb6(&DAT_48000000,4);
  if (iVar8 == 0) {
    *(uint *)(iVar10 + 0x14) = *(uint *)(iVar10 + 0x14) ^ 1;
  }
  FUN_08002fca(&DAT_48000000,8);
  FUN_08003190();
  *DAT_08006a50 = 0;
  if (*DAT_08006a20 == 5) {
    if ((((char)*pbVar4 < '\0') && (*DAT_08006a54 != '\0')) &&
       ((DAT_08006a54[1] != '\0' && ((DAT_08006a54[2] != '\0' && (DAT_08006a54[3] != '\0')))))) {
      cVar1 = DAT_08006a54[4];
      goto LAB_08006986;
    }
LAB_08006990:
    FUN_080051e8();
  }
  else {
    if ((*DAT_08006a20 != 0x15) || (-1 < (char)*pbVar4)) goto LAB_08006990;
    cVar1 = *DAT_08006a54;
LAB_08006986:
    if (cVar1 == '\0') goto LAB_08006990;
    *DAT_08006a50 = 1;
  }
  FUN_080072b4(DAT_08006a58);
  FUN_08002fca(&DAT_48000000,2);
  FUN_080072b4(0x32);
  _DAT_48000000 = _DAT_48000000 & 0xfffff3ff;
  _DAT_4800000c = _DAT_4800000c & 0xfffff3ff | 0x400;
  if (*pbVar4 == 9) {
    FUN_08007310(DAT_08006a48 + 0xd2);
    pbVar4 = DAT_08006a20;
    bVar7 = *DAT_08006a20;
    if ((((bVar7 == 5) || (bVar7 == 0x15)) || (bVar7 == 0x25)) || (bVar7 == 0x35)) {
      FUN_08002f0c(iVar10,0x80);
      FUN_08007294(600);
      FUN_08002fca(iVar10,0x80);
      FUN_08002f0c(&DAT_48000000,2);
    }
    FUN_080072b4(500);
    *DAT_08006d34 = 1;
    FUN_08002fca(&DAT_48000000,2);
    FUN_080072b4(DAT_08006d38);
    puVar6 = DAT_08006d3c;
    *pbVar4 = 0;
    *puVar6 = 0;
    *(undefined1 *)(iVar5 + 2) = 0xff;
  }
  else {
    bVar2 = false;
    uVar11 = 0;
    do {
      FUN_080072b4(3);
      iVar8 = FUN_08002fb6(&DAT_48000000,0x20);
      if (iVar8 == 0) {
        bVar2 = true;
        uVar11 = DAT_08006d40;
      }
      uVar11 = uVar11 + 1 & 0xffff;
    } while (uVar11 < DAT_08006d40);
    if (bVar2) {
      cVar1 = *DAT_08006d44;
      if ((cVar1 == '\x05') || (cVar1 == '\x15')) {
        if (*DAT_08006d48 == '\0') goto LAB_08006b02;
      }
      else {
        if ((cVar1 != '%') && (cVar1 != '5')) goto LAB_08006ac6;
LAB_08006b02:
        FUN_08002f0c(iVar10,0x80);
        FUN_08007294(500);
        FUN_08002fca(iVar10,0x80);
      }
      FUN_08002f0c(&DAT_48000000,2);
    }
LAB_08006ac6:
    FUN_08002f0c(&DAT_48000000,2);
    FUN_080072b4(0x208);
    *DAT_08006d34 = 0;
    puVar6 = DAT_08006d3c;
    *DAT_08006d44 = '\0';
    *puVar6 = 0;
    *(undefined1 *)(iVar5 + 2) = 0xff;
  }
  FUN_0800431c();
  FUN_08002fca(&DAT_48000000,2);
LAB_0800671c:
  FUN_08002fca(&DAT_48000000,2);
  return;
}

