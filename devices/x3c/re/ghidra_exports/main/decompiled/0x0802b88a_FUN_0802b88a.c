// 0x0802b88a FUN_0802b88a size=40

undefined4 FUN_0802b88a(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uStack_c;
  
  uStack_c = param_2;
  iVar1 = FUN_0802b754();
  if (iVar1 == 0) {
    return 1;
  }
  iVar2 = FUN_0802fcb6(iVar1,0x80);
  if (iVar2 == 0) {
    uVar3 = FUN_0802ad74(iVar1,0xd,&uStack_c);
    return uVar3;
  }
  return 1;
}

