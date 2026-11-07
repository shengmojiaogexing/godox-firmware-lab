// 0x08018cb0 FUN_08018cb0 size=12

void FUN_08018cb0(int param_1,int param_2)

{
  *(uint *)(param_1 + 0x38) = *(uint *)(param_1 + 0x38) & 0xffcfffff | (param_2 - 1U & 3) << 0x14;
  return;
}

