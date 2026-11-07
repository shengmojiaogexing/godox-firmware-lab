// 0x0803a852 FUN_0803a852 size=26

void FUN_0803a852(int param_1,undefined4 *param_2,undefined4 param_3,int param_4)

{
  uint uVar1;
  
  for (uVar1 = 0; uVar1 < param_4 + 3U >> 2; uVar1 = uVar1 + 1) {
    *param_2 = *(undefined4 *)(param_1 + 0x1000);
    param_2 = param_2 + 1;
  }
  return;
}

