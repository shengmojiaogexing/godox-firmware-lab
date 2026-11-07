// 0x08033b40 FUN_08033b40 size=120

int FUN_08033b40(char *param_1,uint param_2,int param_3,int param_4,uint param_5)

{
  int iVar1;
  int iVar2;
  char *local_30;
  uint local_2c [3];
  
  if (param_1 == (char *)0x0) {
    iVar2 = 0;
  }
  else if (param_3 == 0) {
    iVar2 = 0;
  }
  else if (*param_1 == '\0') {
    iVar2 = 0;
  }
  else {
    local_2c[2] = 0;
    iVar2 = 0;
    local_2c[1] = 0;
    local_30 = param_1;
    local_2c[0] = param_2;
    if (param_2 != 0) {
      while (local_2c[2] < param_2) {
        FUN_080188e4(param_1,local_2c,&local_30,local_2c + 2);
        if ((((param_5 & 1) == 0) || (iVar1 = FUN_08018a20(local_2c + 1,local_2c[0]), iVar1 == 0))
           && (iVar1 = FUN_0802afc2(param_3,local_2c[0],local_30), 0 < iVar1)) {
          iVar2 = iVar1 + iVar2 + param_4;
        }
      }
      if (0 < iVar2) {
        iVar2 = iVar2 - param_4;
      }
    }
  }
  return iVar2;
}

