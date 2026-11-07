// 0x08004df8 FUN_08004df8 size=62

int FUN_08004df8(uint *param_1)

{
  uint *puVar1;
  uint *puVar2;
  int iVar3;
  uint uVar4;
  
  puVar2 = param_1;
  while (((uint)puVar2 & 3) != 0) {
    puVar1 = (uint *)((int)puVar2 + 1);
    uVar4 = *puVar2;
    puVar2 = puVar1;
    if ((char)uVar4 == '\0') {
      return (int)puVar1 - ((int)param_1 + 1);
    }
  }
  do {
    uVar4 = *puVar2;
    puVar2 = puVar2 + 1;
    uVar4 = uVar4 + 0xfefefeff & ~uVar4;
  } while ((uVar4 & 0x80808080) == 0);
  iVar3 = (int)puVar2 - (int)((int)param_1 + 1);
  if ((uVar4 & 0x80) != 0) {
    return iVar3 + -3;
  }
  if ((uVar4 & 0x8080) == 0) {
    if ((uVar4 & 0x808080) != 0) {
      return iVar3 + -1;
    }
  }
  else {
    iVar3 = iVar3 + -2;
  }
  return iVar3;
}

