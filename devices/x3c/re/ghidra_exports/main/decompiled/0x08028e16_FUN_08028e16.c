// 0x08028e16 FUN_08028e16 size=26

void FUN_08028e16(int param_1,int param_2)

{
  if ((*(int *)(param_2 + 4) == 0) || (*(byte *)(param_2 + 0x10) < 3)) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x08028e2a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1 + 0x24))();
  return;
}

