// 0x08017586 FUN_08017586 size=148

undefined4 FUN_08017586(int *param_1,int *param_2,int param_3)

{
  int iVar1;
  int local_18;
  int local_14;
  
  if ((((param_1[2] < *param_2) || (param_1[3] < param_2[1])) ||
      (local_18 = *param_1, param_2[2] < local_18)) || (param_2[3] < param_1[1])) {
    return 1;
  }
  if (param_3 == 0) {
    return 0;
  }
  local_14 = param_1[1];
  iVar1 = FUN_0801761a(param_2,&local_18,param_3);
  if (iVar1 != 0) {
    return 0;
  }
  local_18 = param_1[2];
  local_14 = param_1[1];
  iVar1 = FUN_0801761a(param_2,&local_18,param_3);
  if (iVar1 != 0) {
    return 0;
  }
  local_18 = *param_1;
  local_14 = param_1[3];
  iVar1 = FUN_0801761a(param_2,&local_18,param_3);
  if (iVar1 != 0) {
    return 0;
  }
  local_18 = param_1[2];
  local_14 = param_1[3];
  iVar1 = FUN_0801761a(param_2,&local_18,param_3);
  if (iVar1 == 0) {
    return 1;
  }
  return 0;
}

