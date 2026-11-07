// 0x08039924 FUN_08039924 size=56

void FUN_08039924(int param_1,int param_2,uint param_3)

{
  if (param_2 == 0) {
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffffff3 | (param_3 & 3) << 2;
    return;
  }
  if (param_2 == 2) {
    *(uint *)(param_1 + 0x18) = *(uint *)(param_1 + 0x18) & 0xfffff3ff | (param_3 & 3) << 10;
    return;
  }
  if (param_2 == 4) {
    *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffffff3 | (param_3 & 3) << 2;
    return;
  }
  if (param_2 == 6) {
    *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffff3ff | (param_3 & 3) << 10;
  }
  return;
}

