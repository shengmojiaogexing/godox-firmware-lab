// 0x0801bd04 FUN_0801bd04 size=44

void FUN_0801bd04(uint param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 1 << (param_1 & 0x1f);
  iVar1 = DAT_0801bd30 + (param_1 >> 0x10);
  if (param_2 == 1) {
    *(uint *)(iVar1 + 0x800) = *(uint *)(iVar1 + 0x800) | uVar2;
    return;
  }
  *(uint *)(iVar1 + 0x800) = *(uint *)(iVar1 + 0x800) & ~uVar2;
  return;
}

