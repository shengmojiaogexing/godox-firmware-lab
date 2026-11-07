// 0x08004010 FUN_08004010 size=42

void FUN_08004010(int param_1)

{
  if (param_1 == DAT_0800403c) {
    FUN_08003990(param_1 >> 0x12);
    FUN_08003940(param_1 >> 0x12);
    return;
  }
  FUN_08003970(0x4000);
  FUN_08003920(0x4000);
  return;
}

