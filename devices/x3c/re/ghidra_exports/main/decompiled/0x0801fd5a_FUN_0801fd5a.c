// 0x0801fd5a FUN_0801fd5a size=386

int FUN_0801fd5a(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar1 = FUN_0802fc9e(param_1,0x20,param_3,param_4,param_1,param_2,param_3,param_4);
  if (iVar1 == 0) {
    if (param_4 + param_2 < 0) {
      param_2 = -param_4;
    }
    if (param_3 - param_2 < 0) {
      param_2 = param_3;
    }
  }
  else {
    if (param_5 == 3) {
      iVar1 = FUN_0802f094(param_1);
    }
    else {
      iVar1 = FUN_0802f0a8(param_1);
    }
    iVar2 = FUN_0802c628();
    iVar5 = 0;
    iVar6 = 0;
    if (param_5 == 3) {
      iVar3 = FUN_0802f932(param_1,0);
      iVar4 = FUN_0802f95c(param_1,0);
      if (iVar1 == 1) {
        iVar5 = *(int *)(param_1 + 0x14) + iVar3;
        iVar6 = *(int *)(iVar2 + 0x14);
      }
      else if (iVar1 == 2) {
        iVar5 = *(int *)(param_1 + 0x1c) - iVar4;
        iVar6 = *(int *)(iVar2 + 0x1c);
      }
      else if (iVar1 == 3) {
        iVar5 = iVar3 + ((((*(int *)(param_1 + 0x1c) - *(int *)(param_1 + 0x14)) + 1) - iVar3) -
                        iVar4) / 2 + *(int *)(param_1 + 0x14);
        iVar6 = *(int *)(iVar2 + 0x14) + ((*(int *)(iVar2 + 0x1c) - *(int *)(iVar2 + 0x14)) + 1) / 2
        ;
      }
    }
    else {
      iVar3 = FUN_0802f992(param_1,0);
      iVar4 = FUN_0802f8fc(param_1,0);
      if (iVar1 == 1) {
        iVar5 = *(int *)(param_1 + 0x18) + iVar3;
        iVar6 = *(int *)(iVar2 + 0x18);
      }
      else if (iVar1 == 2) {
        iVar5 = *(int *)(param_1 + 0x20) - iVar4;
        iVar6 = *(int *)(iVar2 + 0x20);
      }
      else if (iVar1 == 3) {
        iVar5 = iVar3 + ((((*(int *)(param_1 + 0x20) - *(int *)(param_1 + 0x18)) + 1) - iVar3) -
                        iVar4) / 2 + *(int *)(param_1 + 0x18);
        iVar6 = *(int *)(iVar2 + 0x18) + ((*(int *)(iVar2 + 0x20) - *(int *)(iVar2 + 0x18)) + 1) / 2
        ;
      }
    }
    if (param_4 < 0) {
      if ((iVar1 != 0) && (iVar5 < iVar6)) {
        return param_2;
      }
      if (param_2 < 0) {
        param_2 = param_2 + -2;
      }
      if (0 < param_2) {
        param_2 = param_2 + 2;
      }
      return (int)(param_2 + ((uint)(param_2 >> 0x1f) >> 0x1e)) >> 2;
    }
    if (param_3 < 0) {
      if ((iVar1 != 0) && (iVar6 < iVar5)) {
        return param_2;
      }
      if (param_2 < 0) {
        param_2 = param_2 + -2;
      }
      if (0 < param_2) {
        param_2 = param_2 + 2;
      }
      return (int)(param_2 + ((uint)(param_2 >> 0x1f) >> 0x1e)) >> 2;
    }
  }
  return param_2;
}

