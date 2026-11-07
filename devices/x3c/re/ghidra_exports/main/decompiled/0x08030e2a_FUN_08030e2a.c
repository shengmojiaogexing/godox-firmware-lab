// 0x08030e2a FUN_08030e2a size=278

void FUN_08030e2a(int param_1,int param_2,undefined4 param_3)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined4 extraout_r2;
  int extraout_r2_00;
  undefined4 *puVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  undefined8 uVar12;
  
  iVar2 = FUN_08031788(param_3);
  iVar3 = FUN_08031782(extraout_r2);
  iVar10 = 0xffff;
  if ((param_2 != 0) && (*(char *)(param_2 + 7) == '\0')) {
    iVar10 = 0;
  }
  uVar9 = 0;
  iVar6 = 0;
  while (uVar9 < (*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3) {
    iVar11 = uVar9 * 8 + 4;
    uVar12 = FUN_08031788(*(uint *)(*(int *)(param_1 + 0xc) + iVar11) & 0xffffff);
    uVar12 = FUN_08031782(*(uint *)(*(int *)(param_1 + 0xc) + iVar11) & 0xffffff,
                          (int)((ulonglong)uVar12 >> 0x20),(int)uVar12);
    iVar6 = (int)((ulonglong)uVar12 >> 0x20);
    if ((((iVar2 == 0xffff) || (extraout_r2_00 == iVar2)) &&
        ((iVar3 == 0xf0000 || ((int)uVar12 == iVar3)))) &&
       ((param_2 == 0 || (*(int *)(*(int *)(param_1 + 0xc) + uVar9 * 8) == param_2)))) {
      if (*(int *)(*(int *)(param_1 + 0xc) + iVar11) << 6 < 0) {
        FUN_08039f20(param_1,iVar3,0xffff,0);
      }
      iVar6 = *(int *)(*(int *)(param_1 + 0xc) + iVar11);
      if ((iVar6 << 7 < 0) || (iVar6 << 6 < 0)) {
        FUN_08032ea0(*(undefined4 *)(*(int *)(param_1 + 0xc) + uVar9 * 8));
        FUN_0802d6e4(*(undefined4 *)(*(int *)(param_1 + 0xc) + uVar9 * 8));
        *(undefined4 *)(*(int *)(param_1 + 0xc) + uVar9 * 8) = 0;
      }
      for (uVar4 = uVar9; uVar4 < ((*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3) - 1;
          uVar4 = uVar4 + 1) {
        puVar8 = (undefined4 *)(*(int *)(param_1 + 0xc) + uVar4 * 8);
        puVar7 = (undefined4 *)(*(int *)(param_1 + 0xc) + (uVar4 + 1) * 8);
        uVar5 = puVar7[1];
        *puVar8 = *puVar7;
        puVar8[1] = uVar5;
      }
      uVar1 = ((*(ushort *)(param_1 + 0x2a) >> 3) - 1 & 0x3f) << 3;
      *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) & 0xfe07 | uVar1;
      uVar5 = FUN_0802d72c(*(undefined4 *)(param_1 + 0xc),uVar1);
      *(undefined4 *)(param_1 + 0xc) = uVar5;
      iVar6 = 1;
    }
    else {
      uVar9 = uVar9 + 1;
    }
  }
  if ((iVar6 == 0) || (iVar10 == 0)) {
    return;
  }
  FUN_08030ca4(param_1,iVar3,iVar10);
  return;
}

