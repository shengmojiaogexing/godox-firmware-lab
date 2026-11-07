// 0x08019abc FUN_08019abc size=36

uint FUN_08019abc(int param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = 0;
  uVar3 = 0;
  for (uVar1 = 0; uVar1 < param_2; uVar1 = uVar1 + 1 & 0xffff) {
    uVar2 = uVar2 + *(byte *)(param_1 + uVar1) & 0xffff;
    uVar3 = uVar1 * *(byte *)(param_1 + uVar1) + uVar3 & 0xffff;
  }
  return uVar2 | uVar3 << 0x10;
}

