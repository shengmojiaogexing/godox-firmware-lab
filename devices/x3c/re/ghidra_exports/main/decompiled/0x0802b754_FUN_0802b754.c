// 0x0802b754 FUN_0802b754 size=20

undefined4 FUN_0802b754(int param_1)

{
  if (param_1 == 0) {
    return 0;
  }
  if (*(undefined4 **)(param_1 + 0xc) == (undefined4 *)0x0) {
    return 0;
  }
  return **(undefined4 **)(param_1 + 0xc);
}

