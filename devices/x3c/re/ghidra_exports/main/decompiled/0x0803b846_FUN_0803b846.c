// 0x0803b846 FUN_0803b846 size=54

void FUN_0803b846(int *param_1)

{
  if ((*(uint *)(*param_1 + 0x808) & 1) != 0) {
    *(char *)((int)param_1 + 0x2e5) = (char)param_1[0xb9];
    *(undefined1 *)(param_1 + 0xb9) = 4;
    FUN_0803ae84(param_1);
    if (*(code **)(param_1[1] + 0x20) != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0803b878. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(param_1[1] + 0x20))(param_1,2);
      return;
    }
  }
  return;
}

