// 0x0803a602 FUN_0803a602 size=92

void FUN_0803a602(int param_1,byte *param_2)

{
  int iVar1;
  
  iVar1 = param_1 + (uint)*param_2 * 0x20;
  if (param_2[2] == 0) {
    if (-1 < *(int *)(iVar1 + 0x900)) {
      *(uint *)(iVar1 + 0x900) = *(uint *)(iVar1 + 0x900) & 0xbfffffff;
    }
    param_1 = param_1 + (uint)*param_2 * 0x20;
    *(uint *)(param_1 + 0x900) = *(uint *)(param_1 + 0x900) | 0x200000;
    return;
  }
  if (-1 < *(int *)(iVar1 + 0xb00)) {
    *(uint *)(iVar1 + 0xb00) = *(uint *)(iVar1 + 0xb00) & 0xbfffffff;
  }
  param_1 = param_1 + (uint)*param_2 * 0x20;
  *(uint *)(param_1 + 0xb00) = *(uint *)(param_1 + 0xb00) | 0x200000;
  return;
}

