// 0x08018536 FUN_08018536 size=14

uint FUN_08018536(uint param_1)

{
  uint uVar1;
  
  uVar1 = (param_1 & 0x1ff) >> 4;
  if (7 < uVar1) {
    uVar1 = 7;
  }
  return uVar1;
}

