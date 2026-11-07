// 0x080398f0 FUN_080398f0 size=10

void FUN_080398f0(uint *param_1,uint param_2)

{
  *param_1 = *param_1 & 0xffffff8f | (param_2 & 7) << 4;
  return;
}

