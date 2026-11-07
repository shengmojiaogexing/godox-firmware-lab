// 0x08006fde FUN_08006fde size=20

undefined4 FUN_08006fde(int param_1,uint param_2)

{
  if (((*(uint *)(param_1 + 0x10) & param_2) == 0) || ((*(uint *)(param_1 + 0xc) & param_2) == 0)) {
    return 0;
  }
  return 1;
}

