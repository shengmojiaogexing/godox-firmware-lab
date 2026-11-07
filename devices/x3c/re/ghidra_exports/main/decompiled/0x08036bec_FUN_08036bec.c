// 0x08036bec FUN_08036bec size=98

void FUN_08036bec(int param_1,uint param_2)

{
  uint uVar1;
  uint *puVar2;
  uint uVar3;
  int iVar4;
  
  puVar2 = DAT_08036c50;
  iVar4 = (param_2 & 3) << 2;
  uVar3 = 0xf << iVar4;
  param_2 = param_2 >> 2;
  uVar1 = param_1 << iVar4;
  if (param_2 == 0) {
    puVar2 = DAT_08036c50 + -3;
    *puVar2 = *puVar2 & ~uVar3;
    *puVar2 = *puVar2 | uVar1;
    return;
  }
  if (param_2 == 1) {
    puVar2 = DAT_08036c50 + -2;
    *puVar2 = *puVar2 & ~uVar3;
    *puVar2 = *puVar2 | uVar1;
    return;
  }
  if (param_2 == 2) {
    puVar2 = DAT_08036c50 + -1;
    *puVar2 = *puVar2 & ~uVar3;
    *puVar2 = *puVar2 | uVar1;
    return;
  }
  if (param_2 == 3) {
    *DAT_08036c50 = *DAT_08036c50 & ~uVar3;
    *puVar2 = *puVar2 | uVar1;
  }
  return;
}

