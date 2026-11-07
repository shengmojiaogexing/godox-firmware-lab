// 0x0800455c FUN_0800455c size=410

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0800455c(void)

{
  bool bVar1;
  char *pcVar2;
  undefined1 *puVar3;
  undefined4 uVar4;
  char *pcVar5;
  int iVar6;
  
  FUN_08004740();
  puVar3 = DAT_080046fc;
  pcVar2 = DAT_080046f8;
  if (*DAT_080046f8 != '\0') {
    bVar1 = (bool)isCurrentModePrivileged();
    if (bVar1) {
      enableIRQinterrupts(1);
    }
    *DAT_080046fc = 0;
    FUN_08003190(1);
    *(undefined1 *)(DAT_08004700 + 7) = 0;
    *DAT_08004704 = 0;
    FUN_08003980(0x400000);
    *(uint *)(DAT_08004708 + 8) = *(uint *)(DAT_08004708 + 8) | DAT_08004708 >> 0x12;
    FUN_08003190();
    FUN_08002d18();
    FUN_08004508();
    if (*pcVar2 == '\x02') {
      FUN_0800446c();
      FUN_080026ac();
    }
    else if (*pcVar2 == '\x01') {
      FUN_080026ac();
      if (*DAT_0800470c == '\x01') {
        FUN_0800446c();
        FUN_080044bc();
      }
      else {
        FUN_0800441c();
      }
    }
    FUN_08003190();
    uVar4 = DAT_08004710;
    FUN_08007294(DAT_08004710);
    FUN_08007294(uVar4);
    FUN_08007294(uVar4);
    FUN_08007294(uVar4);
    FUN_08007294(uVar4);
    FUN_08003190();
    FUN_08002c84(0x200);
    FUN_08002c84(0x8000);
    *DAT_08004714 = 0;
    FUN_080030a4();
    FUN_08003190();
    pcVar5 = DAT_08004718;
    while( true ) {
      *pcVar5 = '\0';
      FUN_08003a20();
      bVar1 = (bool)isCurrentModePrivileged();
      if (bVar1) {
        enableIRQinterrupts(0);
      }
      FUN_08003190(0);
      FUN_0800385c(1,0);
      if (*pcVar5 != '\x01') break;
      FUN_08003190();
    }
    bVar1 = (bool)isCurrentModePrivileged();
    if (bVar1) {
      enableIRQinterrupts(1);
    }
    FUN_08003190(1);
    FUN_08006e78();
    FUN_08003190();
    FUN_0800446c();
    FUN_0800703c();
    pcVar5 = DAT_0800471c;
    if (*pcVar2 == '\x02') {
      *DAT_08004720 = 0;
    }
    else if (*pcVar2 == '\x01') {
      if ((*DAT_0800471c == '\x01') && (*DAT_0800470c == '\0')) {
        *puVar3 = 1;
        *DAT_08004724 = 200;
        _DAT_48000000 = _DAT_48000000 & 0xfff3ffff;
        _DAT_4800000c = _DAT_4800000c & 0xfff3ffff | 0x40000;
        iVar6 = FUN_08002fb6(&DAT_48000000,0x200);
        if (iVar6 == 0) {
          FUN_08004044();
          *DAT_08004728 = 0;
          *DAT_0800472c = 0;
          FUN_0800431c();
        }
      }
      *pcVar2 = '\0';
    }
    FUN_08002fca(DAT_08004730,0x80);
    *pcVar5 = '\0';
    FUN_08004538();
    *DAT_08004734 = 0;
    *DAT_08004714 = 0x28;
    *DAT_08004738 = 0;
    *pcVar2 = '\0';
    FUN_080044bc();
    FUN_08006fcc(DAT_0800473c);
    FUN_080026ac();
    FUN_08007868();
    *(undefined1 *)(DAT_08004700 + 7) = 0;
    bVar1 = (bool)isCurrentModePrivileged();
    if (bVar1) {
      enableIRQinterrupts(0);
    }
  }
  return;
}

