// 0x08033760 FUN_08033760 size=22

uint FUN_08033760(uint param_1)

{
  if ((param_1 & 3) == 0) {
    FUN_0801b8a0(param_1);
    return param_1;
  }
  return 0;
}

