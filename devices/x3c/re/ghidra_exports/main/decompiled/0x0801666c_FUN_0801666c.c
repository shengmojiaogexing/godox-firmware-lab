// 0x0801666c FUN_0801666c size=80

void FUN_0801666c(void)

{
  uint *puVar1;
  uint *puVar2;
  
  puVar2 = DAT_080166bc;
  *DAT_080166bc = *DAT_080166bc | 1;
  do {
  } while (-1 < (int)(*puVar2 << 0x1e));
  puVar1 = DAT_080166bc + 2;
  *puVar1 = *puVar1 & 0xfffffffc;
  do {
  } while ((*puVar1 & 0xc) != 0);
  *puVar2 = *puVar2 & DAT_080166c0;
  *puVar1 = 0;
  puVar2 = DAT_080166bc + 1;
  *puVar2 = DAT_080166c4;
  DAT_080166bc[0x28] = 0;
  DAT_080166bc[3] = 0x9f0000;
  *DAT_080166c8 = (int)puVar2 * 0x2000000;
  return;
}

