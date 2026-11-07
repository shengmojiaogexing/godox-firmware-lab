// 0x0801bc08 FUN_0801bc08 size=110

void FUN_0801bc08(int param_1)

{
  uint *puVar1;
  
  if (param_1 != DAT_0801bc80) {
    if (param_1 < DAT_0801bc80) {
      switch(param_1) {
      case 0xc0000:
        *DAT_0801bc84 = *DAT_0801bc84 | 0x10000;
        return;
      case 0xc0001:
        *DAT_0801bc84 = *DAT_0801bc84 | 0x20000;
        return;
      case 0xc0002:
        *DAT_0801bc84 = *DAT_0801bc84 | 0x40000;
        return;
      case 0xc0003:
        *DAT_0801bc84 = *DAT_0801bc84 | 0x80000;
        return;
      case 0xc0004:
        *DAT_0801bc84 = *DAT_0801bc84 | 0x100000;
        return;
      default:
        return;
      case 0xc0007:
        *DAT_0801bc84 = *DAT_0801bc84 | 0x800000;
        return;
      }
    }
    if (5 < (param_1 - DAT_0801bc80) - 2U) {
      return;
    }
  }
  puVar1 = DAT_0801bc84 + 0x1a;
  *puVar1 = *puVar1 | 0x1000000;
  do {
  } while ((int)(*puVar1 << 7) < 0);
  return;
}

