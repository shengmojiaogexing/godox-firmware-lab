// 0x08018afc FUN_08018afc size=44

void FUN_08018afc(int param_1,byte *param_2)

{
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xfffffeff | (*param_2 & 1) << 8;
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xfffffffd | (param_2[1] & 1) << 1;
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) & 0xfffff7ff | (param_2[2] & 1) << 0xb;
  *(uint *)(param_1 + 0x2c) =
       *(uint *)(param_1 + 0x2c) & 0xff0fffff | (param_2[3] - 1 & 0xf) << 0x14;
  return;
}

