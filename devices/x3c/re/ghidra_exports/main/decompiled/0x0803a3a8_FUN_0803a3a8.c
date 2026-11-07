// 0x0803a3a8 FUN_0803a3a8 size=96

void FUN_0803a3a8(int param_1,byte *param_2)

{
  int iVar1;
  
  iVar1 = param_1 + (uint)*param_2 * 0x20;
  if (param_2[2] == 0) {
    *(uint *)(iVar1 + 0x900) = *(uint *)(iVar1 + 0x900) & 0xffdfffff;
    if ((param_2[3] == 3) || (param_2[3] == 2)) {
      param_1 = param_1 + (uint)*param_2 * 0x20;
      *(uint *)(param_1 + 0x900) = *(uint *)(param_1 + 0x900) | 0x10000000;
      return;
    }
  }
  else {
    *(uint *)(iVar1 + 0xb00) = *(uint *)(iVar1 + 0xb00) & 0xffdfffff;
    if ((param_2[3] == 3) || (param_2[3] == 2)) {
      param_1 = param_1 + (uint)*param_2 * 0x20;
      *(uint *)(param_1 + 0xb00) = *(uint *)(param_1 + 0xb00) | 0x10000000;
    }
  }
  return;
}

