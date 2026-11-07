// 0x08027774 FUN_08027774 size=14

undefined4 FUN_08027774(int param_1)

{
  if (*(int *)(param_1 + 0x58) == -1) {
    return *(undefined4 *)(param_1 + 0x2c);
  }
  return *(undefined4 *)(param_1 + 0x54);
}

