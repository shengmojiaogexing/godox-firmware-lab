// 0x08003cf0 FUN_08003cf0 size=40

void FUN_08003cf0(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  uint extraout_r2;
  
  FUN_08003c88(param_1,param_2,param_1);
  iVar1 = DAT_08003d18;
  *(uint *)(DAT_08003d18 + 8) = *(uint *)(DAT_08003d18 + 8) | extraout_r2 & 0xfffffffb;
  *(uint *)(DAT_08003d18 + 0x40) = *(uint *)(DAT_08003d18 + 0x40) | extraout_r2 & 4;
  *(undefined4 *)(iVar1 + 0x24) = 0xff;
  return;
}

