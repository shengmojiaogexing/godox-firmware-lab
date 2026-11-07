// 0x08002fc0 FUN_08002fc0 size=10

bool FUN_08002fc0(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 0x14) & param_2) != 0;
}

