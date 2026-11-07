// 0x08003fe2 FUN_08003fe2 size=30

undefined4 FUN_08003fe2(int param_1,uint param_2)

{
  if (((*(uint *)(param_1 + 4) & param_2 >> 0x10) == 0) ||
     ((*(uint *)(param_1 + 8) & param_2 & 0x1ff) == 0)) {
    return 0;
  }
  return 1;
}

