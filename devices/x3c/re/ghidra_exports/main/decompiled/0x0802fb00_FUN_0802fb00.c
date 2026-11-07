// 0x0802fb00 FUN_0802fb00 size=350

void FUN_0802fb00(undefined4 param_1,int *param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  
  local_34 = *param_2;
  local_30 = param_2[1];
  local_2c = *param_2;
  local_28 = param_2[3];
  local_24 = param_2[2];
  local_20 = param_2[1];
  local_1c = param_2[2];
  local_18 = param_2[3];
  FUN_0803178c(param_1,&local_34,param_3,param_4);
  FUN_0803178c(param_1,&local_2c,param_3,param_4);
  FUN_0803178c(param_1,&local_24,param_3,param_4);
  FUN_0803178c(param_1,&local_1c,param_3,param_4);
  if (local_34 < local_2c) {
    iVar1 = local_34;
  }
  else {
    iVar1 = local_2c;
  }
  if (local_24 < local_1c) {
    iVar2 = local_24;
  }
  else {
    iVar2 = local_1c;
  }
  if (iVar1 < iVar2) {
    iVar1 = local_34;
    if (local_2c <= local_34) {
      iVar1 = local_2c;
    }
  }
  else if (local_24 < local_1c) {
    iVar1 = local_24;
  }
  else {
    iVar1 = local_1c;
  }
  *param_2 = iVar1;
  if (local_2c < local_34) {
    iVar1 = local_34;
  }
  else {
    iVar1 = local_2c;
  }
  if (local_1c < local_24) {
    iVar2 = local_24;
  }
  else {
    iVar2 = local_1c;
  }
  if (iVar2 < iVar1) {
    iVar1 = local_34;
    if (local_34 <= local_2c) {
      iVar1 = local_2c;
    }
  }
  else if (local_1c < local_24) {
    iVar1 = local_24;
  }
  else {
    iVar1 = local_1c;
  }
  param_2[2] = iVar1;
  if (local_30 < local_28) {
    iVar1 = local_30;
  }
  else {
    iVar1 = local_28;
  }
  if (local_20 < local_18) {
    iVar2 = local_20;
  }
  else {
    iVar2 = local_18;
  }
  if (iVar1 < iVar2) {
    if (local_30 < local_28) {
      iVar1 = local_30;
    }
    else {
      iVar1 = local_28;
    }
  }
  else if (local_20 < local_18) {
    iVar1 = local_20;
  }
  else {
    iVar1 = local_18;
  }
  param_2[1] = iVar1;
  if (local_28 < local_30) {
    iVar1 = local_30;
  }
  else {
    iVar1 = local_28;
  }
  if (local_18 < local_20) {
    iVar2 = local_20;
  }
  else {
    iVar2 = local_18;
  }
  if (iVar2 < iVar1) {
    if (local_28 < local_30) {
      iVar1 = local_30;
    }
    else {
      iVar1 = local_28;
    }
  }
  else if (local_18 < local_20) {
    iVar1 = local_20;
  }
  else {
    iVar1 = local_18;
  }
  param_2[3] = iVar1;
  FUN_080274e4(param_2,5);
  return;
}

