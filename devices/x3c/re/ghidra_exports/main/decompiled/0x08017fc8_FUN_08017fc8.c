// 0x08017fc8 FUN_08017fc8 size=60

int FUN_08017fc8(int *param_1)

{
  int iVar1;
  
  iVar1 = FUN_0802d5c4(*param_1 + 8);
  if (iVar1 != 0) {
    FUN_08035c34(param_1,iVar1,0);
    FUN_08035c46(param_1,iVar1,param_1[2]);
    if (param_1[2] != 0) {
      FUN_08035c34(param_1,param_1[2],iVar1);
    }
    param_1[2] = iVar1;
    if (param_1[1] == 0) {
      param_1[1] = iVar1;
    }
  }
  return iVar1;
}

