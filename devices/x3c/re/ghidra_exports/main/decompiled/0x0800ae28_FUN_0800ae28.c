// 0x0800ae28 FUN_0800ae28 size=32

uint FUN_0800ae28(uint param_1)

{
  if (*DAT_0800ae48 == '\0') {
    return (uint)*DAT_0800ae4c;
  }
  if (*DAT_0800ae48 == '\x01') {
    param_1 = (*DAT_0800ae4c / 10) * 3;
  }
  return param_1;
}

