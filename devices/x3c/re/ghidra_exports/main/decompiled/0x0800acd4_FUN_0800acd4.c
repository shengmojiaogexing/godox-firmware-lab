// 0x0800acd4 FUN_0800acd4 size=42

uint FUN_0800acd4(uint param_1)

{
  int iVar1;
  
  if (param_1 % 10 < 3) {
    iVar1 = 0;
  }
  else if (param_1 % 10 < 7) {
    iVar1 = 1;
  }
  else {
    iVar1 = 2;
  }
  return iVar1 + (param_1 / 10) * 3 & 0xff;
}

