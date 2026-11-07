// 0x080050f0 FUN_080050f0 size=18

/* WARNING: This function may have set the stack pointer */

void FUN_080050f0(undefined4 param_1,undefined4 param_2)

{
  FUN_08004ad4();
  FUN_08005098(param_1,param_2);
  (*DAT_08004b3c)();
  (*DAT_08004b40)();
                    /* WARNING: Could not recover jumptable at 0x08004b18. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_08004b44)();
  return;
}

