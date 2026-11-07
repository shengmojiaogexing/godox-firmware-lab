// 0x08035c00 FUN_08035c00 size=42

uint FUN_08035c00(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar1 = DAT_08035c2c;
  if (*(int *)(DAT_08035c2c + 4) == 0) {
    return 0;
  }
  if (param_1 == 0) {
    return *DAT_08035c30;
  }
  iVar2 = FUN_08021f20(param_1);
  uVar3 = iVar2 + param_1;
  if (*(uint *)(iVar1 + 8) <= uVar3) {
    uVar3 = 0;
  }
  return uVar3;
}

