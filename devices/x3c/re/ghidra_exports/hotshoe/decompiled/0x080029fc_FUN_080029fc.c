// 0x080029fc FUN_080029fc size=586

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_080029fc(void)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  undefined4 uVar4;
  undefined1 *puVar5;
  
  iVar3 = FUN_08002cfc(0x20);
  if (iVar3 != 1) goto LAB_08002a2c;
  FUN_08002c84(0x20);
  FUN_080030fe();
  iVar3 = FUN_08002fb6(&DAT_48000000,0x20);
  if (iVar3 != 0) {
    *DAT_08002c48 = 0;
    FUN_0800431c();
    goto LAB_08002a2c;
  }
  cVar1 = *DAT_08002c4c;
  if (cVar1 == '\x1a') {
    if (*DAT_08002c50 == '\x01') {
      if (*DAT_08002c54 == '%') {
        uVar4 = DAT_08002c5c;
      }
      else {
        uVar4 = DAT_08002c58;
      }
      goto LAB_08002a72;
    }
  }
  else if (cVar1 != '*') {
    if (cVar1 != 'J') goto LAB_08002a2c;
    uVar4 = DAT_08002c60;
LAB_08002a72:
    FUN_08007310(uVar4);
  }
  FUN_08003488();
  FUN_080044bc();
LAB_08002a2c:
  iVar3 = FUN_08002cfc(0x200);
  if (iVar3 == 1) {
    FUN_08002c84(0x200);
    *DAT_08002c64 = 5;
    if (*DAT_08002c68 == '\x01') {
      FUN_080072b4(0x1e);
      _DAT_48000000 = _DAT_48000000 & 0xfff3ffff;
      _DAT_4800000c = _DAT_4800000c & 0xfff3ffff | 0x40000;
      iVar3 = FUN_08002fb6(&DAT_48000000,0x200);
      if (iVar3 == 0) {
        FUN_08002fca(&DAT_48000000,1);
        FUN_080026ee();
        _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
        FUN_08002f0c(&DAT_48000000,0x8000);
        FUN_080072f2(0x32);
        FUN_08002fca(&DAT_48000000,0x8000);
        _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
        _DAT_4800000c = _DAT_4800000c | 0x40000000;
        puVar5 = DAT_08002c70;
      }
      else {
        puVar5 = DAT_08002c6c;
      }
      *puVar5 = 1;
    }
    else if (*DAT_08002c68 == '\0') {
      FUN_080072b4(0x1e);
      _DAT_48000000 = _DAT_48000000 & 0xfff3ffff;
      _DAT_4800000c = _DAT_4800000c & 0xfff3ffff | 0x40000;
      iVar3 = FUN_08002fb6(&DAT_48000000,0x200);
      if (iVar3 == 0) {
        FUN_08002fca(&DAT_48000000,1);
        if (*DAT_08002c74 == '\x01') {
          FUN_080026ee();
          _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
          FUN_08002f0c(&DAT_48000000,0x8000);
          FUN_080072f2(0x14);
          FUN_08002fca(&DAT_48000000,0x8000);
          _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
          _DAT_4800000c = _DAT_4800000c | 0x40000000;
        }
        FUN_08004538();
        *DAT_08002c78 = 0;
        *DAT_08002c48 = 0;
        FUN_08004044();
        if (*DAT_08002c7c == '\0') {
          FUN_0800431c();
        }
      }
    }
  }
  iVar3 = FUN_08002cfc(0x8000);
  if (iVar3 != 1) {
    return;
  }
  FUN_08002c84(0x8000);
  FUN_08004538();
  if (*DAT_08002c68 == '\0') {
    _DAT_48000000 = _DAT_48000000 & 0xfff3ffff;
    _DAT_4800000c = _DAT_4800000c & 0xfff3ffff | 0x40000;
    iVar3 = FUN_08002fb6(&DAT_48000000,0x200);
    pcVar2 = DAT_08002c80;
    if (iVar3 == 0) {
      FUN_08002f0c(&DAT_48000000,2);
      FUN_08003084();
      if (*pcVar2 == '\0') {
        *pcVar2 = '\x01';
        FUN_08004a58();
        *pcVar2 = '\0';
      }
      FUN_0800431c();
      return;
    }
    if (*DAT_08002c80 == '\0') {
      *DAT_08002c80 = '\x01';
      FUN_08004a58();
      *pcVar2 = '\0';
      return;
    }
  }
  else {
    FUN_080072b4(0x1e);
    iVar3 = FUN_08002fb6(&DAT_48000000,0x8000);
    if (iVar3 != 0) {
      *DAT_08002c6c = 1;
      return;
    }
    FUN_08002fca(&DAT_48000000,1);
    *DAT_08002c70 = 2;
  }
  return;
}

