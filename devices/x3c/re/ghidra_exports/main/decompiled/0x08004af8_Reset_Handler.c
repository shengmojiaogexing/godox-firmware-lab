// 0x08004af8 Reset_Handler size=34

/* WARNING: This function may have set the stack pointer */

void Reset_Handler(void)

{
  (*DAT_08004b3c)();
  (*DAT_08004b40)();
                    /* WARNING: Could not recover jumptable at 0x08004b18. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_08004b44)();
  return;
}

