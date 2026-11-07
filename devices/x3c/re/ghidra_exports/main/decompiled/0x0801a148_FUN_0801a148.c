// 0x0801a148 FUN_0801a148 size=84

undefined4 FUN_0801a148(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  int iVar2;
  
  iVar2 = FUN_0802f9bc(param_1,0,0x6d,param_4,param_4);
  if (iVar2 != 0) {
    return 2;
  }
  iVar2 = FUN_0802f9bc(param_1,0,0x6c);
  if (iVar2 != 0x100) {
    return 2;
  }
  iVar2 = FUN_0802f8d8(param_1,0);
  if (iVar2 != 0xff) {
    return 1;
  }
  cVar1 = FUN_0802f9bc(param_1,0,0x67);
  if (cVar1 == '\0') {
    return 0;
  }
  return 1;
}

