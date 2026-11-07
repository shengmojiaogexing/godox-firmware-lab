// 0x0803a0b0 FUN_0803a0b0 size=154

void FUN_0803a0b0(int param_1,int param_2,int param_3,int *param_4,int *param_5)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x18);
  if ((*(int *)(param_1 + 0x1c) == 0) && (iVar1 == 0x100)) {
    *param_4 = param_2 << 8;
    *param_5 = param_3 << 8;
    return;
  }
  param_2 = param_2 - *(int *)(param_1 + 0x28);
  param_3 = param_3 - *(int *)(param_1 + 0x2c);
  if (*(int *)(param_1 + 0x1c) == 0) {
    *param_4 = param_2 * iVar1 + *(int *)(param_1 + 0x20);
    *param_5 = param_3 * *(int *)(param_1 + 0x18) + *(int *)(param_1 + 0x24);
    return;
  }
  if (iVar1 == 0x100) {
    *param_4 = *(int *)(param_1 + 0x20) +
               (param_2 * *(int *)(param_1 + 0x14) - *(int *)(param_1 + 0x10) * param_3 >> 2);
    *param_5 = *(int *)(param_1 + 0x24) +
               (*(int *)(param_1 + 0x14) * param_3 + param_2 * *(int *)(param_1 + 0x10) >> 2);
    return;
  }
  *param_4 = *(int *)(param_1 + 0x20) +
             (iVar1 * (param_2 * *(int *)(param_1 + 0x14) - *(int *)(param_1 + 0x10) * param_3) >>
             10);
  *param_5 = *(int *)(param_1 + 0x24) +
             (*(int *)(param_1 + 0x18) *
              (*(int *)(param_1 + 0x14) * param_3 + param_2 * *(int *)(param_1 + 0x10)) >> 10);
  return;
}

