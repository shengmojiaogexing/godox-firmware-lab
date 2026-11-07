// 0x08017f7c FUN_08017f7c size=16

void FUN_08017f7c(uint *param_1,int param_2)

{
  param_1[1] = 0;
  param_1[2] = 0;
  *param_1 = param_2 + 3U & 0xfffffffc;
  return;
}

