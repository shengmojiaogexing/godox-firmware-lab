// 0x08030384 FUN_08030384 size=46

undefined4 FUN_08030384(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_0802fcac(param_1,DAT_080303b4);
  if (iVar1 != 0) {
    return 0;
  }
  iVar1 = FUN_0802ee0a(param_1);
  if (iVar1 == 0) {
    return 0;
  }
  iVar1 = FUN_0802f8ba(iVar1,0);
  if (iVar1 == 0) {
    return 0;
  }
  return 1;
}

