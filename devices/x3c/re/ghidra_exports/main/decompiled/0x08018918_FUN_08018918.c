// 0x08018918 FUN_08018918 size=258

int FUN_08018918(char *param_1,int param_2,undefined4 param_3,int param_4,int *param_5,uint param_6)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int local_30;
  int local_2c [2];
  
  if (param_5 != (int *)0x0) {
    *param_5 = 0;
  }
  if (param_1 == (char *)0x0) {
    local_2c[0] = 0;
  }
  else if (*param_1 == '\0') {
    local_2c[0] = 0;
  }
  else if (param_2 == 0) {
    local_2c[0] = 0;
  }
  else {
    iVar4 = 0;
    if ((param_6 & 6) == 0) {
      if ((int)(param_6 << 0x1e) < 0) {
        param_4 = 0x1fffffff;
      }
      local_2c[1] = 0;
      local_2c[0] = 0;
      do {
        if ((param_1[local_2c[0]] == '\0') || (param_4 < 1)) goto LAB_080189e8;
        local_30 = 0;
        iVar2 = FUN_08033934(param_1 + local_2c[0],param_2,param_3,param_4,param_6,&local_30,
                             local_2c + 1,local_2c[0] == 0);
        param_4 = param_4 - local_30;
        iVar4 = iVar4 + local_30;
        if (iVar2 == 0) goto LAB_080189e8;
        local_2c[0] = iVar2 + local_2c[0];
        if ((*param_1 == '\n') || (*param_1 == '\r')) goto LAB_080189e8;
      } while ((param_1[local_2c[0]] != '\n') && (param_1[local_2c[0]] != '\r'));
      local_2c[0] = local_2c[0] + 1;
LAB_080189e8:
      if ((local_2c[0] == 0) &&
         (uVar3 = (**(code **)(DAT_08018a1c + 4))(param_1,local_2c), param_5 != (int *)0x0)) {
        iVar4 = FUN_0802afc2(param_2,uVar3,0);
      }
      if (param_5 != (int *)0x0) {
        *param_5 = iVar4;
      }
    }
    else {
      for (local_2c[0] = 0;
          ((cVar1 = param_1[local_2c[0]], cVar1 != '\n' && (cVar1 != '\r')) && (cVar1 != '\0'));
          local_2c[0] = local_2c[0] + 1) {
      }
      if (param_1[local_2c[0]] != '\0') {
        local_2c[0] = local_2c[0] + 1;
      }
      if (param_5 != (int *)0x0) {
        *param_5 = -1;
      }
    }
  }
  return local_2c[0];
}

