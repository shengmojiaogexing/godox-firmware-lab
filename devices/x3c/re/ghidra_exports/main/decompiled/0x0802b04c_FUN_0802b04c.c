// 0x0802b04c FUN_0802b04c size=46

undefined * FUN_0802b04c(int param_1)

{
  char cVar1;
  int iVar2;
  
  iVar2 = FUN_08004df8(param_1);
  while( true ) {
    if (iVar2 == 0) {
      return &DAT_0802b07c;
    }
    cVar1 = *(char *)(param_1 + iVar2);
    if (cVar1 == '.') break;
    if ((cVar1 == '/') || (cVar1 == '\\')) {
      return &DAT_0802b07c;
    }
    iVar2 = iVar2 + -1;
  }
  return (undefined *)(iVar2 + param_1 + 1);
}

