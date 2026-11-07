// 0x0803a6b0 FUN_0803a6b0 size=14

uint FUN_0803a6b0(int param_1)

{
  return *(uint *)(param_1 + 0x81c) & *(uint *)(param_1 + 0x818) & 0xffff;
}

