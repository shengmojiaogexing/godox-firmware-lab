// 0x0801c85e FUN_0801c85e size=222

void FUN_0801c85e(undefined4 param_1,int *param_2,int *param_3)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  
  if ((((2 < *(byte *)(param_2 + 10)) && (param_2[9] != 0)) &&
      ((*(byte *)((int)param_2 + 0x29) & 0x3e) != 0)) && ((*(byte *)((int)param_2 + 0x29) & 1) == 0)
     ) {
    iVar1 = (param_3[2] - *param_3) + 1;
    iVar3 = (param_3[3] - param_3[1]) + 1;
    if (iVar3 <= iVar1) {
      iVar1 = iVar3;
    }
    iVar3 = *param_2;
    if (iVar1 >> 1 < *param_2) {
      iVar3 = iVar1 >> 1;
    }
    FUN_0802748a(&local_24,param_3);
    uVar2 = (*(byte *)((int)param_2 + 0x29) & 0x3f) >> 1;
    if ((int)(uVar2 << 0x1d) < 0) {
      iVar1 = param_2[9];
    }
    else {
      iVar1 = -(param_2[9] + iVar3);
    }
    local_24 = iVar1 + local_24;
    if ((int)(uVar2 << 0x1c) < 0) {
      iVar1 = param_2[9];
    }
    else {
      iVar1 = -(param_2[9] + iVar3);
    }
    local_1c = local_1c - iVar1;
    if ((int)((uint)(*(byte *)((int)param_2 + 0x29) >> 1) << 0x1e) < 0) {
      iVar1 = param_2[9];
    }
    else {
      iVar1 = -(param_2[9] + iVar3);
    }
    local_20 = iVar1 + local_20;
    if ((*(byte *)((int)param_2 + 0x29) >> 1 & 1) == 0) {
      iVar1 = -(param_2[9] + iVar3);
    }
    else {
      iVar1 = param_2[9];
    }
    local_18 = local_18 - iVar1;
    iVar1 = iVar3 - param_2[9];
    if (iVar1 < 0) {
      iVar1 = 0;
    }
    FUN_0801c93c(param_1,param_3,&local_24,iVar3,iVar1,*(undefined2 *)((int)param_2 + 0x22),
                 (char)param_2[10],(char)param_2[1]);
  }
  return;
}

