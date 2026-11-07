// 0x080f9a60 FUN_080f9a60 size=16

uint FUN_080f9a60(uint param_1)

{
  return (param_1 & 0xf) + (param_1 >> 4) * 10 & 0xff;
}

