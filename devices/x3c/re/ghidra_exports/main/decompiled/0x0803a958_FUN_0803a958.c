// 0x0803a958 FUN_0803a958 size=30

void FUN_0803a958(int param_1,undefined4 *param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < param_4 + 3U >> 2; uVar2 = uVar2 + 1) {
    uVar1 = *param_2;
    param_2 = param_2 + 1;
    *(undefined4 *)(param_1 + param_3 * 0x1000 + 0x1000) = uVar1;
  }
  return;
}

