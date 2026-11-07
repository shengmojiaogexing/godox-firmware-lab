// 0x08021812 FUN_08021812 size=24

void FUN_08021812(undefined2 *param_1,int param_2,uint param_3)

{
  uint uVar1;
  
  for (uVar1 = 0; uVar1 < param_3; uVar1 = uVar1 + 1 & 0xffff) {
    *(undefined2 *)(param_2 + uVar1 * 2) = *param_1;
    param_1 = param_1 + 1;
  }
  return;
}

