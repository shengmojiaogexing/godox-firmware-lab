// 0x08008560 FUN_08008560 size=148

undefined4 FUN_08008560(uint param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (param_1 == 0) {
    return 0x12;
  }
  if (param_1 < 4) {
    return 0x10;
  }
  if (param_1 < 8) {
    return 0xe;
  }
  if (param_1 < 0xb) {
    return 0xc;
  }
  if (param_1 < 0xe) {
    return 10;
  }
  if (param_1 < 0x12) {
    return 8;
  }
  if (param_1 < 0x15) {
    return 6;
  }
  if (param_1 < 0x18) {
    return 4;
  }
  if (param_1 < 0x1c) {
    uVar1 = 2;
  }
  else if (0x1e < param_1) {
    if (param_1 < 0x22) {
      return 0xfe;
    }
    if (param_1 < 0x26) {
      return 0xfc;
    }
    if (param_1 < 0x29) {
      return 0xfa;
    }
    if (param_1 < 0x2c) {
      return 0xf8;
    }
    if (param_1 < 0x30) {
      return 0xf6;
    }
    if (param_1 < 0x33) {
      return 0xf4;
    }
    if (param_1 < 0x36) {
      return 0xf2;
    }
    if (param_1 < 0x3a) {
      return 0xf0;
    }
    return 0xee;
  }
  return uVar1;
}

