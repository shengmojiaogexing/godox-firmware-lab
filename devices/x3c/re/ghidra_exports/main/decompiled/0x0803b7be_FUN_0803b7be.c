// 0x0803b7be FUN_0803b7be size=38

undefined4 FUN_0803b7be(int param_1)

{
  if (*(short *)(param_1 + 0x2ce) == 1) {
    *(undefined1 *)(param_1 + 0x2e7) = 1;
    (**(code **)(*(int *)(param_1 + 4) + 8))(param_1);
    FUN_0803acbc(param_1);
  }
  return 0;
}

