// 0x0802c55a FUN_0802c55a size=36

undefined4 FUN_0802c55a(byte *param_1)

{
  if (param_1 == (byte *)0x0) {
    return 3;
  }
  if (*param_1 - 0x20 < 0x60) {
    return 1;
  }
  if (*param_1 < 0x80) {
    return 0;
  }
  return 2;
}

