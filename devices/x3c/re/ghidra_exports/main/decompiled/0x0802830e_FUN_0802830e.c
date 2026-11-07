// 0x0802830e FUN_0802830e size=32

undefined4 FUN_0802830e(int param_1)

{
  if (param_1 == 0) {
    param_1 = FUN_0802821c();
  }
  if (param_1 == 0) {
    return 0;
  }
  if (*(int *)(param_1 + 0x254) < 1) {
    return 0;
  }
  return 1;
}

