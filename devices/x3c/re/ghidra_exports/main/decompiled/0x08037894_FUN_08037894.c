// 0x08037894 FUN_08037894 size=90

undefined4 FUN_08037894(int param_1,uint param_2)

{
  int iVar1;
  
  if (((param_1 == 0) || (param_2 < 0x400)) || (0x9f00 < param_2)) {
    return 0xffffffff;
  }
  if ((*(char *)(param_1 + 0x30) != '\x16') || (*(char *)(param_1 + 0x31) != 'X')) {
    return 0xffffffff;
  }
  iVar1 = FUN_080373f0();
  if (iVar1 != 0) {
    return 0xffffffff;
  }
  iVar1 = FUN_080373a6();
  if (iVar1 != 0) {
    return 0xffffffff;
  }
  iVar1 = FUN_08037618(param_1,0,param_2);
  if (iVar1 == 0) {
    return 0;
  }
  return 0xffffffff;
}

