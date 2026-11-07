// 0x08002fb6 FUN_08002fb6 size=10

bool FUN_08002fb6(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 0x10) & param_2) != 0;
}

