// 0x0801a38c FUN_0801a38c size=304

uint FUN_0801a38c(uint param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = (uint)*DAT_0801a4c0;
  if (*DAT_0801a4bc == '\0') {
    if (param_2 < 1) {
      if (uVar2 < (param_1 - param_2 & 0xff)) {
        return uVar2;
      }
    }
    else if ((int)param_1 < param_2) {
      return 0;
    }
    return param_1 - param_2 & 0xff;
  }
  if (*DAT_0801a4bc != '\x01') {
    return 0;
  }
  if (param_2 < 1) {
    uVar1 = (param_1 / 10) * 3 & 0xff;
    if (2 < param_1 % 10) {
      if (param_1 % 10 < 7) {
        uVar1 = uVar1 + 1 & 0xff;
      }
      else if (param_1 % 10 < 10) {
        uVar1 = uVar1 + 2 & 0xff;
      }
    }
    uVar1 = uVar1 - param_2 & 0xff;
    if ((uVar2 / 10) * 3 < uVar1) {
      uVar1 = (uVar2 / 10) * 3;
    }
    uVar2 = ((uVar1 / 3) * 5 & 0x7f) * 2;
    if (uVar1 % 3 == 1) {
      return uVar2 + 3 & 0xff;
    }
    if (uVar1 % 3 == 2) {
      return uVar2 + 7 & 0xff;
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
    if ((int)uVar2 < param_2) {
      uVar2 = 0;
    }
    else {
      uVar2 = uVar2 - param_2 & 0xff;
    }
    uVar1 = ((uVar2 / 3) * 5 & 0x7f) * 2;
    if (uVar2 % 3 != 1) {
      if (uVar2 % 3 == 2) {
        return uVar1 + 7 & 0xff;
      }
      return uVar1;
    }
    uVar2 = uVar1 + 3 & 0xff;
  }
  return uVar2;
}

