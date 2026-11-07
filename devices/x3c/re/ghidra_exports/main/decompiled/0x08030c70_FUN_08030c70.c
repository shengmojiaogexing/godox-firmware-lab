// 0x08030c70 FUN_08030c70 size=46

undefined4 FUN_08030c70(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_0802fadc(param_1,0);
  iVar2 = FUN_0802f89c(param_1,0);
  if ((iVar1 == DAT_08030ca0) || (iVar2 == DAT_08030ca0)) {
    FUN_08030436(param_1);
    return 1;
  }
  return 0;
}

