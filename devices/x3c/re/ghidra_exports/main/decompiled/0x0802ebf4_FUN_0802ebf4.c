// 0x0802ebf4 FUN_0802ebf4 size=14

undefined4 FUN_0802ebf4(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0;
  }
  return *(undefined4 *)(*(int *)(param_1 + 8) + 4);
}

