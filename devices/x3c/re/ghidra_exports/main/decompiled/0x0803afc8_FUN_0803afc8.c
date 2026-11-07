// 0x0803afc8 FUN_0803afc8 size=246

void FUN_0803afc8(int *param_1,uint param_2,int param_3,int param_4)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint *puVar6;
  byte *pbVar7;
  
  iVar1 = *param_1;
  uVar4 = param_2 & 0x7f;
  pbVar7 = (byte *)(param_1 + uVar4 * 0xb + 3);
  iVar5 = iVar1 + (uint)*pbVar7 * 0x20;
  puVar6 = (uint *)(iVar5 + 0x900);
  param_1[uVar4 * 0xb + 8] = param_3;
  param_1[uVar4 * 0xb + 9] = param_4;
  param_1[uVar4 * 0xb + 10] = 0;
  uVar3 = param_1[uVar4 * 0xb + 9];
  if (uVar3 == 0) {
    *(uint *)(iVar5 + 0x910) = *(uint *)(iVar5 + 0x910) & 0xe007ffff | 0x80000;
    *(uint *)(iVar5 + 0x910) = *(uint *)(iVar5 + 0x910) & 0xfff80000;
  }
  else if ((param_2 & 0x7f) == 0) {
    if ((uint)param_1[uVar4 * 0xb + 5] < uVar3) {
      param_1[uVar4 * 0xb + 9] = param_1[uVar4 * 0xb + 5];
    }
    *(uint *)(iVar5 + 0x910) =
         *(uint *)(iVar5 + 0x910) & 0xfff80000 | param_1[uVar4 * 0xb + 9] & 0x7ffffU;
    *(uint *)(iVar5 + 0x910) = *(uint *)(iVar5 + 0x910) & 0xe007ffff | 0x80000;
    param_1[uVar4 * 0xb + 0xb] = param_1[uVar4 * 0xb + 9];
  }
  else {
    uVar2 = param_1[uVar4 * 0xb + 5];
    *(uint *)(iVar5 + 0x910) = *(uint *)(iVar5 + 0x910) & 0xfff80000 | uVar3 & 0x7ffff;
    *(uint *)(iVar5 + 0x910) =
         *(uint *)(iVar5 + 0x910) & 0xe007ffff | (((uVar3 + uVar2) - 1) / uVar2 & 0x3ff) << 0x13;
    if (*(char *)((int)param_1 + uVar4 * 0x2c + 0xf) == '\x01') {
      *(uint *)(iVar5 + 0x910) = (*(uint *)(iVar5 + 0x910) & 0x9fffffff) + 0x20000000;
    }
  }
  if ((*(char *)((int)param_1 + uVar4 * 0x2c + 0xf) != '\x01') && (param_1[uVar4 * 0xb + 9] != 0)) {
    *(uint *)(iVar1 + 0x834) = *(uint *)(iVar1 + 0x834) | 1 << *pbVar7;
  }
  if (*(char *)((int)param_1 + uVar4 * 0x2c + 0xf) == '\x01') {
    if ((*(uint *)(iVar1 + 0x808) >> 8 & 1) == 0) {
      *puVar6 = *puVar6 | 0x20000000;
    }
    else {
      *puVar6 = *puVar6 | 0x10000000;
    }
  }
  *puVar6 = *puVar6 | 0x4000000;
  *puVar6 = *puVar6 | 0x80000000;
  if (*(char *)((int)param_1 + uVar4 * 0x2c + 0xf) == '\x01') {
    FUN_0803a958(iVar1,param_1[uVar4 * 0xb + 8],*pbVar7,(short)param_1[uVar4 * 0xb + 9]);
    return;
  }
  return;
}

