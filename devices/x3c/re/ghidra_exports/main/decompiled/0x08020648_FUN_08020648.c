// 0x08020648 FUN_08020648 size=22

uint FUN_08020648(uint param_1)

{
  return param_1 % 10 | (param_1 / 10 & 0xf) << 4;
}

