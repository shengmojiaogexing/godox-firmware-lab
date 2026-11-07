// 0x08019138 FUN_08019138 size=36

void FUN_08019138(undefined4 *param_1,undefined4 param_2)

{
  if (param_1[1] != 0) {
    FUN_08019138(param_1[1],param_2);
  }
  if ((code *)*param_1 == (code *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x08019158. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)*param_1)(param_1,param_2);
  return;
}

