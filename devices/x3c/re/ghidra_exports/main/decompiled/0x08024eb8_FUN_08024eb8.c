// 0x08024eb8 FUN_08024eb8 size=128

void FUN_08024eb8(int param_1)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  undefined1 auStack_98 [128];
  
  if ((*(byte *)(param_1 + 0x32) & 1) == 0) {
    return;
  }
  uVar2 = *(ushort *)(param_1 + 0x2c) / 7;
  uVar6 = ((uint)*(ushort *)(param_1 + 0x2e) - uVar2 * (*(ushort *)(param_1 + 0x2e) / uVar2)) +
          uVar2 * 3;
  sVar1 = (short)uVar6;
  *(short *)(param_1 + 0x2e) = sVar1;
  *(short *)(param_1 + 0x30) =
       (sVar1 - (short)uVar2 * (short)((uVar6 & 0xffff) / uVar2)) + (short)(uVar2 * 3);
  iVar3 = FUN_0802faa0(param_1,0);
  iVar4 = FUN_0802fab2(param_1,0);
  iVar7 = *(int *)(iVar3 + 8);
  iVar3 = FUN_0802ec9c(param_1);
  uVar5 = FUN_080223b4(param_1);
  iVar4 = (iVar3 / 2 - iVar7 / 2) - (iVar7 + iVar4) * (uint)*(ushort *)(param_1 + 0x2e);
  iVar3 = FUN_0802edcc(uVar5,8,auStack_98 + 0x88,0);
  if (((iVar3 == 1) && (iVar7 - (iVar7 >> 0x1f) != iVar4)) || (iVar3 == 0)) {
    FUN_080316f4(uVar5,iVar4,0);
  }
  return;
}

