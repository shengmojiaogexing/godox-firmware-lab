// 0x08018b68 FUN_08018b68 size=114

void FUN_08018b68(byte *param_1)

{
  uint *puVar1;
  
  puVar1 = DAT_08018bdc;
  *DAT_08018bdc = *DAT_08018bdc & 0xffffffe0 | *param_1 & 0x1f;
  *puVar1 = *puVar1 & 0xfff0ffff | (param_1[1] & 0xf) << 0x10;
  if ((int)((uint)param_1[2] << 0x1d) < 0) {
    *puVar1 = *puVar1 | 0x10000000;
  }
  else {
    *puVar1 = *puVar1 & 0xefffffff;
  }
  *puVar1 = *puVar1 & 0xffff3fff | (param_1[2] & 3) << 0xe;
  *puVar1 = *puVar1 & 0xffffdfff | (param_1[3] & 1) << 0xd;
  *puVar1 = *puVar1 & 0xfffff0ff | (param_1[4] & 0xf) << 8;
  *puVar1 = *puVar1 & 0xff7fffff | (param_1[5] & 1) << 0x17;
  *puVar1 = *puVar1 & 0xffbfffff | (param_1[6] & 1) << 0x16;
  return;
}

