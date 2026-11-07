// 0x0800adf6 FUN_0800adf6 size=50

uint FUN_0800adf6(uint param_1)

{
  if (param_1 < 0x15) {
    return param_1 - 1 & 0xff;
  }
  if (param_1 < 0x33) {
    return (int)(param_1 - 0x14) / 5 + 0x13U & 0xff;
  }
  if (param_1 < 0xbf) {
    return (int)(param_1 - 0x32) / 10 + 0x19U & 0xff;
  }
  return 0x28;
}

