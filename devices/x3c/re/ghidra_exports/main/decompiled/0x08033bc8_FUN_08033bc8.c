// 0x08033bc8 FUN_08033bc8 size=98

undefined4 FUN_08033bc8(int param_1,int param_2)

{
  char cVar1;
  
  if (*(char *)(DAT_08033c2c + param_1 * 10) != '\"') {
    return 0;
  }
  if (param_2 == -1) {
    return 0;
  }
  cVar1 = *(char *)(DAT_08033c2c + param_2 * 10);
  if (cVar1 == '\0') {
    return 0xfef5;
  }
  if (cVar1 == '\x01') {
    return 0xfef7;
  }
  if (cVar1 == '\x03') {
    return 0xfef9;
  }
  if (cVar1 == '\x05') {
    return 0xfefb;
  }
  return 0;
}

