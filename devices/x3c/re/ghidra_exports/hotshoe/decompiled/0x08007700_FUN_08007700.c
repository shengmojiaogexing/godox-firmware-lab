// 0x08007700 FUN_08007700 size=310

void FUN_08007700(void)

{
  byte bVar1;
  ushort uVar2;
  bool bVar3;
  char *pcVar4;
  char *pcVar5;
  byte *pbVar6;
  ushort *puVar7;
  char *pcVar8;
  int iVar9;
  
  bVar3 = (bool)isCurrentModePrivileged();
  if (bVar3) {
    enableIRQinterrupts(1);
  }
  FUN_08006e78(1);
  FUN_080031a0();
  FUN_08003440();
  FUN_0800337c();
  FUN_08003364();
  FUN_08003324();
  FUN_08007294(500);
  FUN_08003190();
  FUN_0800504c();
  FUN_08002724();
  FUN_08002fca(DAT_08007838,2);
  FUN_0800441c();
  FUN_08002fca(&DAT_48000000,0x100);
  FUN_080044bc();
  FUN_080026ac();
  enableIRQinterrupts();
  bVar3 = (bool)isCurrentModePrivileged();
  if (bVar3) {
    enableIRQinterrupts(0);
  }
  if (*DAT_0800783c == '\0') {
    FUN_08006fcc(DAT_08007840);
  }
  FUN_080026ac();
  pcVar4 = DAT_08007844;
  *DAT_08007844 = '\0';
  pbVar6 = DAT_0800784c;
  pcVar5 = DAT_08007848;
  do {
    do {
      FUN_08003190();
    } while (*pcVar5 != '\x01');
    *pcVar5 = '\0';
    FUN_08003190();
    if (*pcVar4 == '\0') {
      iVar9 = FUN_0800504c();
      puVar7 = DAT_08007850;
      if (iVar9 != 0) {
        uVar2 = *DAT_08007850;
        *DAT_08007850 = uVar2 + 1;
        if (300 < (ushort)(uVar2 + 1)) {
          *DAT_08007854 = 2;
          *puVar7 = 0;
        }
        goto LAB_080077ae;
      }
      FUN_08005104();
      *pcVar4 = '\x01';
      *DAT_08007850 = 0;
LAB_080077c2:
      bVar1 = *pbVar6;
      *pbVar6 = bVar1 + 1;
      pcVar8 = DAT_08007858;
      if (0x13 < (byte)(bVar1 + 1)) {
        *pbVar6 = 0;
        if (*pcVar8 == '\0') {
          *DAT_08007860 = 1;
          if (*DAT_08007864 == '\0') {
            *DAT_08007864 = '\x01';
            FUN_08004f74();
            *DAT_08007860 = 0;
            *DAT_08007864 = '\0';
          }
          iVar9 = FUN_080070cc();
          if ((iVar9 != 0) && (*DAT_0800785c = 1, *DAT_08007864 == '\0')) {
            *DAT_08007864 = '\x01';
            FUN_08004e8c();
            *DAT_0800785c = 0;
            *DAT_08007864 = '\0';
          }
        }
        else {
          *DAT_0800785c = 1;
          *DAT_08007860 = 1;
        }
      }
    }
    else {
LAB_080077ae:
      if (*pcVar4 == '\x01') goto LAB_080077c2;
    }
    FUN_0800455c();
    FUN_080040d8();
    FUN_08003730();
  } while( true );
}

