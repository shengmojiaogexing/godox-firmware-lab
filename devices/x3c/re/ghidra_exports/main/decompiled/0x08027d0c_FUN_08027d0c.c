// 0x08027d0c FUN_08027d0c size=44

uint FUN_08027d0c(uint param_1,uint param_2,uint param_3)

{
  return param_3 >> 3 & 0x1f | (param_2 >> 2 & 0x3f) << 5 | (param_1 >> 3 & 0x1f) << 0xb;
}

