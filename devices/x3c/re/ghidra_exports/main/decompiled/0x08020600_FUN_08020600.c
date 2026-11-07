// 0x08020600 FUN_08020600 size=68

void FUN_08020600(undefined4 param_1,undefined4 param_2)

{
  uint *puVar1;
  uint extraout_r2;
  uint uVar2;
  int extraout_r3;
  
  FUN_080207a4(param_1,param_2,param_1,param_2);
  uVar2 = extraout_r2;
  if ((int)(extraout_r2 << 0x1d) < 0) {
    if (extraout_r3 == 0) {
      *DAT_08020644 = *DAT_08020644 & 0xfffffffb;
    }
    else {
      *DAT_08020644 = *DAT_08020644 | 4;
    }
    uVar2 = extraout_r2 & 0xfffffffb;
  }
  puVar1 = DAT_08020644 + -0xe;
  if (extraout_r3 == 0) {
    *puVar1 = *puVar1 & ~uVar2;
  }
  else {
    *puVar1 = *puVar1 | uVar2;
  }
  FUN_080207b4();
  return;
}

