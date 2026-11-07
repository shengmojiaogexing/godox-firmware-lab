// 0x0801a294 FUN_0801a294 size=248

uint FUN_0801a294(uint param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  
  if (param_2 < 1) {
    uVar2 = (param_1 / 10) * 3 & 0xff;
    if (2 < param_1 % 10) {
      if (param_1 % 10 < 7) {
        uVar2 = uVar2 + 1 & 0xff;
      }
      else if (param_1 % 10 < 10) {
        uVar2 = uVar2 + 2 & 0xff;
      }
    }
    if ((int)uVar2 < -param_2) {
      uVar2 = 0;
    }
    else {
      uVar2 = uVar2 + param_2 & 0xff;
    }
    if (0x12 < uVar2) {
      uVar2 = 0x12;
    }
    uVar1 = ((uVar2 / 3) * 5 & 0x7f) * 2;
    if (uVar2 % 3 == 1) {
      return uVar1 + 3 & 0xff;
    }
    if (uVar2 % 3 == 2) {
      return uVar1 + 7 & 0xff;
    }
  }
  else {
    uVar2 = (param_1 / 10) * 3 & 0xff;
    if (2 < param_1 % 10) {
      if (param_1 % 10 < 7) {
        uVar2 = uVar2 + 1 & 0xff;
      }
      else if (param_1 % 10 < 10) {
        uVar2 = uVar2 + 2 & 0xff;
      }
    }
    uVar2 = uVar2 + param_2 & 0xff;
    if (0x12 < uVar2) {
      uVar2 = 0x12;
    }
    uVar1 = ((uVar2 / 3) * 5 & 0x7f) * 2;
    if (uVar2 % 3 != 1) {
      if (uVar2 % 3 == 2) {
        uVar1 = uVar1 + 7 & 0xff;
      }
      return uVar1;
    }
    uVar1 = uVar1 + 3 & 0xff;
  }
  return uVar1;
}

