// 0x08033a84 FUN_08033a84 size=188

void FUN_08033a84(int *param_1,int param_2,int param_3,undefined4 param_4,int param_5,
                 undefined4 param_6,int param_7)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  
  *param_1 = 0;
  param_1[1] = 0;
  if ((param_2 != 0) && (param_3 != 0)) {
    if (param_7 << 0x1e < 0) {
      param_6 = 0x1fffffff;
    }
    uVar5 = (uint)*(ushort *)(param_3 + 8);
    iVar4 = 0;
    while (*(char *)(param_2 + iVar4) != '\0') {
      iVar2 = FUN_08018918(param_2 + iVar4,param_3,param_4,param_6,0,param_7);
      if (0x7fffffff < uVar5 + param_5 + param_1[1]) {
        return;
      }
      param_1[1] = param_1[1] + uVar5 + param_5;
      iVar3 = FUN_08033b40(param_2 + iVar4,(iVar4 + iVar2) - iVar4,param_3,param_4,param_7);
      if (iVar3 <= *param_1) {
        iVar3 = *param_1;
      }
      *param_1 = iVar3;
      iVar4 = iVar4 + iVar2;
    }
    if ((iVar4 != 0) &&
       ((cVar1 = *(char *)(param_2 + iVar4 + -1), cVar1 == '\n' || (cVar1 == '\r')))) {
      param_1[1] = param_1[1] + uVar5 + param_5;
    }
    if (param_1[1] == 0) {
      param_1[1] = uVar5;
    }
    else {
      param_1[1] = param_1[1] - param_5;
    }
  }
  return;
}

