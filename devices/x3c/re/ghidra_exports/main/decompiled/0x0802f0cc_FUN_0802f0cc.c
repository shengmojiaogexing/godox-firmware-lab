// 0x0802f0cc FUN_0802f0cc size=16

int FUN_0802f0cc(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0;
  }
  return -*(int *)(*(int *)(param_1 + 8) + 0x10);
}

