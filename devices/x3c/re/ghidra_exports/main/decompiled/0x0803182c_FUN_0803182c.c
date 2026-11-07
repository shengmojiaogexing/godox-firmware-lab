// 0x0803182c FUN_0803182c size=54

undefined2 FUN_0803182c(uint param_1,uint param_2)

{
  undefined2 uVar1;
  
  if (0x12 < param_1) {
    uVar1 = FUN_08027a64();
    return uVar1;
  }
  if ((param_2 == 0) || (4 < param_2)) {
    uVar1 = FUN_08027a64();
    return uVar1;
  }
  return *(undefined2 *)(DAT_08031864 + param_1 * 8 + (param_2 - 1 & 0xff) * 2);
}

