// 0x0803bd2c FUN_0803bd2c size=16

void FUN_0803bd2c(int param_1,uint param_2)

{
  uint *puVar1;
  
  puVar1 = (uint *)(param_1 * 8 + -0x60000000);
  *puVar1 = *puVar1 & 0xfffffffe | param_2 & 1;
  return;
}

