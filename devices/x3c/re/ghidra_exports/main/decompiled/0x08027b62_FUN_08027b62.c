// 0x08027b62 FUN_08027b62 size=30

uint FUN_08027b62(uint param_1)

{
  return (param_1 & 0xf80000) >> 8 | (param_1 & 0xfc00) >> 5 | (param_1 & 0xff) >> 3;
}

