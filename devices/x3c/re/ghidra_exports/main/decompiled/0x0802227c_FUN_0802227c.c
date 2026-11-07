// 0x0802227c FUN_0802227c size=136

int FUN_0802227c(int param_1,int param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int local_14;
  
  iVar4 = 0;
  piVar3 = *(int **)(*(int *)(param_1 + 0x14) + 0xc);
  uVar1 = piVar3[2];
  if ((int)((uint)*(ushort *)(*(int *)(param_1 + 0x14) + 0x12) << 0x12) < 0) {
    if ((*(byte *)(piVar3[1] + param_2) != 0) && (*(byte *)(uVar1 + param_3) != 0)) {
      iVar4 = (int)*(char *)((*(byte *)(piVar3[1] + param_2) - 1) *
                             (uint)*(byte *)((int)piVar3 + 0xd) + *piVar3 +
                            (*(byte *)(uVar1 + param_3) - 1));
    }
  }
  else if (uVar1 >> 0x1e == 0) {
    iVar5 = *piVar3;
    local_14 = param_2 + param_3 * 0x100;
    iVar2 = FUN_08018a5a(&local_14,iVar5,uVar1 & 0x3fffffff,2,DAT_08022308);
    if (iVar2 != 0) {
      iVar4 = (int)*(char *)(piVar3[1] + (iVar2 - iVar5 >> 1));
    }
  }
  else if (uVar1 >> 0x1e == 1) {
    iVar5 = *piVar3;
    local_14 = param_2 + param_3 * 0x10000;
    iVar2 = FUN_08018a5a(&local_14,iVar5,piVar3[2] & 0x3fffffff,4,DAT_08022304);
    if (iVar2 != 0) {
      iVar4 = (int)*(char *)(piVar3[1] + (iVar2 - iVar5 >> 2));
    }
  }
  return iVar4;
}

