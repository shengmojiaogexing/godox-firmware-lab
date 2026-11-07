// 0x080204d8 FUN_080204d8 size=52

undefined4 FUN_080204d8(undefined4 param_1,uint param_2)

{
  uint *puVar1;
  int iVar2;
  uint extraout_r3;
  
  FUN_080207a4();
  iVar2 = FUN_08020570();
  puVar1 = DAT_0802050c;
  if (iVar2 == 1) {
    *DAT_0802050c = *DAT_0802050c & 0xff80ffff | (extraout_r3 & 0x7f) << 0x10;
    *puVar1 = *puVar1 & 0xffff8000 | param_2 & 0x7fff;
    FUN_080205a0();
    FUN_080207b4();
    return 1;
  }
  return 0;
}

