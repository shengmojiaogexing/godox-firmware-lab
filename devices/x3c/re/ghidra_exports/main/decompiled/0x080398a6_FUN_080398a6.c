// 0x080398a6 FUN_080398a6 size=20

int FUN_080398a6(int param_1)

{
  int iVar1;
  
  if (param_1 == 0) {
    iVar1 = 0;
  }
  else {
    iVar1 = 0x20 - LZCOUNT(param_1);
  }
  return iVar1 + -1;
}

