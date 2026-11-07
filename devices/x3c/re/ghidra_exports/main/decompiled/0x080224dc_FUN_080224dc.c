// 0x080224dc FUN_080224dc size=52

undefined4 FUN_080224dc(int param_1,int param_2,int *param_3)

{
  int iVar1;
  
  if (param_2 == 0) {
    iVar1 = *param_3 + 1;
    *param_3 = iVar1;
    if (iVar1 < (*(int **)(param_1 + 8))[1]) {
      return *(undefined4 *)(**(int **)(param_1 + 8) + iVar1 * 4);
    }
    return 0;
  }
  iVar1 = *param_3 + -1;
  *param_3 = iVar1;
  if (iVar1 < 0) {
    return 0;
  }
  return *(undefined4 *)(**(int **)(param_1 + 8) + iVar1 * 4);
}

