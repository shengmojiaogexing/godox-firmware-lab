// 0x0802c682 FUN_0802c682 size=28

undefined4 FUN_0802c682(undefined4 *param_1)

{
  if (param_1 == (undefined4 *)0x0) {
    return 0;
  }
  if ((*(char *)*param_1 == '\x01') || (*(char *)*param_1 == '\x03')) {
    return param_1[0x14];
  }
  return 0;
}

