// 0x0801bd64 FUN_0801bd64 size=54

void FUN_0801bd64(uint param_1,uint param_2,uint param_3,uint param_4)

{
  uint *puVar1;
  
  if (param_1 == 0) {
    *DAT_0801bd9c = *DAT_0801bd9c | 0x1000;
  }
  puVar1 = DAT_0801bd9c + -0x27;
  *puVar1 = *puVar1 & 0xffbfffff | (param_1 & 1) << 0x16;
  *puVar1 = *puVar1 & 0xffff803f | (param_2 & 0x1ff) << 6;
  *puVar1 = *puVar1 & 0xfffffff0 | param_3 & 0xf;
  *puVar1 = *puVar1 & 0xfff8ffff | (param_4 & 7) << 0x10;
  return;
}

