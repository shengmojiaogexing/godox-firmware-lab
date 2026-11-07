// 0x0802091c FUN_0802091c size=72

void FUN_0802091c(undefined4 param_1,uint param_2,undefined4 param_3,undefined4 param_4)

{
  uint local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_10 = param_2;
  local_c = param_3;
  local_8 = param_4;
  FUN_0801bd04(DAT_08020964,1);
  FUN_08036bec(2,10);
  FUN_08020968(&local_10);
  local_8 = CONCAT22(local_8._2_2_,0x100);
  local_10 = local_10 & 0xffffff00;
  local_c = 0x400;
  local_8 = CONCAT31(local_8._1_3_,2);
  FUN_0802099c(&local_10);
  FUN_08035cf0(3);
  FUN_08035c78(0x28,0xf,0);
  return;
}

