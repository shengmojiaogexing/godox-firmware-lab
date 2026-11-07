// 0x080fcf38 FUN_080fcf38 size=116

void FUN_080fcf38(uint *param_1,ushort *param_2)

{
  uint *puVar1;
  
  puVar1 = DAT_080fcfac;
  if ((param_1 == DAT_080fcfac) || (param_1 == DAT_080fcfb0)) {
    *param_1 = *param_1 & 0xffffffef | ((byte)param_2[1] & 1) << 4;
    *param_1 = *param_1 & 0xffffff9f | (((byte)param_2[1] & 7) >> 1) << 5;
  }
  if (param_1 != DAT_080fcfb4) {
    *param_1 = *param_1 & 0xfffffcff | ((byte)param_2[4] & 3) << 8;
  }
  param_1[0xb] = *(uint *)(param_2 + 2);
  param_1[10] = (uint)*param_2;
  if ((((param_1 == puVar1) || (param_1 == DAT_080fcfb8)) || (param_1 == DAT_080fcfbc)) ||
     (param_1 == DAT_080fcfc0)) {
    param_1[0xc] = (uint)*(byte *)((int)param_2 + 9);
  }
  param_1[5] = param_1[5] | 1;
  return;
}

