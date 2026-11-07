// 0x08022204 FUN_08022204 size=88

int FUN_08022204(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 0;
  iVar4 = 0;
  iVar1 = FUN_0802c610(0);
  do {
    if (iVar1 == 0) {
      if (iVar3 == 0) {
        if (iVar4 == 0) {
          iVar4 = FUN_0802c610(0);
          return iVar4;
        }
      }
      else {
        iVar4 = iVar3;
      }
      return iVar4;
    }
    iVar2 = FUN_0802c69e(iVar1);
    if (*(int *)(iVar1 + 0x80) == param_1) {
      if (iVar2 == 2) {
        return iVar1;
      }
      if (iVar2 == 4) {
        iVar3 = iVar1;
      }
      iVar4 = iVar1;
    }
    iVar1 = FUN_0802c610(iVar1);
  } while( true );
}

