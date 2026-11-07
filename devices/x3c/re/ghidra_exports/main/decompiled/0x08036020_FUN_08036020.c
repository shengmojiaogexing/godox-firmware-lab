// 0x08036020 FUN_08036020 size=38

void FUN_08036020(int param_1)

{
  uint *puVar1;
  
  puVar1 = DAT_08036048;
  *DAT_08036048 = *DAT_08036048 | 4;
  if (param_1 == 1) {
    WaitForEvent();
    WaitForEvent();
  }
  else if (param_1 == 0) {
    WaitForInterrupt();
  }
  *puVar1 = *puVar1 & 0xfffffffb;
  return;
}

