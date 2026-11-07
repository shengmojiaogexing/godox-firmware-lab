// 0x08038216 FUN_08038216 size=14

bool FUN_08038216(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 8) & param_2) != 0;
}

