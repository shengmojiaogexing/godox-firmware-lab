// 0x0802f0dc FUN_0802f0dc size=16

int FUN_0802f0dc(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0;
  }
  return -*(int *)(*(int *)(param_1 + 8) + 0x14);
}

