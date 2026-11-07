// 0x0803a684 FUN_0803a684 size=38

void FUN_0803a684(int param_1,uint param_2)

{
  bool bVar1;
  uint uVar2;
  
  *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xfffff83f | (param_2 & 0x1f) << 6;
  *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) | 0x20;
  uVar2 = 0;
  do {
    if (-1 < *(int *)(param_1 + 0x10) << 0x1a) {
      return;
    }
    bVar1 = uVar2 <= DAT_0803a6ac;
    uVar2 = uVar2 + 1;
  } while (bVar1);
  return;
}

