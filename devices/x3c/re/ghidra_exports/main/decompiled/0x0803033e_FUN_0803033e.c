// 0x0803033e FUN_0803033e size=34

bool FUN_0803033e(undefined4 *param_1)

{
  for (param_1 = (undefined4 *)*param_1;
      (param_1 != (undefined4 *)0x0 && ((*(byte *)(param_1 + 7) & 3) == 0));
      param_1 = (undefined4 *)*param_1) {
  }
  if (param_1 == (undefined4 *)0x0) {
    return false;
  }
  return (*(byte *)(param_1 + 7) & 3) == 1;
}

