// 0x08020540 FUN_08020540 size=42

undefined4 FUN_08020540(void)

{
  int iVar1;
  uint extraout_r3;
  
  FUN_080207a4();
  iVar1 = FUN_08020570();
  if (iVar1 == 1) {
    *DAT_0802056c = *DAT_0802056c & 0xffffffbf | (extraout_r3 & 1) << 6;
    FUN_080205a0();
    FUN_080207b4();
    return 1;
  }
  return 0;
}

