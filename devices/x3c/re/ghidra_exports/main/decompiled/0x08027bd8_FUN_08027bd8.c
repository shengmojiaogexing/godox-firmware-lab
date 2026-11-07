// 0x08027bd8 FUN_08027bd8 size=44

uint FUN_08027bd8(uint param_1,uint param_2,uint param_3)

{
  return param_3 >> 3 & 0x1f | (param_2 >> 2 & 0x3f) << 5 | (param_1 >> 3 & 0x1f) << 0xb;
}

