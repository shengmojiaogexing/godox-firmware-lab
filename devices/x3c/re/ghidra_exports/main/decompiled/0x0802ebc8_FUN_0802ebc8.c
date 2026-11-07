// 0x0802ebc8 FUN_0802ebc8 size=44

undefined4 FUN_0802ebc8(int param_1,uint param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 8);
  if (piVar1 == (int *)0x0) {
    return 0;
  }
  if (((int)param_2 < 0) && (param_2 = param_2 + piVar1[1], (int)param_2 < 0)) {
    return 0;
  }
  if (param_2 < (uint)piVar1[1]) {
    return *(undefined4 *)(*piVar1 + param_2 * 4);
  }
  return 0;
}

