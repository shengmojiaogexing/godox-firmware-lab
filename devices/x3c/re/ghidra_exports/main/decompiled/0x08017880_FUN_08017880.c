// 0x08017880 FUN_08017880 size=378

void FUN_08017880(int *param_1,int param_2,int param_3,int param_4,int param_5,undefined4 param_6)

{
  int iVar1;
  int iVar2;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  
  if ((param_4 == 0) && (param_5 == 0x100)) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = param_2 + -1;
    param_1[3] = param_3 + -1;
  }
  else {
    FUN_08004e9a(&local_3c,0x20);
    local_34 = param_2;
    local_28 = param_3;
    local_24 = param_2;
    local_20 = param_3;
    FUN_080318c8(&local_3c,param_4,param_5,param_6);
    FUN_080318c8(&local_34,param_4,param_5,param_6);
    FUN_080318c8(&local_2c,param_4,param_5,param_6);
    FUN_080318c8(&local_24,param_4,param_5,param_6);
    if (local_3c < local_34) {
      iVar1 = local_3c;
    }
    else {
      iVar1 = local_34;
    }
    if (local_2c < local_24) {
      iVar2 = local_2c;
    }
    else {
      iVar2 = local_24;
    }
    if (iVar1 < iVar2) {
      if (local_3c < local_34) {
        iVar1 = local_3c;
      }
      else {
        iVar1 = local_34;
      }
    }
    else if (local_2c < local_24) {
      iVar1 = local_2c;
    }
    else {
      iVar1 = local_24;
    }
    *param_1 = iVar1 + -2;
    if (local_34 < local_3c) {
      iVar1 = local_3c;
    }
    else {
      iVar1 = local_34;
    }
    if (local_24 < local_2c) {
      iVar2 = local_2c;
    }
    else {
      iVar2 = local_24;
    }
    if (iVar2 < iVar1) {
      if (local_3c <= local_34) {
        local_3c = local_34;
      }
    }
    else if (local_24 < local_2c) {
      local_3c = local_2c;
    }
    else {
      local_3c = local_24;
    }
    param_1[2] = local_3c + 2;
    if (local_38 < local_30) {
      iVar1 = local_38;
    }
    else {
      iVar1 = local_30;
    }
    if (local_28 < local_20) {
      iVar2 = local_28;
    }
    else {
      iVar2 = local_20;
    }
    if (iVar1 < iVar2) {
      if (local_38 < local_30) {
        iVar1 = local_38;
      }
      else {
        iVar1 = local_30;
      }
    }
    else if (local_28 < local_20) {
      iVar1 = local_28;
    }
    else {
      iVar1 = local_20;
    }
    param_1[1] = iVar1 + -2;
    if (local_30 < local_38) {
      iVar1 = local_38;
    }
    else {
      iVar1 = local_30;
    }
    if (local_20 < local_28) {
      iVar2 = local_28;
    }
    else {
      iVar2 = local_20;
    }
    if (iVar2 < iVar1) {
      if (local_38 <= local_30) {
        local_38 = local_30;
      }
    }
    else if (local_20 < local_28) {
      local_38 = local_28;
    }
    else {
      local_38 = local_20;
    }
    param_1[3] = local_38 + 2;
  }
  return;
}

