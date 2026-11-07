// 0x08038400 FUN_08038400 size=10

void FUN_08038400(uint *param_1,uint param_2)

{
  *param_1 = *param_1 & 0xfffffeff | (param_2 & 1) << 8;
  return;
}

