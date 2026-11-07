// 0x080038c0 FUN_080038c0 size=44

void FUN_080038c0(int param_1)

{
  uint *puVar1;
  
  puVar1 = DAT_080038ec;
  *DAT_080038ec = *DAT_080038ec & 0xfffeffff;
  *puVar1 = *puVar1 & 0xfffbffff;
  if (param_1 != 1) {
    if (param_1 != 5) {
      return;
    }
    *puVar1 = *puVar1 | 0x40000;
  }
  *puVar1 = *puVar1 | 0x10000;
  return;
}

