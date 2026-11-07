// 0x080f8fc0 FUN_080f8fc0 size=10

bool FUN_080f8fc0(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 0x14) & param_2) != 0;
}

