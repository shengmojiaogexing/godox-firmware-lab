// 0x0802fcac FUN_0802fcac size=10

bool FUN_0802fcac(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 0x24) & param_2) != 0;
}

