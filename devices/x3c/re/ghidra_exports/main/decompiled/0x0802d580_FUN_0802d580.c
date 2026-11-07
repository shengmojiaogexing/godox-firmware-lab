// 0x0802d580 FUN_0802d580 size=68

int FUN_0802d580(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  if ((param_2 <= param_3) && (param_3 <= param_1)) {
    return param_5;
  }
  if ((param_2 <= param_3) && (param_1 <= param_2)) {
    return param_4;
  }
  if ((param_3 <= param_2) && (param_1 <= param_3)) {
    return param_5;
  }
  if ((param_2 < param_3) || (param_1 < param_2)) {
    return ((param_5 - param_4) * (param_1 - param_2)) / (param_3 - param_2) + param_4;
  }
  return param_4;
}

