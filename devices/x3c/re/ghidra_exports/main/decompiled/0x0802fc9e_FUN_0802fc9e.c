// 0x0802fc9e FUN_0802fc9e size=14

undefined4 FUN_0802fc9e(int param_1,uint param_2)

{
  if ((param_2 & ~*(uint *)(param_1 + 0x24)) == 0) {
    return 1;
  }
  return 0;
}

