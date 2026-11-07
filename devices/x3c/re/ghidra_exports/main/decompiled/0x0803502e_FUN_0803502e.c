// 0x0803502e FUN_0803502e size=42

void FUN_0803502e(uint param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  if (0x7f < param_1) {
    iVar1 = FUN_080398a6(param_1);
    param_1 = param_1 + (1 << (iVar1 - 5U & 0xff)) + -1;
  }
  FUN_08035000(param_1,param_2,param_3);
  return;
}

