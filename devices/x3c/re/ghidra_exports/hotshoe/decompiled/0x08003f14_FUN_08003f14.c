// 0x08003f14 FUN_08003f14 size=142

void FUN_08003f14(uint *param_1,byte *param_2)

{
  *param_1 = *param_1 & 0xfffffffb | (*param_2 & 1) << 2;
  param_1[1] = param_1[1] & 0xfffff0ff | (param_2[1] & 0xf) << 8;
  *param_1 = *param_1 & 0xfffffffe | param_2[2] & 1;
  *param_1 = *param_1 & 0xfffffffd | (param_2[3] & 1) << 1;
  *param_1 = *param_1 & 0xfffffdff | (param_2[4] & 1) << 9;
  *param_1 = *param_1 & 0xffffff7f | (param_2[5] & 1) << 7;
  *param_1 = *param_1 & DAT_08003fa4;
  *param_1 = *param_1 | (uint)*(ushort *)(param_2 + 6);
  *param_1 = *param_1 & 0xffffffc7 | (param_2[8] & 7) << 3;
  param_1[4] = param_1[4] | (uint)param_2[9];
  return;
}

