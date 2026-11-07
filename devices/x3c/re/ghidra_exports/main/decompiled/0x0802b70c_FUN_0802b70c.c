// 0x0802b70c FUN_0802b70c size=36

void FUN_0802b70c(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_08021d48(param_1,DAT_0802b734,DAT_0802b730);
  if ((*(code **)(param_1 + 0x14) == (code *)0x0) || (iVar1 != 0)) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0802b72c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1 + 0x14))(param_1,0);
  return;
}

