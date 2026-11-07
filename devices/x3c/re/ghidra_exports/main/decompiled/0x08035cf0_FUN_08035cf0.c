// 0x08035cf0 FUN_08035cf0 size=26

void FUN_08035cf0(uint param_1)

{
  *DAT_08035d0c = *DAT_08035d0c & 0xf8ff | (param_1 & 7) << 8 | DAT_08035d10;
  return;
}

