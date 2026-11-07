// 0x08020570 FUN_08020570 size=44

undefined4 FUN_08020570(void)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = DAT_0802059c;
  iVar2 = 0x6c0000;
  if (-1 < *DAT_0802059c << 0x19) {
    *DAT_0802059c = -1;
    while (-1 < *piVar1 << 0x19) {
      if (iVar2 == 0) {
        FUN_080207b4();
        return 0;
      }
      iVar2 = iVar2 + -1;
    }
  }
  return 1;
}

