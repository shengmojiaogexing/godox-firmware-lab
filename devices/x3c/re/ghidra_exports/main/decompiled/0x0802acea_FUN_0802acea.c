// 0x0802acea FUN_0802acea size=10

void FUN_0802acea(int param_1)

{
  if (*(code **)(param_1 + 0x34) == (code *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0802acf0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1 + 0x34))();
  return;
}

