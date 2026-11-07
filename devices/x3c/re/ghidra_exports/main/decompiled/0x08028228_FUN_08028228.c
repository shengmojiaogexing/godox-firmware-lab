// 0x08028228 FUN_08028228 size=26

uint FUN_08028228(int *param_1)

{
  if (param_1 == (int *)0x0) {
    param_1 = (int *)FUN_0802821c();
  }
  if (param_1 == (int *)0x0) {
    return 0x82;
  }
  return (*(uint *)(*param_1 + 0x1c) & 0x1ffff) >> 7;
}

