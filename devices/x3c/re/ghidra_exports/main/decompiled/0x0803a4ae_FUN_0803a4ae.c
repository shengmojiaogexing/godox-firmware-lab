// 0x0803a4ae FUN_0803a4ae size=30

uint FUN_0803a4ae(int param_1,uint param_2)

{
  return *(uint *)(param_1 + param_2 * 0x20 + 0x908) &
         (*(uint *)(param_1 + 0x810) | (*(uint *)(param_1 + 0x834) >> (param_2 & 0xff) & 1) << 7);
}

