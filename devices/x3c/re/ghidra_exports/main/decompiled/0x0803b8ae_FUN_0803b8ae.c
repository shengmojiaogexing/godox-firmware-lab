// 0x0803b8ae FUN_0803b8ae size=52

void FUN_0803b8ae(int *param_1)

{
  *(uint *)(*param_1 + 0x804) = *(uint *)(*param_1 + 0x804) & 0xfffffffe;
  FUN_0803a844(*param_1);
  *(undefined1 *)(param_1 + 0xb9) = *(undefined1 *)((int)param_1 + 0x2e5);
  if (*(code **)(param_1[1] + 0x20) == (code *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0803b8de. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1[1] + 0x20))(param_1,3);
  return;
}

