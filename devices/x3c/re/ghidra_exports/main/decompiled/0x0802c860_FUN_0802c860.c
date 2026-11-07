// 0x0802c860 FUN_0802c860 size=58

int FUN_0802c860(int *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  if (param_1 == (int *)0x0) {
    return 0;
  }
  if (param_2 == 3) {
    iVar2 = param_1[0x10];
  }
  else {
    if (param_2 != 0xc) {
      return 0;
    }
    iVar2 = param_1[0x11];
  }
  iVar1 = 0;
  for (; iVar2 != 0; iVar2 = (int)((100 - (uint)*(byte *)(*param_1 + 0x19)) * iVar2) / 100) {
    iVar1 = iVar1 + iVar2;
  }
  return iVar1;
}

