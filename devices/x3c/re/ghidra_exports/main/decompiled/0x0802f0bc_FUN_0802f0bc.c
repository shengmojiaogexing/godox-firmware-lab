// 0x0802f0bc FUN_0802f0bc size=16

int FUN_0802f0bc(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0;
  }
  return -*(int *)(*(int *)(param_1 + 8) + 0x14);
}

