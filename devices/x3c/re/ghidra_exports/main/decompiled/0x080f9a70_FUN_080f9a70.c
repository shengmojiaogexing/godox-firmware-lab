// 0x080f9a70 FUN_080f9a70 size=24

uint FUN_080f9a70(uint param_1)

{
  uint uVar1;
  
  uVar1 = 0;
  for (; 9 < param_1; param_1 = param_1 - 10 & 0xff) {
    uVar1 = uVar1 + 1 & 0xff;
  }
  return param_1 | (uVar1 & 0xf) << 4;
}

