// 0x08035164 FUN_08035164 size=28

uint FUN_08035164(uint param_1,uint param_2)

{
  if (param_2 < 0xfd) {
    if (2 < param_2) {
      return param_2 * param_1 * 0x10102 >> 0x18;
    }
    param_1 = 0;
  }
  return param_1;
}

