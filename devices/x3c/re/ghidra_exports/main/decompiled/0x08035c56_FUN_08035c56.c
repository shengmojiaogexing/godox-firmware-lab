// 0x08035c56 FUN_08035c56 size=34

void FUN_08035c56(uint param_1)

{
  if (-1 < (int)param_1) {
    *(int *)((param_1 >> 5) * 4 + -0x1fff1e80) = 1 << (param_1 & 0x1f);
    DataSynchronizationBarrier(0xf);
    InstructionSynchronizationBarrier(0xf);
  }
  return;
}

