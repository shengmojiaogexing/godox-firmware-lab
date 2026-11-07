// 0x0802d268 FUN_0802d268 size=86

undefined4 FUN_0802d268(int param_1,undefined4 param_2,uint param_3)

{
  int iVar1;
  
  FUN_0802ca36();
  if (param_3 < 5) {
    *(byte *)(param_1 + 0x54) = *(byte *)(param_1 + 0x54) & 0xbf;
    FUN_0802d764(param_1 + 0x30,param_2,param_3);
  }
  else {
    iVar1 = FUN_0802d5c4(param_3 + 1);
    *(int *)(param_1 + 0x30) = iVar1;
    if (iVar1 == 0) {
      return 0;
    }
    FUN_0802d764(iVar1,param_2,param_3);
    *(undefined1 *)(*(int *)(param_1 + 0x30) + param_3) = 0;
    *(byte *)(param_1 + 0x54) = *(byte *)(param_1 + 0x54) | 0x40;
  }
  return 1;
}

