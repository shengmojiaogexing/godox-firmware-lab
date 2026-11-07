// 0x08037348 FUN_08037348 size=38

undefined4 FUN_08037348(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  *DAT_08037370 = 4;
  iVar1 = FUN_080373f0();
  if (iVar1 == 0) {
    uVar2 = FUN_08037570(param_1,0xa010,4);
    return uVar2;
  }
  return 0xffffffff;
}

