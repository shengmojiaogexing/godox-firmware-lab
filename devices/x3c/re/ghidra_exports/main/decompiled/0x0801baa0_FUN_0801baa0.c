// 0x0801baa0 FUN_0801baa0 size=26

void FUN_0801baa0(int param_1)

{
  if (param_1 == 1) {
    *DAT_0801babc = *DAT_0801babc | 0x30;
    return;
  }
  *DAT_0801babc = *DAT_0801babc & 0xffffffcf;
  return;
}

