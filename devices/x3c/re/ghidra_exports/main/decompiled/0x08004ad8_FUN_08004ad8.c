// 0x08004ad8 FUN_08004ad8 size=32

/* WARNING: This function may have set the stack pointer */

void FUN_08004ad8(void)

{
  undefined4 uVar1;
  undefined4 extraout_r2;
  undefined8 uVar2;
  
  uVar1 = FUN_080050a6();
  FUN_08004ad0(uVar1,extraout_r2);
  FUN_08033e20();
  uVar2 = FUN_080050f0();
  FUN_08004ad4();
  FUN_08005098((int)uVar2,(int)((ulonglong)uVar2 >> 0x20));
  (*DAT_08004b3c)();
  (*DAT_08004b40)();
                    /* WARNING: Could not recover jumptable at 0x08004b18. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_08004b44)();
  return;
}

