// 0x08030360 FUN_08030360 size=36

bool FUN_08030360(undefined4 *param_1)

{
  for (param_1 = (undefined4 *)*param_1;
      (param_1 != (undefined4 *)0x0 && ((*(byte *)(param_1 + 7) & 0xc) == 0));
      param_1 = (undefined4 *)*param_1) {
  }
  if (param_1 == (undefined4 *)0x0) {
    return false;
  }
  return (*(byte *)(param_1 + 7) & 0xf) >> 2 == 1;
}

