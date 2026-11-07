// 0x08028d5e FUN_08028d5e size=10

void FUN_08028d5e(int param_1)

{
  if (*(code **)(param_1 + 0x44) == (code *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x08028d64. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1 + 0x44))();
  return;
}

