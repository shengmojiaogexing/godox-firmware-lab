// 0x0802a3e8 FUN_0802a3e8 size=84

void FUN_0802a3e8(undefined4 *param_1,int param_2,uint param_3)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_08018528();
  if ((param_3 & 1) == 0) {
    *(uint *)(param_2 + 0x3c) = *(uint *)(param_2 + 0x3c) & 0x7fffffff;
    *(uint *)(*piVar1 + 0x1c) = *(uint *)(*piVar1 + 0x1c) & 0xffffffbf;
  }
  else {
    FUN_0802d8d6(*(undefined4 *)(param_2 + 0x28),*(uint *)(param_2 + 0x3c) & 0x7fffffff);
    *(uint *)(param_2 + 0x3c) = *(uint *)(param_2 + 0x3c) | 0x80000000;
    *(uint *)(*piVar1 + 0x1c) = *(uint *)(*piVar1 + 0x1c) | 0x40;
  }
  *param_1 = *(undefined4 *)(param_2 + 0x28);
  param_1[1] = param_2 + 0x10;
  param_1[2] = param_2 + 0x10;
  return;
}

