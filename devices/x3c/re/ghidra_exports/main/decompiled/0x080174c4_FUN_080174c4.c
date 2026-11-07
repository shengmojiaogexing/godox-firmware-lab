// 0x080174c4 FUN_080174c4 size=154

undefined4 FUN_080174c4(int *param_1,int *param_2,int param_3)

{
  bool bVar1;
  int iVar2;
  int local_18;
  int local_14;
  
  bVar1 = false;
  local_18 = *param_1;
  if ((((*param_2 <= local_18) && (param_2[1] <= param_1[1])) && (param_1[2] <= param_2[2])) &&
     (param_1[3] <= param_2[3])) {
    bVar1 = true;
  }
  if (!bVar1) {
    return 0;
  }
  if (param_3 == 0) {
    return 1;
  }
  local_14 = param_1[1];
  iVar2 = FUN_0801761a(param_2,&local_18,param_3);
  if (iVar2 == 0) {
    return 0;
  }
  local_18 = param_1[2];
  local_14 = param_1[1];
  iVar2 = FUN_0801761a(param_2,&local_18,param_3);
  if (iVar2 == 0) {
    return 0;
  }
  local_18 = *param_1;
  local_14 = param_1[3];
  iVar2 = FUN_0801761a(param_2,&local_18,param_3);
  if (iVar2 == 0) {
    return 0;
  }
  local_18 = param_1[2];
  local_14 = param_1[3];
  iVar2 = FUN_0801761a(param_2,&local_18,param_3);
  if (iVar2 == 0) {
    return 0;
  }
  return 1;
}

