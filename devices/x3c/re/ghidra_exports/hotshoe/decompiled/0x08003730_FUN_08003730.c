// 0x08003730 FUN_08003730 size=144

void FUN_08003730(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  short sVar2;
  char *pcVar3;
  undefined2 *puVar4;
  undefined1 *puVar5;
  undefined4 uVar6;
  short *psVar7;
  int iVar8;
  
  psVar7 = DAT_080037d0;
  uVar6 = DAT_080037cc;
  puVar5 = DAT_080037c8;
  puVar4 = DAT_080037c4;
  pcVar3 = DAT_080037c0;
  cVar1 = *DAT_080037c0;
  if (cVar1 == '\x03') {
    *DAT_080037c4 = 0;
    iVar8 = FUN_08002fb6(uVar6,4,param_3,puVar4,param_4);
    if (iVar8 == 1) {
      FUN_08002fca(uVar6,0x80);
      iVar8 = FUN_080047c0();
      if (iVar8 != 0) {
        FUN_080047c0();
      }
      *pcVar3 = '\0';
      *puVar5 = 0;
      return;
    }
  }
  else {
    sVar2 = *DAT_080037d0;
    if (sVar2 != 0) {
      *DAT_080037c4 = 0;
      *psVar7 = sVar2 + -1;
      if (sVar2 == 1) {
        if (cVar1 == '\x01') {
          FUN_08002f0c(uVar6,0x80);
          FUN_08007294(DAT_080037d4);
          FUN_08002fca(uVar6,0x80);
          FUN_080072f2(DAT_080037d8);
          *pcVar3 = '\0';
          *puVar5 = 0;
        }
        else {
          if (cVar1 != '\x02') {
            return;
          }
          *pcVar3 = '\0';
          *puVar5 = 0;
          FUN_08002fca(uVar6,0x80);
        }
        iVar8 = FUN_080047c0();
        if (iVar8 != 0) {
          FUN_080047c0();
        }
      }
    }
  }
  return;
}

