// 0x0802099c FUN_0802099c size=100

void FUN_0802099c(char *param_1)

{
  uint uVar1;
  uint *puVar2;
  uint *puVar3;
  
  puVar3 = DAT_08020a00;
  uVar1 = *(uint *)(param_1 + 4);
  *DAT_08020a00 = *DAT_08020a00 & ~uVar1;
  puVar2 = puVar3 + 1;
  *puVar2 = *puVar2 & ~uVar1;
  if (param_1[9] != '\0') {
    if (*param_1 == '\0') {
      *puVar3 = *puVar3 | uVar1;
    }
    else {
      *puVar2 = *puVar2 | uVar1;
    }
    puVar3 = DAT_08020a00;
    puVar2 = DAT_08020a00 + 2;
    *puVar2 = *puVar2 & ~uVar1;
    puVar3 = puVar3 + 3;
    *puVar3 = *puVar3 & ~uVar1;
    if (param_1[8] == '\0') {
      *puVar2 = *puVar2 | uVar1;
      return;
    }
    if (param_1[8] == '\x01') {
      *puVar3 = *puVar3 | uVar1;
      return;
    }
    *puVar2 = *puVar2 | uVar1;
    *puVar3 = *puVar3 | uVar1;
  }
  return;
}

