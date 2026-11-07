// 0x08018a5a FUN_08018a5a size=70

int FUN_08018a5a(undefined4 param_1,int param_2,uint param_3,int param_4,code *param_5)

{
  int iVar1;
  int iVar2;
  
  while( true ) {
    while( true ) {
      if (param_3 == 0) {
        return 0;
      }
      iVar2 = (param_3 >> 1) * param_4 + param_2;
      iVar1 = (*param_5)(param_1,iVar2);
      if (iVar1 < 1) break;
      param_2 = iVar2 + param_4;
      param_3 = ((param_3 >> 1) - ((int)(param_3 << 0x1f) >> 0x1f)) - 1;
    }
    if (-1 < iVar1) break;
    param_3 = param_3 >> 1;
  }
  return iVar2;
}

