// 0x08031868 FUN_08031868 size=58

undefined2 FUN_08031868(uint param_1,uint param_2)

{
  undefined2 uVar1;
  
  if (0x12 < param_1) {
    uVar1 = FUN_08027a64();
    return uVar1;
  }
  if ((param_2 == 0) || (5 < param_2)) {
    uVar1 = FUN_08027a64();
    return uVar1;
  }
  return *(undefined2 *)(DAT_080318a4 + param_1 * 10 + (param_2 - 1 & 0xff) * 2);
}

