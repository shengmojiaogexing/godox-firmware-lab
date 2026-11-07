// 0x080202bc FUN_080202bc size=76

undefined4 FUN_080202bc(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  uint extraout_r2;
  int extraout_r3;
  
  FUN_080207a4(param_1,param_2,param_2,param_1);
  if (extraout_r3 == 0) {
    *DAT_08020308 = *DAT_08020308 & 0xfffffeff | (extraout_r2 & 1) << 8;
    if ((extraout_r2 == 0) && (iVar1 = FUN_0802074c(1,0), iVar1 != 1)) {
      return 0;
    }
  }
  else {
    *DAT_08020308 = *DAT_08020308 & 0xfffffdff | (extraout_r2 & 1) << 9;
    if ((extraout_r2 == 0) && (iVar1 = FUN_0802074c(2,0), iVar1 != 1)) {
      return 0;
    }
  }
  FUN_080207b4();
  return 1;
}

