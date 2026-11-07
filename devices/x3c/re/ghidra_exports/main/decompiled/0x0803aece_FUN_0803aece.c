// 0x0803aece FUN_0803aece size=34

void FUN_0803aece(undefined4 *param_1,uint param_2)

{
  int iVar1;
  
  if ((int)(param_2 << 0x18) < 0) {
    iVar1 = (param_2 & 0x7f) * 0xb + 3;
  }
  else {
    iVar1 = (param_2 & 0x7f) * 0xb + 0x5b;
  }
  FUN_0803a408(*param_1,param_1 + iVar1);
  return;
}

