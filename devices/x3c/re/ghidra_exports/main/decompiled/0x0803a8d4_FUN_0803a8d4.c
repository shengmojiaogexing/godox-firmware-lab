// 0x0803a8d4 FUN_0803a8d4 size=132

void FUN_0803a8d4(int *param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = *param_1;
  uVar2 = param_1[param_2 * 0xb + 9] - param_1[param_2 * 0xb + 10];
  if ((uint)param_1[param_2 * 0xb + 5] <
      (uint)(param_1[param_2 * 0xb + 9] - param_1[param_2 * 0xb + 10])) {
    uVar2 = param_1[param_2 * 0xb + 5];
  }
  while (uVar2 + 3 >> 2 < (*(uint *)(iVar3 + param_2 * 0x20 + 0x918) & 0xffff)) {
    uVar1 = param_1[param_2 * 0xb + 9];
    if ((uVar1 <= (uint)param_1[param_2 * 0xb + 10]) || (uVar1 == 0)) break;
    uVar2 = uVar1 - param_1[param_2 * 0xb + 10];
    if ((uint)param_1[param_2 * 0xb + 5] < uVar2) {
      uVar2 = param_1[param_2 * 0xb + 5];
    }
    FUN_0803a958(iVar3,param_1[param_2 * 0xb + 8],param_2 & 0xffff,uVar2 & 0xffff);
    param_1[param_2 * 0xb + 8] = param_1[param_2 * 0xb + 8] + uVar2;
    param_1[param_2 * 0xb + 10] = param_1[param_2 * 0xb + 10] + uVar2;
  }
  if (uVar2 == 0) {
    *(uint *)(iVar3 + 0x834) = *(uint *)(iVar3 + 0x834) & ~(1 << (param_2 & 0xff));
  }
  return;
}

