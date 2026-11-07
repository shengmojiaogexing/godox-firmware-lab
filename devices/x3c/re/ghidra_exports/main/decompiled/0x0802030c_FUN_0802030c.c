// 0x0802030c FUN_0802030c size=48

void FUN_0802030c(undefined4 param_1,undefined4 param_2)

{
  uint extraout_r2;
  int extraout_r3;
  
  FUN_080207a4(param_1,param_2,param_2,param_1);
  if (extraout_r3 == 0) {
    DAT_0802033c[-1] = DAT_0802033c[-1] & 0x7f7f7f7f | extraout_r2;
  }
  else {
    *DAT_0802033c = *DAT_0802033c & 0x7f7f7f7f | extraout_r2;
  }
  FUN_080207b4();
  return;
}

