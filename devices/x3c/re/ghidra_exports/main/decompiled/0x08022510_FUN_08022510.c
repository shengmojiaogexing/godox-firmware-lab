// 0x08022510 FUN_08022510 size=38

int FUN_08022510(int param_1,int param_2,int *param_3,uint *param_4)

{
  *param_3 = (uint)*(ushort *)(*(int *)(param_1 + 0xc) + (param_2 + 1) * 2) -
             (uint)*(ushort *)(*(int *)(param_1 + 0xc) + param_2 * 2);
  *param_4 = (uint)*(ushort *)(*(int *)(param_1 + 8) + param_2 * 2);
  return *(int *)(param_1 + 4) + (uint)*(ushort *)(*(int *)(param_1 + 0xc) + param_2 * 2);
}

