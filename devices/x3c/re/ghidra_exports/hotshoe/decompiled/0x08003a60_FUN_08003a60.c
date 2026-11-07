// 0x08003a60 FUN_08003a60 size=16

uint FUN_08003a60(uint param_1)

{
  return (param_1 & 0xf) + (param_1 >> 4) * 10 & 0xff;
}

