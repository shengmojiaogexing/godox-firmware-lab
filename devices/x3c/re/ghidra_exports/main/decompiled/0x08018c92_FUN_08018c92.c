// 0x08018c92 FUN_08018c92 size=20

void FUN_08018c92(int param_1,uint param_2,uint param_3)

{
  uint *puVar1;
  
  puVar1 = (uint *)(param_1 + 0x80);
  *puVar1 = *puVar1 & 0xffffffe3 | (param_2 & 7) << 2;
  *puVar1 = *puVar1 & 0xfffffe1f | (param_3 & 0xf) << 5;
  return;
}

