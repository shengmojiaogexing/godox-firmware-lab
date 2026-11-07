// 0x0803a326 FUN_0803a326 size=52

undefined4 FUN_0803a326(int param_1,int param_2)

{
  *(char *)(param_1 + 0x2f4) = (char)param_2;
  *(char *)(param_1 + 0x2fd) = (char)param_2;
  *(undefined1 *)(param_1 + 0x2ff) = 0;
  *(undefined1 *)(param_1 + 0x301) = 0;
  *(undefined2 *)(param_1 + 0x2f8) = 0x40;
  *(undefined1 *)(param_1 + 0x2f7) = 8;
  *(undefined2 *)(param_1 + 0x2fa) = 0x140;
  if (param_2 == 1) {
    *(undefined1 *)(param_1 + 0x2fc) = 2;
  }
  *(undefined1 *)(param_1 + 0x301) = 1;
  return 0;
}

