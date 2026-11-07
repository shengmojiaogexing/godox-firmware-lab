// 0x0801c118 FUN_0801c118 size=122

void FUN_0801c118(uint param_1)

{
  byte bVar1;
  bool bVar2;
  
  if (*DAT_0801c194 == '\0') {
    while (bVar2 = param_1 != 0, param_1 = param_1 - 1 & 0xffff, bVar2) {
      bVar1 = 0;
      do {
        bVar1 = bVar1 + 1;
      } while (bVar1 < 0x11);
    }
  }
  else if (*DAT_0801c194 == '\x01') {
    while (bVar2 = param_1 != 0, param_1 = param_1 - 1 & 0xffff, bVar2) {
      bVar1 = 0;
      do {
        bVar1 = bVar1 + 1;
      } while (bVar1 < 2);
    }
    return;
  }
  return;
}

