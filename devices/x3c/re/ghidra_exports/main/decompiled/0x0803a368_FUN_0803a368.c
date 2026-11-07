// 0x0803a368 FUN_0803a368 size=26

void FUN_0803a368(int param_1)

{
  *(uint *)(param_1 + 0x900) = *(uint *)(param_1 + 0x900) & 0xfffff800;
  *(uint *)(param_1 + 0x804) = *(uint *)(param_1 + 0x804) & 0xfffffeff;
  return;
}

