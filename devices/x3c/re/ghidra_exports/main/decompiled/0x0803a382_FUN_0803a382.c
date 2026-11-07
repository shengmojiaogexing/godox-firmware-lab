// 0x0803a382 FUN_0803a382 size=38

void FUN_0803a382(int param_1)

{
  *(undefined4 *)(param_1 + 0xb10) = 0;
  *(uint *)(param_1 + 0xb10) = *(uint *)(param_1 + 0xb10) & 0xe007ffff | 0x80000;
  *(uint *)(param_1 + 0xb10) = (*(uint *)(param_1 + 0xb10) & 0xfff80000) + 0x18;
  *(uint *)(param_1 + 0xb10) = *(uint *)(param_1 + 0xb10) | 0x60000000;
  return;
}

