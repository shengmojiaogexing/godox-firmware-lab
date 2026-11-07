// 0x0802e35c FUN_0802e35c size=40

void FUN_0802e35c(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (*piVar1 != 0) {
    *param_1 = *piVar1;
    FUN_0802e35c(param_1);
  }
  *param_1 = (int)piVar1;
  if ((code *)piVar1[1] == (code *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0802e380. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)piVar1[1])(piVar1,param_1);
  return;
}

