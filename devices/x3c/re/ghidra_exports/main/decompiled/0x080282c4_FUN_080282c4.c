// 0x080282c4 FUN_080282c4 size=20

undefined4 FUN_080282c4(int param_1)

{
  if (param_1 == 0) {
    param_1 = FUN_0802821c();
  }
  if (param_1 == 0) {
    return 0;
  }
  return *(undefined4 *)(param_1 + 0x14);
}

