// 0x0803abb8 FUN_0803abb8 size=116

void FUN_0803abb8(int param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  code *UNRECOVERED_JUMPTABLE;
  
  iVar2 = param_1 + (param_2 & 0x7f) * 0x2c;
  if (param_2 == 0) {
    if (*(char *)(param_1 + 0x2e0) == '\x03') {
      if (*(uint *)(iVar2 + 0x174) < *(uint *)(iVar2 + 400)) {
        uVar1 = *(uint *)(iVar2 + 400) - *(uint *)(iVar2 + 0x174);
        *(uint *)(iVar2 + 400) = uVar1;
        if (*(uint *)(iVar2 + 0x174) <= uVar1) {
          uVar1 = *(uint *)(iVar2 + 0x174);
        }
        FUN_0803af24(param_1,0,*(undefined4 *)(iVar2 + 0x180),uVar1 & 0xffff);
        return;
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*(int *)(param_1 + 4) + 0x10);
      if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
        (*UNRECOVERED_JUMPTABLE)(param_1);
      }
      FUN_0803acbc(param_1);
      return;
    }
  }
  else {
    UNRECOVERED_JUMPTABLE = *(code **)(*(int *)(param_1 + 4) + 0x18);
    if ((UNRECOVERED_JUMPTABLE != (code *)0x0) && (*(char *)(param_1 + 0x2e4) == '\x03')) {
                    /* WARNING: Could not recover jumptable at 0x0803abea. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*UNRECOVERED_JUMPTABLE)(param_1);
      return;
    }
  }
  return;
}

