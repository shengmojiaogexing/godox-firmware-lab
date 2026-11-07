// 0x080fa31c FUN_080fa31c size=210

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_080fa31c(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  byte *pbVar2;
  uint uVar3;
  int iVar4;
  byte bVar5;
  
  pbVar2 = DAT_080fa3f0;
  if (*DAT_080fa3f0 == 0) {
    if (*DAT_080fa3f4 == '\x01') {
      return;
    }
    *DAT_080fa3f8 = 0;
    FUN_080f8f0c(&DAT_48000000,2,param_3,param_4,param_4);
    FUN_080f8fca(&DAT_48000000,8);
    uVar3 = (uint)*DAT_080fa3fc;
    if ((uVar3 == 0xb7) && (uVar3 = (uint)*DAT_080fa400, uVar3 == 0)) {
      uVar3 = *(uint *)(DAT_080fa404 + 8);
    }
    FUN_080f937c(uVar3);
    *DAT_080fa408 = 2;
    *DAT_080fa40c = 1;
    FUN_080f8f0c(&DAT_48000000,8);
    FUN_080fd2b4(0xf);
    pbVar2 = DAT_080fa410;
    DAT_080fa410[2] = 0xff;
    _DAT_48000000 = _DAT_48000000 & 0xffffffcf;
    _DAT_4800000c = _DAT_4800000c & 0xffffffcf | 0x20;
    FUN_080fd2b4(0xf);
    iVar4 = FUN_080f8fb6(&DAT_48000000,4);
    iVar1 = DAT_080fa414;
    if (iVar4 == 0) {
      *(uint *)(DAT_080fa414 + 0x14) = *(uint *)(DAT_080fa414 + 0x14) ^ 1;
    }
    iVar4 = FUN_080f8fc0(iVar1,1);
    bVar5 = pbVar2[2];
    if (iVar4 != 0) {
      bVar5 = ~bVar5;
    }
    *DAT_080fa410 = bVar5;
    FUN_080fa2cc(DAT_080fa404,bVar5);
    FUN_080fd2b4(0x50);
    FUN_080f8f0c(iVar1,2);
    FUN_080f8fca(&DAT_48000000,2);
    *DAT_080fa418 = 0;
    pbVar2 = DAT_080fa3fc;
  }
  *pbVar2 = 0;
  return;
}

