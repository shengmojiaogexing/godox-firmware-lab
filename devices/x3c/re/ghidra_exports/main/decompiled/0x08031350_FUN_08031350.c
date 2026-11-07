// 0x08031350 FUN_08031350 size=26

void FUN_08031350(int param_1,byte param_2)

{
  byte *pbVar1;
  
  FUN_0802dee0(param_1);
  pbVar1 = (byte *)(*(int *)(param_1 + 8) + 0x20);
  *pbVar1 = *pbVar1 & 0xf3 | (param_2 & 3) << 2;
  return;
}

