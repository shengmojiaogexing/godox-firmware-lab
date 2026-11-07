// 0x0803136a FUN_0803136a size=26

void FUN_0803136a(int param_1,byte param_2)

{
  byte *pbVar1;
  
  FUN_0802dee0(param_1);
  pbVar1 = (byte *)(*(int *)(param_1 + 8) + 0x20);
  *pbVar1 = *pbVar1 & 0xcf | (param_2 & 3) << 4;
  return;
}

