// 0x0801bce8 FUN_0801bce8 size=22

void FUN_0801bce8(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  uint extraout_r3;
  
  FUN_0801bcd8(1,param_2,param_3,param_1);
  *DAT_0801bd00 = *DAT_0801bd00 & 0xffffbfff | (extraout_r3 & 1) << 0xe;
  return;
}

