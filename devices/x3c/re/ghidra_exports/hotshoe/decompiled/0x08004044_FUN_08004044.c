// 0x08004044 FUN_08004044 size=142

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08004044(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint *puVar1;
  
  FUN_08002fca(&DAT_48000000,8,param_3,param_4,param_4);
  puVar1 = DAT_080040d4;
  *DAT_080040d4 = *DAT_080040d4 & 0xfffffff3;
  *puVar1 = *puVar1 | 4;
  puVar1[3] = puVar1[3] & 0xfffffff3;
  FUN_08002f0c(puVar1,2);
  _DAT_48000000 = _DAT_48000000 & 0xfffffff3 | 4;
  _DAT_4800000c = _DAT_4800000c & 0xfffffff3;
  FUN_08002fca(&DAT_48000000,2);
  *puVar1 = *puVar1 & 0xfffffffc;
  *puVar1 = *puVar1 | 1;
  puVar1[3] = puVar1[3] & 0xfffffffe;
  puVar1[3] = puVar1[3] | 2;
  FUN_08002f0c(puVar1,1);
  _DAT_48000000 = _DAT_48000000 & 0xffffffcf;
  _DAT_4800000c = _DAT_4800000c & 0xffffffcf | 0x20;
  FUN_08002fca(&DAT_48000000,0x100);
  return;
}

