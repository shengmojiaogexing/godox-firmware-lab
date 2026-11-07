// 0x08006f38 FUN_08006f38 size=116

void FUN_08006f38(uint *param_1,ushort *param_2)

{
  uint *puVar1;
  
  puVar1 = DAT_08006fac;
  if ((param_1 == DAT_08006fac) || (param_1 == DAT_08006fb0)) {
    *param_1 = *param_1 & 0xffffffef | ((byte)param_2[1] & 1) << 4;
    *param_1 = *param_1 & 0xffffff9f | (((byte)param_2[1] & 7) >> 1) << 5;
  }
  if (param_1 != DAT_08006fb4) {
    *param_1 = *param_1 & 0xfffffcff | ((byte)param_2[4] & 3) << 8;
  }
  param_1[0xb] = *(uint *)(param_2 + 2);
  param_1[10] = (uint)*param_2;
  if ((((param_1 == puVar1) || (param_1 == DAT_08006fb8)) || (param_1 == DAT_08006fbc)) ||
     (param_1 == DAT_08006fc0)) {
    param_1[0xc] = (uint)*(byte *)((int)param_2 + 9);
  }
  param_1[5] = param_1[5] | 1;
  return;
}

