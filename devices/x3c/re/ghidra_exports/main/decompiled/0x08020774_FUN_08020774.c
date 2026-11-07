// 0x08020774 FUN_08020774 size=42

undefined4 FUN_08020774(void)

{
  uint *puVar1;
  int iVar2;
  
  puVar1 = DAT_080207a0;
  iVar2 = 0x6c0000;
  *DAT_080207a0 = *DAT_080207a0 & 0x80 | 0xffffff5f;
  while( true ) {
    if ((int)(*puVar1 << 0x1a) < 0) {
      return 1;
    }
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
  }
  return 0;
}

