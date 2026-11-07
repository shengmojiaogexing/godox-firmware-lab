// 0x080318a8 FUN_080318a8 size=28

undefined2 FUN_080318a8(uint param_1)

{
  undefined2 uVar1;
  
  if (param_1 < 0x13) {
    return *(undefined2 *)(DAT_080318c4 + param_1 * 2);
  }
  uVar1 = FUN_08027a64();
  return uVar1;
}

