// 0x08031330 FUN_08031330 size=32

void FUN_08031330(int param_1,uint param_2)

{
  FUN_0802dee0(param_1);
  if ((*(byte *)(*(int *)(param_1 + 8) + 0x21) & 0xf) != param_2) {
    *(char *)(*(int *)(param_1 + 8) + 0x21) = (char)param_2;
  }
  return;
}

