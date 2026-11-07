// 0x08023ac0 FUN_08023ac0 size=58

uint FUN_08023ac0(uint param_1)

{
  uint uVar1;
  
  uVar1 = 0;
  if (param_1 - 0x30 < 10) {
    uVar1 = param_1 - 0x30 & 0xff;
  }
  else {
    if (0x60 < param_1) {
      param_1 = param_1 - 0x20 & 0xff;
    }
    switch(param_1) {
    case 0x41:
      return 10;
    case 0x42:
      return 0xb;
    case 0x43:
      return 0xc;
    case 0x44:
      return 0xd;
    case 0x45:
      return 0xe;
    case 0x46:
      return 0xf;
    }
  }
  return uVar1;
}

