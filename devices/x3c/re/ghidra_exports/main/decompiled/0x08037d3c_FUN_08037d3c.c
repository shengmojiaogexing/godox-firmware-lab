// 0x08037d3c FUN_08037d3c size=320

void FUN_08037d3c(int param_1,uint param_2,ushort *param_3)

{
  int iVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  ushort *puVar11;
  ushort *puVar12;
  int iVar13;
  int iVar14;
  undefined2 uVar15;
  
  iVar1 = (int)param_2 >> 1;
  iVar13 = iVar1;
  if ((param_2 & 1) == 0) {
    iVar13 = iVar1 + -1;
  }
  iVar4 = FUN_0802d624();
  iVar5 = param_1 + -1;
  puVar11 = param_3;
  for (iVar14 = 0; iVar14 < param_1; iVar14 = iVar14 + 1) {
    iVar7 = param_2 * puVar11[iVar5];
    iVar9 = param_1;
    while (iVar9 = iVar9 + -1, -1 < iVar9) {
      *(short *)(iVar4 + iVar9 * 2) = (short)iVar7;
      uVar6 = 0;
      if (iVar9 + iVar1 < param_1) {
        uVar6 = (uint)puVar11[iVar9 + iVar1];
      }
      if ((iVar9 - iVar13) + -1 < 0) {
        uVar2 = *puVar11;
      }
      else {
        uVar2 = puVar11[(iVar9 - iVar13) + -1];
      }
      iVar7 = (uint)uVar2 + (iVar7 - uVar6);
    }
    FUN_0802d764(puVar11,iVar4,param_1 << 1);
    puVar11 = puVar11 + param_1;
  }
  for (uVar6 = 0; uVar6 < (uint)(param_1 * param_1); uVar6 = uVar6 + 1) {
    uVar8 = (uint)param_3[uVar6];
    if (uVar8 != 0) {
      if (uVar8 == 0x3fc0) {
        param_3[uVar6] = (ushort)(0x3fc0 / (ulonglong)param_2);
      }
      else {
        param_3[uVar6] = (ushort)((int)uVar8 / (int)param_2);
      }
    }
  }
  for (iVar14 = 0; iVar14 < param_1; iVar14 = iVar14 + 1) {
    puVar12 = param_3 + iVar14;
    iVar9 = param_2 * *puVar12;
    puVar11 = puVar12;
    for (iVar7 = 0; iVar7 < param_1; iVar7 = iVar7 + 1) {
      if (iVar9 < 0) {
        uVar15 = 0;
      }
      else {
        uVar15 = (undefined2)(iVar9 >> 6);
      }
      *(undefined2 *)(iVar4 + iVar7 * 2) = uVar15;
      if (iVar7 - iVar1 < 1) {
        uVar2 = *puVar11;
      }
      else {
        uVar2 = param_3[(iVar7 - iVar1) * param_1 + iVar14];
      }
      iVar10 = iVar7 + iVar13 + 1;
      if (iVar10 < param_1) {
        uVar3 = param_3[iVar10 * param_1 + iVar14];
      }
      else {
        uVar3 = param_3[iVar5 * param_1 + iVar14];
      }
      iVar9 = (uint)uVar3 + (iVar9 - (uint)uVar2);
      puVar11 = puVar11 + param_1;
    }
    for (iVar7 = 0; iVar7 < param_1; iVar7 = iVar7 + 1) {
      *puVar12 = *(ushort *)(iVar4 + iVar7 * 2);
      puVar12 = puVar12 + param_1;
    }
  }
  FUN_0802d6c0(iVar4,iVar14,iVar5,param_1 << 1);
  return;
}

