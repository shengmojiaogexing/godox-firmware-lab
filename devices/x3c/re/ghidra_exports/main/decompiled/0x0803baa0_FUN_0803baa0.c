// 0x0803baa0 FUN_0803baa0 size=28

void FUN_0803baa0(int param_1,uint param_2,uint param_3)

{
  uint *puVar1;
  
  puVar1 = (uint *)(param_1 * 4 + -0x5ffffde0);
  *puVar1 = *puVar1 & 0xffff00ff | (param_3 & 0xff) << 8;
  *puVar1 = *puVar1 & 0xffffff00 | param_2 & 0xff;
  return;
}

