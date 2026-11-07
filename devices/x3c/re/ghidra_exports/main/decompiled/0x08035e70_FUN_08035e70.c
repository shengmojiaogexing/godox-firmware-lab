// 0x08035e70 FUN_08035e70 size=58

undefined4 FUN_08035e70(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = 0;
  if (*(int *)(param_1 + 8) != 0) {
    uVar3 = *(uint *)(*(int *)(param_1 + 8) + 4);
  }
  uVar2 = 0;
  while( true ) {
    if (uVar3 <= uVar2) {
      return 0;
    }
    iVar1 = *(int *)(**(int **)(param_1 + 8) + uVar2 * 4);
    if (iVar1 == param_2) break;
    iVar1 = FUN_08035e70(iVar1,param_2);
    if (iVar1 != 0) {
      return 1;
    }
    uVar2 = uVar2 + 1;
  }
  return 1;
}

