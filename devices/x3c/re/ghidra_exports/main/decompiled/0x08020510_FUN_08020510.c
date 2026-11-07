// 0x08020510 FUN_08020510 size=22

void FUN_08020510(uint param_1)

{
  *DAT_08020528 = ~(param_1 | 0x80) | *DAT_08020528 & 0x80;
  return;
}

