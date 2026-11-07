// 0x080398fa FUN_080398fa size=10

void FUN_080398fa(uint *param_1,uint param_2)

{
  *param_1 = *param_1 & 0xfffffffe | param_2 & 1;
  return;
}

