// 0x08027756 FUN_08027756 size=30

undefined4 FUN_08027756(int param_1)

{
  if ((*(byte *)(param_1 + 0x6c) & 3) != 2) {
    return *(undefined4 *)(param_1 + 0x30);
  }
  if (*(int *)(param_1 + 0x68) == -1) {
    return *(undefined4 *)(param_1 + 0x38);
  }
  return *(undefined4 *)(param_1 + 100);
}

