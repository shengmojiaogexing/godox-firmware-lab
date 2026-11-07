// 0x0802b290 FUN_0802b290 size=176

int FUN_0802b290(undefined4 *param_1,uint param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  code *UNRECOVERED_JUMPTABLE;
  undefined4 local_18;
  
  iVar2 = param_1[1];
  if (iVar2 == 0) {
    return 0xb;
  }
  UNRECOVERED_JUMPTABLE = *(code **)(iVar2 + 0x18);
  if (UNRECOVERED_JUMPTABLE == (code *)0x0) {
    iVar1 = 9;
  }
  else {
    iVar1 = 0;
    local_18 = param_4;
    if (*(short *)(iVar2 + 2) == 0) {
                    /* WARNING: Could not recover jumptable at 0x0802b33e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      iVar2 = (*UNRECOVERED_JUMPTABLE)(iVar2,*param_1,param_2,param_3);
      return iVar2;
    }
    if (param_3 == 0) {
      *(uint *)(param_1[2] + 8) = param_2;
      if ((param_2 < *(uint *)param_1[2]) || (((uint *)param_1[2])[1] < param_2)) {
                    /* WARNING: Could not recover jumptable at 0x0802b300. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        iVar2 = (**(code **)(param_1[1] + 0x18))(param_1[1],*param_1,param_2,0);
        return iVar2;
      }
    }
    else {
      if (param_3 != 1) {
        if (((param_3 == 2) &&
            (iVar1 = (*UNRECOVERED_JUMPTABLE)(iVar2,*param_1,param_2,2), iVar1 == 0)) &&
           (iVar1 = (**(code **)(param_1[1] + 0x1c))(param_1[1],*param_1,&local_18), iVar1 == 0)) {
          *(undefined4 *)(param_1[2] + 8) = local_18;
        }
        return iVar1;
      }
      param_2 = *(int *)(param_1[2] + 8) + param_2;
      *(uint *)(param_1[2] + 8) = param_2;
      if ((param_2 < *(uint *)param_1[2]) || (((uint *)param_1[2])[1] < param_2)) {
                    /* WARNING: Could not recover jumptable at 0x0802b32a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        iVar2 = (**(code **)(param_1[1] + 0x18))(param_1[1],*param_1,param_2,0);
        return iVar2;
      }
    }
  }
  return iVar1;
}

