// 0x080037f4 FUN_080037f4 size=74

void FUN_080037f4(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint *puVar4;
  
  iVar3 = (param_1 & 3) << 3;
  uVar2 = 0xff << iVar3;
  uVar1 = ((param_2 & 3) << 6) << iVar3;
  if ((int)param_1 < 0) {
    iVar3 = ((param_1 & 0xf) - 8 & 0xfffffffc) + DAT_08003840;
    *(uint *)(iVar3 + 0x1c) = *(uint *)(iVar3 + 0x1c) & ~uVar2 | uVar1;
  }
  else {
    puVar4 = (uint *)((param_1 & 0xfffffffc) + DAT_08003844);
    *puVar4 = *puVar4 & ~uVar2 | uVar1;
  }
  *DAT_08003848 = 1 << (param_1 & 0x1f);
  return;
}

