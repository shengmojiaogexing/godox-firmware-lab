// 0x08039a8c FUN_08039a8c size=32

void FUN_08039a8c(int param_1,uint param_2)

{
  if (param_1 == 0x40000000) {
    uRam40000050 = uRam40000050 & 0xfffff3ff | (param_2 & 3) << 10;
    return;
  }
  if (param_1 == DAT_08039aac) {
    *(uint *)(param_1 + 0x50) = *(uint *)(param_1 + 0x50) & 0xffffff3f | (param_2 & 3) << 6;
  }
  return;
}

