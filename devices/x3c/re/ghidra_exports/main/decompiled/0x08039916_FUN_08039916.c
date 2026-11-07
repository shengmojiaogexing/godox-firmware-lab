// 0x08039916 FUN_08039916 size=14

bool FUN_08039916(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 0x10) & param_2) != 0;
}

