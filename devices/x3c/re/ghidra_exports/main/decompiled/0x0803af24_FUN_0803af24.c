// 0x0803af24 FUN_0803af24 size=164

void FUN_0803af24(int *param_1,uint param_2,int param_3,int param_4)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  
  iVar6 = *param_1;
  uVar5 = param_2 & 0x7f;
  iVar1 = iVar6 + (uint)*(byte *)(param_1 + uVar5 * 0xb + 0x5b) * 0x20;
  puVar2 = (uint *)(iVar1 + 0xb00);
  param_1[uVar5 * 0xb + 0x60] = param_3;
  param_1[uVar5 * 0xb + 0x61] = param_4;
  param_1[uVar5 * 0xb + 0x62] = 0;
  if ((param_2 & 0x7f) == 0) {
    param_1[uVar5 * 0xb + 0x61] = param_1[uVar5 * 0xb + 0x5d];
  }
  if ((param_1[uVar5 * 0xb + 0x61] == 0) || ((param_2 & 0x7f) == 0)) {
    *(uint *)(iVar1 + 0xb10) =
         *(uint *)(iVar1 + 0xb10) & 0xfff80000 | param_1[uVar5 * 0xb + 0x5d] & 0x7ffffU;
    *(uint *)(iVar1 + 0xb10) = *(uint *)(iVar1 + 0xb10) & 0xe007ffff | 0x80000;
  }
  else {
    uVar3 = param_1[uVar5 * 0xb + 0x5d];
    uVar4 = ((param_1[uVar5 * 0xb + 0x61] + uVar3) - 1) / uVar3;
    *(uint *)(iVar1 + 0xb10) = *(uint *)(iVar1 + 0xb10) & 0xfff80000 | uVar4 * uVar3 & 0x7ffff;
    *(uint *)(iVar1 + 0xb10) = *(uint *)(iVar1 + 0xb10) & 0xe007ffff | (uVar4 & 0x3ff) << 0x13;
  }
  if (*(char *)((int)param_1 + uVar5 * 0x2c + 0x16f) == '\x01') {
    if ((*(uint *)(iVar6 + 0x808) >> 8 & 1) == 0) {
      *puVar2 = *puVar2 | 0x20000000;
    }
    else {
      *puVar2 = *puVar2 | 0x10000000;
    }
  }
  *puVar2 = *puVar2 | 0x4000000;
  *puVar2 = *puVar2 | 0x80000000;
  return;
}

