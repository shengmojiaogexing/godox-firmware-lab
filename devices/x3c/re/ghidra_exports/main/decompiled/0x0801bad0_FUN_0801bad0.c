// 0x0801bad0 FUN_0801bad0 size=68

void FUN_0801bad0(undefined4 param_1,uint param_2)

{
  switch(param_1) {
  case 0:
    *DAT_0801bb1c = *DAT_0801bb1c & 0xfffffffe | param_2 & 1;
    break;
  case 1:
    *DAT_0801bb1c = *DAT_0801bb1c & 0xfffeffff | (param_2 & 1) << 0x10;
    return;
  case 2:
    *DAT_0801bb1c = *DAT_0801bb1c & 0xfeffffff | (param_2 & 1) << 0x18;
    return;
  case 3:
    DAT_0801bb1c[0x1c] = DAT_0801bb1c[0x1c] & 0xfffffffe | param_2 & 1;
    return;
  case 4:
    DAT_0801bb1c[0x1d] = DAT_0801bb1c[0x1d] & 0xfffffffe | param_2 & 1;
    return;
  }
  return;
}

