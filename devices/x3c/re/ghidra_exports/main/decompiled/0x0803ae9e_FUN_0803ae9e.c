// 0x0803ae9e FUN_0803ae9e size=48

void FUN_0803ae9e(int *param_1)

{
  int iVar1;
  
  iVar1 = *param_1;
  FUN_0803a368(iVar1);
  *(uint *)(iVar1 + 0xc) = (*(uint *)(iVar1 + 0xc) & 0xffffc3ff) + 0x1400;
  *(undefined1 *)(param_1 + 0xb9) = 1;
  if (*(code **)(param_1[1] + 4) == (code *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0803aeca. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1[1] + 4))(param_1);
  return;
}

