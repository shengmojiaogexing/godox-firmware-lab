// 0x0801ea78 FUN_0801ea78 size=164

void FUN_0801ea78(undefined4 param_1,int *param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  if ((2 < *(byte *)(param_2 + 0xd)) && (param_2[0xb] != 0)) {
    FUN_0802748a(&local_1c,param_3);
    iVar1 = param_2[0xc] + -1;
    local_1c = local_1c - iVar1;
    local_18 = local_18 - iVar1;
    local_14 = local_14 + iVar1;
    local_10 = local_10 + iVar1;
    FUN_0802748a(&local_2c,&local_1c);
    local_2c = local_2c - param_2[0xb];
    local_24 = param_2[0xb] + local_24;
    local_28 = local_28 - param_2[0xb];
    local_20 = local_20 + param_2[0xb];
    iVar1 = (local_14 - local_1c) + 1;
    iVar2 = (local_10 - local_18) + 1;
    if (iVar2 <= iVar1) {
      iVar1 = iVar2;
    }
    iVar2 = *param_2;
    if (iVar1 >> 1 < *param_2) {
      iVar2 = iVar1 >> 1;
    }
    FUN_0801c93c(param_1,&local_2c,&local_1c,param_2[0xb] + iVar2,iVar2,
                 *(undefined2 *)((int)param_2 + 0x2a),(char)param_2[0xd],(char)param_2[1]);
  }
  return;
}

