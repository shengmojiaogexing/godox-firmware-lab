// 0x0803c384 FUN_0803c384 size=38

uint FUN_0803c384(uint param_1)

{
  uint uVar1;
  
  uVar1 = param_1 << LZCOUNT(param_1);
  if (uVar1 != 0) {
    param_1 = (0x9d - LZCOUNT(param_1)) * 0x800000 + (uVar1 >> 8);
    if ((uVar1 & 0x80) == 0) {
      return param_1;
    }
    param_1 = param_1 + 1;
    if ((uVar1 & 0x7f) == 0) {
      param_1 = param_1 & 0xfffffffe;
    }
  }
  return param_1;
}

