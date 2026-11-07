// 0x0800431c FUN_0800431c size=210

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0800431c(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  byte *pbVar2;
  uint uVar3;
  int iVar4;
  byte bVar5;
  
  pbVar2 = DAT_080043f0;
  if (*DAT_080043f0 == 0) {
    if (*DAT_080043f4 == '\x01') {
      return;
    }
    *DAT_080043f8 = 0;
    FUN_08002f0c(&DAT_48000000,2,param_3,param_4,param_4);
    FUN_08002fca(&DAT_48000000,8);
    uVar3 = (uint)*DAT_080043fc;
    if ((uVar3 == 0xb7) && (uVar3 = (uint)*DAT_08004400, uVar3 == 0)) {
      uVar3 = *(uint *)(DAT_08004404 + 8);
    }
    FUN_0800337c(uVar3);
    *DAT_08004408 = 2;
    *DAT_0800440c = 1;
    FUN_08002f0c(&DAT_48000000,8);
    FUN_080072b4(0xf);
    pbVar2 = DAT_08004410;
    DAT_08004410[2] = 0xff;
    _DAT_48000000 = _DAT_48000000 & 0xffffffcf;
    _DAT_4800000c = _DAT_4800000c & 0xffffffcf | 0x20;
    FUN_080072b4(0xf);
    iVar4 = FUN_08002fb6(&DAT_48000000,4);
    iVar1 = DAT_08004414;
    if (iVar4 == 0) {
      *(uint *)(DAT_08004414 + 0x14) = *(uint *)(DAT_08004414 + 0x14) ^ 1;
    }
    iVar4 = FUN_08002fc0(iVar1,1);
    bVar5 = pbVar2[2];
    if (iVar4 != 0) {
      bVar5 = ~bVar5;
    }
    *DAT_08004410 = bVar5;
    FUN_080042cc(DAT_08004404,bVar5);
    FUN_080072b4(0x50);
    FUN_08002f0c(iVar1,2);
    FUN_08002fca(&DAT_48000000,2);
    *DAT_08004418 = 0;
    pbVar2 = DAT_080043fc;
  }
  *pbVar2 = 0;
  return;
}

