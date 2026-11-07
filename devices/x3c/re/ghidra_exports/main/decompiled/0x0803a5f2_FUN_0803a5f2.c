// 0x0803a5f2 FUN_0803a5f2 size=16

uint FUN_0803a5f2(int param_1,int param_2)

{
  return *(uint *)(param_1 + 0x814) & *(uint *)(param_1 + param_2 * 0x20 + 0xb08);
}

