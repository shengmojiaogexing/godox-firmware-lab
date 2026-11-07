// 0x080040d8 FUN_080040d8 size=360

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_080040d8(void)

{
  byte *pbVar1;
  byte *pbVar2;
  undefined2 *puVar3;
  int iVar4;
  char cVar5;
  undefined4 in_r3;
  
  FUN_08004278();
  pbVar1 = DAT_08004244;
  if (*DAT_08004240 == '\0') {
    if ((*DAT_08004244 & 0xf) == 0) {
      _DAT_48000000 = _DAT_48000000 & 0xfff3ffff;
      _DAT_4800000c = _DAT_4800000c & 0xfff3ffff | 0x40000;
      iVar4 = FUN_08002fb6(&DAT_48000000,0x200);
      pbVar2 = DAT_08004248;
      if (iVar4 == 0) {
        *DAT_08004248 = *DAT_08004248 << 1;
        FUN_08002fca(&DAT_48000000,0x100);
        FUN_08004538();
        if ((*DAT_08004250 & 3) == 2) {
          *DAT_08004254 = 0;
        }
        if (*DAT_08004258 == '\0') {
          FUN_0800431c();
        }
      }
      else {
        *DAT_08004248 = *DAT_08004248 * '\x02' + 1;
        FUN_08002f0c(&DAT_48000000);
        if ((*DAT_0800424c == '\x01') && (iVar4 = FUN_08002fb6(DAT_0800425c,4), iVar4 != 0)) {
          cVar5 = *DAT_08004260 + -1;
          *DAT_08004260 = cVar5;
          _DAT_48000000 = _DAT_48000000 & 0xfff3ffff;
          _DAT_4800000c = _DAT_4800000c & 0xfff3ffff | 0x40000;
          if ((cVar5 == '\0') ||
             ((*DAT_08004264 == '\0' ||
              (iVar4 = FUN_08002fb6(&DAT_48000000,0x200,cVar5,0x40000,in_r3), iVar4 == 1)))) {
            *DAT_0800424c = '\0';
            *DAT_08004268 = 0;
            *pbVar1 = 0;
            FUN_080044bc();
            FUN_08002f0c(&DAT_48000000,2);
            FUN_08002fca(&DAT_48000000,8);
            FUN_08003084();
            FUN_08004e5c();
            *DAT_08004258 = '\0';
            *DAT_0800426c = 0;
            FUN_08002f0c(&DAT_48000000,1);
            puVar3 = DAT_08004274;
            *DAT_08004270 = 0;
            *puVar3 = 5;
          }
        }
      }
      if ((*pbVar2 & 3) == 1) {
        *DAT_0800424c = '\0';
        *DAT_08004268 = 0;
        *pbVar1 = 0;
        FUN_080044bc();
        FUN_08002f0c(&DAT_48000000,2);
        FUN_08002fca(&DAT_48000000,8);
        FUN_08003084();
        FUN_08004e5c();
        *DAT_08004258 = '\0';
        *DAT_0800426c = 0;
        FUN_08002f0c(&DAT_48000000,1);
        puVar3 = DAT_08004274;
        *DAT_08004270 = 0;
        *puVar3 = 5;
        return;
      }
    }
  }
  else {
    FUN_080044bc();
  }
  return;
}

