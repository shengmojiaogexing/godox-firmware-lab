// 0x0802c89a FUN_0802c89a size=120

int FUN_0802c89a(int param_1,undefined4 *param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 local_18;
  undefined4 uStack_14;
  
  local_18 = param_3;
  uStack_14 = param_4;
  iVar1 = FUN_0802fc9e(param_1,1);
  if (iVar1 == 0) {
    local_18 = *param_2;
    uStack_14 = param_2[1];
    FUN_0803178c(param_1,&local_18,0,1);
    iVar1 = FUN_0802fcc0(param_1,&local_18);
    iVar3 = FUN_0801761a(param_1 + 0x14,&local_18,0);
    if ((iVar3 == 0) && (iVar3 = FUN_0802fc9e(param_1,0x80000), iVar3 == 0)) {
LAB_0802c908:
      if (iVar1 == 0) {
        return 0;
      }
      return param_1;
    }
    iVar3 = FUN_0802ebf4(param_1);
    do {
      iVar3 = iVar3 + -1;
      if (iVar3 < 0) goto LAB_0802c908;
      iVar2 = FUN_0802c89a(*(undefined4 *)(**(int **)(param_1 + 8) + iVar3 * 4),&local_18);
    } while (iVar2 == 0);
  }
  else {
    iVar2 = 0;
  }
  return iVar2;
}

