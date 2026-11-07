// 0x080180c2 FUN_080180c2 size=14

undefined4 FUN_080180c2(int param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    return 0;
  }
  return *(undefined4 *)(*(int *)(param_1 + 8) + 0x1c);
}

