// 0x08017f30 FUN_08017f30 size=12

undefined4 FUN_08017f30(int param_1)

{
  if (param_1 == 0) {
    return 0;
  }
  return *(undefined4 *)(param_1 + 4);
}

