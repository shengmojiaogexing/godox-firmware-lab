// 0x0802e218 FUN_0802e218 size=122

void FUN_0802e218(undefined4 param_1)

{
  int iVar1;
  undefined4 extraout_r3;
  
  FUN_08030436(param_1);
  FUN_0802e79c(0);
  FUN_080333b0(param_1);
  FUN_0802e35c(param_1);
  FUN_0802e79c(1);
  FUN_08030ca4(param_1,0xf0000,0xffff);
  FUN_08030c70(param_1);
  iVar1 = FUN_0802b738();
  if ((iVar1 != 0) && (iVar1 = FUN_08030360(param_1), iVar1 != 0)) {
    FUN_0802b584(extraout_r3,param_1);
  }
  iVar1 = FUN_0802ee0a(param_1);
  if (iVar1 == 0) {
    return;
  }
  FUN_0802ad74(iVar1,0x22,param_1);
  FUN_0802ad74(iVar1,0x23,param_1);
  FUN_080302d0(param_1);
  return;
}

