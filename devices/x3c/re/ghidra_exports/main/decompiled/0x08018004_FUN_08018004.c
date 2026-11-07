// 0x08018004 FUN_08018004 size=26

undefined4 FUN_08018004(int param_1)

{
  if (param_1 == 0) {
    return 1;
  }
  if ((*(int *)(param_1 + 4) == 0) && (*(int *)(param_1 + 8) == 0)) {
    return 1;
  }
  return 0;
}

