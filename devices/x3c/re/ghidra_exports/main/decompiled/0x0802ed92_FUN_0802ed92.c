// 0x0802ed92 FUN_0802ed92 size=58

uint FUN_0802ed92(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  
  iVar1 = FUN_0802ee0a(param_1);
  if (iVar1 == 0) {
    return 0;
  }
  uVar4 = 0;
  while( true ) {
    uVar3 = FUN_0802ebf4(iVar1);
    if (uVar3 <= uVar4) {
      return 0xffffffff;
    }
    iVar2 = FUN_0802ebc8(iVar1,uVar4);
    if (iVar2 == param_1) break;
    uVar4 = uVar4 + 1;
  }
  return uVar4;
}

