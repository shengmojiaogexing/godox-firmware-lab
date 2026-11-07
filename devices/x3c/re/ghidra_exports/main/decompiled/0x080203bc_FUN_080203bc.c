// 0x080203bc FUN_080203bc size=44

void FUN_080203bc(undefined4 param_1,undefined4 param_2)

{
  uint extraout_r2;
  int extraout_r3;
  
  FUN_080207a4(param_1,param_2,param_2,param_1);
  if (extraout_r3 == 0) {
    DAT_080203e8[-1] = DAT_080203e8[-1] & 0xbfffffff | (extraout_r2 & 1) << 0x1e;
  }
  else {
    *DAT_080203e8 = *DAT_080203e8 & 0xbfffffff | (extraout_r2 & 1) << 0x1e;
  }
  FUN_080207b4();
  return;
}

