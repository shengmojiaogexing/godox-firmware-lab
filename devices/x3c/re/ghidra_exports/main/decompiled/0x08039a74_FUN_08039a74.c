// 0x08039a74 FUN_08039a74 size=24

void FUN_08039a74(int param_1,uint param_2,int param_3)

{
  if (param_3 == 1) {
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | param_2;
    return;
  }
  if (param_3 == 0) {
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & ~param_2;
  }
  return;
}

