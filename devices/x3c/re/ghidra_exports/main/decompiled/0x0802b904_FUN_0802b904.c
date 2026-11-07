// 0x0802b904 FUN_0802b904 size=36

undefined4 FUN_0802b904(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  switch(param_1) {
  case 4:
  case 6:
    uVar1 = 0x10;
    break;
  case 5:
    return 0x18;
  case 7:
  case 0xb:
    return 1;
  case 8:
  case 0xc:
    return 2;
  case 9:
  case 0xd:
    return 4;
  case 10:
  case 0xe:
    return 8;
  }
  return uVar1;
}

