// 0x0802225c FUN_0802225c size=28

uint FUN_0802225c(undefined4 *param_1)

{
  for (; (param_1 != (undefined4 *)0x0 && ((param_1[7] & DAT_08022278) == 0));
      param_1 = (undefined4 *)*param_1) {
  }
  if (param_1 == (undefined4 *)0x0) {
    return 0;
  }
  return (param_1[7] & 0xfffff) >> 4;
}

