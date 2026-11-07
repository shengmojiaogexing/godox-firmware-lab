// 0x08022eb4 FUN_08022eb4 size=14

bool FUN_08022eb4(int param_1,uint param_2)

{
  return (param_2 & ~*(uint *)(param_1 + 0x10)) == 0;
}

