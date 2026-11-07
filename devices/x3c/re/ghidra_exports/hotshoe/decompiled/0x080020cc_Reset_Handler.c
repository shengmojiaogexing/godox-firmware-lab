// 0x080020cc Reset_Handler size=8

void Reset_Handler(void)

{
  (*DAT_080020e0)();
                    /* WARNING: Could not recover jumptable at 0x080020d2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_080020e4)();
  return;
}

