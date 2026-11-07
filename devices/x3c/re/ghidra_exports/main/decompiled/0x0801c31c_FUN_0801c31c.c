// 0x0801c31c FUN_0801c31c size=22

void FUN_0801c31c(uint param_1)

{
  if (param_1 < 0x10000001) {
    *DAT_0801c338 = param_1;
    return;
  }
  *DAT_0801c334 = param_1 & 0xfffffff;
  return;
}

