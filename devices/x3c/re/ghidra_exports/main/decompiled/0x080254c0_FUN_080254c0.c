// 0x080254c0 FUN_080254c0 size=60

undefined4 FUN_080254c0(code *param_1,undefined4 param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  
  for (iVar1 = FUN_08035c00(0); (iVar1 != 0 && ((*(uint *)(iVar1 + 4) & 0x3fffffff) != 0));
      iVar1 = FUN_08035c00(iVar1)) {
    iVar2 = (*param_1)(iVar1,param_2);
    if (iVar2 == 1) {
      if (param_3 != (int *)0x0) {
        *param_3 = iVar1;
      }
      return 1;
    }
  }
  return 0;
}

