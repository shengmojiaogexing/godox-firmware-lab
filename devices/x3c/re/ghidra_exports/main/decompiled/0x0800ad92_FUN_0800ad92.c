// 0x0800ad92 FUN_0800ad92 size=50

uint FUN_0800ad92(uint param_1)

{
  if (param_1 < 0x14) {
    return param_1 + 1 & 0xff;
  }
  if (param_1 < 0x1a) {
    return (param_1 - 0x13) * 5 + 0x14 & 0xff;
  }
  if (param_1 < 0x28) {
    return (param_1 - 0x19) * 10 + 0x32 & 0xff;
  }
  return 199;
}

