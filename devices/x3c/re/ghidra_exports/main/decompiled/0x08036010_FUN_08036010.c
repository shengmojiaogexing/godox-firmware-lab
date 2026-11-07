// 0x08036010 FUN_08036010 size=12

void FUN_08036010(uint param_1)

{
  *DAT_0803601c = *DAT_0803601c & 0xfffffeff | (param_1 & 1) << 8;
  return;
}

