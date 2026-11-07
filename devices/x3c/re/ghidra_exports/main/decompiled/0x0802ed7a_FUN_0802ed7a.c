// 0x0802ed7a FUN_0802ed7a size=14

undefined4 FUN_0802ed7a(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0;
  }
  return *(undefined4 *)(*(int *)(param_1 + 8) + 8);
}

