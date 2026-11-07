// 0x08002878 FUN_08002878 size=356

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_08002878(void)

{
  bool bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  
  iVar5 = FUN_08002cfc(4);
  if (iVar5 == 1) {
    FUN_08002c84(4);
    if (*DAT_080029dc == '\0') {
      FUN_08004538();
      FUN_08003190();
      uVar4 = DAT_080029f0;
      uVar3 = DAT_080029ec;
      uVar2 = DAT_080029e8;
      if (*DAT_080029e4 == '\0') {
        if (*DAT_080029f4 == '\x01') {
          _DAT_48000000 = _DAT_48000000 & 0xfffff3ff;
          _DAT_4800000c = _DAT_4800000c & 0xfffff3ff | 0x400;
          iVar5 = FUN_08002fb6(&DAT_48000000,0x20);
          if (iVar5 == 0) {
            FUN_080030fe();
            FUN_080072d4(0x14);
            iVar5 = FUN_08002fb6(uVar4,4);
            if (iVar5 == 0) {
              if (*DAT_080029f8 == '\n') {
                FUN_08003488();
              }
              else {
                FUN_08002fca(&DAT_48000000,2);
              }
            }
          }
        }
        else {
          FUN_080072d4(0x1e);
          iVar5 = FUN_08002fb6(uVar4,4);
          if (iVar5 == 0) {
            FUN_08002f0c(uVar4,0x80);
            FUN_08007294(uVar2);
            FUN_08002fca(uVar4,0x80);
            FUN_08007294(uVar3);
            iVar5 = FUN_080047c0();
            if (iVar5 != 0) {
              FUN_080047c0();
            }
            FUN_080030fe();
          }
        }
      }
      else {
        FUN_080072d4(0x1e);
        iVar5 = FUN_08002fb6(uVar4,4);
        if (iVar5 == 0) {
          FUN_08002f0c(uVar4,0x80);
          FUN_08007294(uVar2);
          FUN_08002fca(uVar4,0x80);
          FUN_08007294(uVar3);
          iVar5 = FUN_080047c0();
          if (iVar5 != 0) {
            FUN_080047c0();
          }
          FUN_080030fe();
        }
        FUN_080044bc();
      }
      bVar1 = (bool)isCurrentModePrivileged();
      if (bVar1) {
        enableIRQinterrupts(0);
      }
      return 0;
    }
    FUN_080026ee();
    _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
    FUN_08002f0c(&DAT_48000000,0x8000);
    FUN_080072f2(0x32);
    FUN_08002fca(&DAT_48000000,0x8000);
    _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
    _DAT_4800000c = _DAT_4800000c | 0x40000000;
    iVar5 = 2;
    *DAT_080029e0 = 2;
  }
  return iVar5;
}

