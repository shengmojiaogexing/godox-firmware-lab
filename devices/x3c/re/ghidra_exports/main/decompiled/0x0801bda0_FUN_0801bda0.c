// 0x0801bda0 FUN_0801bda0 size=74

void FUN_0801bda0(void)

{
  uint *puVar1;
  uint *puVar2;
  
  puVar1 = DAT_0801bdec;
  *DAT_0801bdec = *DAT_0801bdec | 1;
  do {
  } while (-1 < (int)(*puVar1 << 0x1e));
  puVar2 = DAT_0801bdec + 2;
  *puVar2 = *puVar2 & 0xfffffffc;
  do {
  } while ((*puVar2 & 0xc) != 0);
  *puVar1 = *puVar1 & DAT_0801bdf0;
  *puVar2 = 0;
  DAT_0801bdec[1] = DAT_0801bdf4;
  DAT_0801bdec[0x28] = 0;
  DAT_0801bdec[3] = 0x9f0000;
  return;
}

