// 0x0800ced0 FUN_0800ced0 size=138

void FUN_0800ced0(uint param_1)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  
  puVar1 = DAT_0800cf5c;
  *DAT_0800cf5c = *DAT_0800cf5c & 0xffffbfff;
  if ((param_1 & 1) == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = 0x4000;
  }
  *puVar1 = *puVar1 | uVar3;
  puVar1 = DAT_0800cf60;
  *DAT_0800cf60 = *DAT_0800cf60 & 0xffffffbf;
  if ((param_1 >> 1 & 1) == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = 0x40;
  }
  *puVar1 = *puVar1 | uVar3;
  *puVar1 = *puVar1 & 0xfffff7ff;
  if ((param_1 >> 2 & 1) == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = 0x800;
  }
  *puVar1 = *puVar1 | uVar3;
  *puVar1 = *puVar1 & 0xffffefff;
  if ((param_1 >> 3 & 1) == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = 0x1000;
  }
  *puVar1 = *puVar1 | uVar3;
  iVar2 = DAT_0800cf64;
  *(uint *)(DAT_0800cf64 + 0x14) = *(uint *)(DAT_0800cf64 + 0x14) & 0xffffffc3;
  *(uint *)(iVar2 + 0x14) = *(uint *)(iVar2 + 0x14) | param_1 >> 2 & 0x3c;
  return;
}

