// 0x0802fc68 FUN_0802fc68 size=54

int FUN_0802fc68(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar1 = FUN_0802ee0a(param_1);
  if (iVar1 == 0) {
    return *(int *)(param_1 + 0x18);
  }
  iVar2 = *(int *)(param_1 + 0x18);
  iVar5 = *(int *)(iVar1 + 0x18);
  iVar3 = FUN_0802f0dc(iVar1);
  iVar4 = FUN_0802f998(iVar1,0);
  iVar1 = FUN_0802f864(iVar1,0);
  return (((iVar2 - iVar5) + iVar3) - iVar4) - iVar1;
}

