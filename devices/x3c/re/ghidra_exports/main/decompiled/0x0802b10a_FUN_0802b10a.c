// 0x0802b10a FUN_0802b10a size=82

undefined4
FUN_0802b10a(undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 *param_4)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_1c;
  
  if (param_4 != (undefined4 *)0x0) {
    *param_4 = 0;
  }
  if (param_1[1] == 0) {
    uVar1 = 0xb;
  }
  else if (*(int *)(param_1[1] + 0x10) == 0) {
    uVar1 = 9;
  }
  else {
    local_1c = 0;
    iVar2 = param_1[1];
    if (*(short *)(iVar2 + 2) == 0) {
      uVar1 = (**(code **)(iVar2 + 0x10))(iVar2,*param_1,param_2,param_3,&local_1c);
    }
    else {
      uVar1 = FUN_0802b15c(param_1,param_2,param_3,&local_1c,param_3);
    }
    if (param_4 != (undefined4 *)0x0) {
      *param_4 = local_1c;
    }
  }
  return uVar1;
}

