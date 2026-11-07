// 0x08037184 FUN_08037184 size=40

int FUN_08037184(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = *(int *)(param_1 + 0x38);
  iVar3 = 0;
  piVar1 = (int *)FUN_0802c5fc();
  for (; iVar2 != 0; iVar2 = (int)((100 - (uint)*(byte *)(*piVar1 + 0x19)) * iVar2) / 100) {
    iVar3 = iVar3 + iVar2;
  }
  return iVar3;
}

