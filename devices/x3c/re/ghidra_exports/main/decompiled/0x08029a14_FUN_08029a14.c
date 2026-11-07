// 0x08029a14 FUN_08029a14 size=36

void FUN_08029a14(int param_1,undefined4 param_2,int *param_3)

{
  if (((param_3[3] - param_3[1]) + 1 < 1) || ((param_3[2] - *param_3) + 1 < 1)) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x08029a32. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1 + 0x10))();
  return;
}

