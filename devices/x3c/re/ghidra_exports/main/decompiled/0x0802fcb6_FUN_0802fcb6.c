// 0x0802fcb6 FUN_0802fcb6 size=10

bool FUN_0802fcb6(int param_1,ushort param_2)

{
  return (*(ushort *)(param_1 + 0x28) & param_2) != 0;
}

