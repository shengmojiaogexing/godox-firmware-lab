// 0x0802c634 FUN_0802c634 size=44

void FUN_0802c634(undefined4 *param_1,undefined4 *param_2)

{
  if (param_1 == (undefined4 *)0x0) {
    *param_2 = 0;
    param_2[1] = 0;
    return;
  }
  if ((*(char *)*param_1 == '\x01') || (*(char *)*param_1 == '\x03')) {
    *param_2 = param_1[2];
    param_2[1] = param_1[3];
    return;
  }
  *param_2 = 0xffffffff;
  param_2[1] = 0xffffffff;
  return;
}

