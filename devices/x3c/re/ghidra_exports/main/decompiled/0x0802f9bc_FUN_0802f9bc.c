// 0x0802f9bc FUN_0802f9bc size=144

undefined4 FUN_0802f9bc(undefined4 *param_1,int param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  undefined4 local_20;
  
  local_20 = param_4;
  iVar1 = FUN_08032d50(param_3,1);
  iVar2 = 0;
  while (((param_1 != (undefined4 *)0x0 &&
          (iVar2 = FUN_08022536(param_1,param_2,param_3,&local_20), iVar2 != 1)) && (iVar1 != 0))) {
    if ((iVar2 == 2) || (param_2 == 0)) {
      param_1 = (undefined4 *)FUN_0802ee0a(param_1);
    }
    else {
      param_2 = 0;
    }
  }
  if (iVar2 != 1) {
    if ((param_2 == 0) && ((param_3 == 1 || (param_3 == 4)))) {
      for (param_1 = (undefined4 *)*param_1; param_1 != (undefined4 *)0x0;
          param_1 = (undefined4 *)*param_1) {
        if (param_3 == 1) {
          iVar1 = param_1[5];
        }
        else {
          iVar1 = param_1[6];
        }
        if (iVar1 != 0) break;
      }
      if (param_1 == (undefined4 *)0x0) {
        local_20 = 0;
      }
      else if (param_3 == 1) {
        local_20 = param_1[5];
      }
      else {
        local_20 = param_1[6];
      }
    }
    else {
      local_20 = FUN_08032c68(param_3);
    }
  }
  return local_20;
}

