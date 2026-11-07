// 0x0802c660 FUN_0802c660 size=34

byte FUN_0802c660(undefined4 *param_1)

{
  if (param_1 == (undefined4 *)0x0) {
    return 0;
  }
  if ((*(char *)*param_1 == '\x01') || (*(char *)*param_1 == '\x03')) {
    return *(byte *)(param_1 + 0x1c) & 0xf;
  }
  return 0;
}

