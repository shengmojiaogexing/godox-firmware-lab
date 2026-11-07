// 0x08021eb0 FUN_08021eb0 size=112

uint FUN_08021eb0(int param_1)

{
  ushort uVar1;
  short sVar2;
  short sVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  
  uVar4 = (uint)*(ushort *)(param_1 + 0x2c);
  if ((*(ushort *)(param_1 + 0x3c) & 6) == 0) {
    return uVar4 + *(ushort *)(param_1 + 0x30) & 0xffff;
  }
  uVar6 = (*(ushort *)(param_1 + 0x3c) & 7) >> 1;
  if (uVar6 == 2) {
    return uVar4 + *(ushort *)(param_1 + 0x2e) & 0xffff;
  }
  if (uVar6 == 1) {
    sVar2 = *(short *)(param_1 + 0x34);
    if ((uint)*(ushort *)(param_1 + 0x34) < (uint)*(ushort *)(param_1 + 0x32)) {
      sVar2 = *(ushort *)(param_1 + 0x34) + 0x168;
    }
    sVar3 = *(short *)(param_1 + 0x30);
    uVar1 = *(ushort *)(param_1 + 0x30);
    uVar6 = (uint)*(ushort *)(param_1 + 0x2e);
    if (uVar1 < uVar6) {
      sVar3 = uVar1 + 0x168;
    }
    iVar5 = (int)((uint)*(ushort *)(param_1 + 0x32) + (int)sVar2) / 2;
    if (iVar5 <= (int)uVar6) {
      if (iVar5 < sVar3) {
        return uVar4 + uVar1 & 0xffff;
      }
      return uVar4 + iVar5 & 0xffff;
    }
    uVar4 = uVar4 + uVar6 & 0xffff;
  }
  return uVar4;
}

