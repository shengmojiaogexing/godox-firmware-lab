// 0x0803a408 FUN_0803a408 size=76

void FUN_0803a408(int param_1,byte *param_2)

{
  if (param_2[2] == 0) {
    *(uint *)(param_1 + 0x81c) = *(uint *)(param_1 + 0x81c) & ~(1 << *param_2);
    param_1 = param_1 + (uint)*param_2 * 0x20;
    *(uint *)(param_1 + 0x900) = *(uint *)(param_1 + 0x900) & 0xffff7fff;
    return;
  }
  *(uint *)(param_1 + 0x81c) = *(uint *)(param_1 + 0x81c) & ~((1 << *param_2) << 0x10);
  param_1 = param_1 + (uint)*param_2 * 0x20;
  *(uint *)(param_1 + 0xb00) = *(uint *)(param_1 + 0xb00) & 0xffff7fff;
  return;
}

