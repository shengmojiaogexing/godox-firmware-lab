// 0x080282d8 FUN_080282d8 size=14

undefined4 FUN_080282d8(int param_1)

{
  if (param_1 == 0) {
    param_1 = FUN_0802821c();
  }
  return *(undefined4 *)(param_1 + 8);
}

