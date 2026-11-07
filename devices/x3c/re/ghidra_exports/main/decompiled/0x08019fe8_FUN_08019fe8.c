// 0x08019fe8 FUN_08019fe8 size=338

void FUN_08019fe8(undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  
  iVar1 = FUN_080226d4(param_1);
  iVar2 = FUN_0801ba5a(iVar1);
  *(int *)(param_2 + 0x14) = iVar2;
  uVar3 = FUN_0802d624(iVar2 << 2);
  *(undefined4 *)(param_2 + 4) = uVar3;
  uVar3 = FUN_0802d624(*(int *)(param_2 + 0x14) << 2);
  *(undefined4 *)(param_2 + 0xc) = uVar3;
  iVar2 = DAT_0801a13c;
  for (uVar6 = 0; uVar6 < *(uint *)(param_2 + 0x14); uVar6 = uVar6 + 1) {
    if (*(int *)(iVar1 + uVar6 * 4) == iVar2) {
      iVar9 = DAT_0801a140;
      for (uVar7 = 0; uVar5 = FUN_0802ebf4(param_1), uVar7 < uVar5; uVar7 = uVar7 + 1) {
        uVar3 = FUN_0802ebc8(param_1,uVar7);
        iVar8 = FUN_0802fcac(uVar3,DAT_0801a144);
        if ((((iVar8 == 0) && (iVar8 = FUN_080226f8(uVar3), iVar8 == 1)) &&
            (uVar5 = FUN_080226e4(uVar3), uVar5 == uVar6)) &&
           (iVar8 = FUN_0802ed88(uVar3), iVar9 <= iVar8)) {
          iVar9 = FUN_0802ed88(uVar3);
        }
      }
      if (iVar9 < 0) {
        *(undefined4 *)(*(int *)(param_2 + 0xc) + uVar6 * 4) = 0;
      }
      else {
        *(int *)(*(int *)(param_2 + 0xc) + uVar6 * 4) = iVar9;
      }
    }
  }
  uVar7 = 0;
  iVar9 = 0;
  iVar8 = DAT_0801a13c + 1;
  for (uVar6 = 0; uVar6 < *(uint *)(param_2 + 0x14); uVar6 = uVar6 + 1) {
    iVar4 = *(int *)(iVar1 + uVar6 * 4);
    if (iVar4 < iVar8) {
      if (iVar4 == iVar2) {
        iVar9 = iVar9 + *(int *)(*(int *)(param_2 + 0xc) + uVar6 * 4);
      }
      else {
        *(int *)(*(int *)(param_2 + 0xc) + uVar6 * 4) = iVar4;
        iVar9 = iVar9 + iVar4;
      }
    }
    else {
      uVar7 = iVar4 + uVar7 + -iVar8;
    }
  }
  iVar2 = FUN_0802f98c(param_1,0);
  iVar4 = FUN_0802ec9c(param_1);
  iVar9 = (iVar2 * (1 - *(int *)(param_2 + 0x14)) + iVar4) - iVar9;
  if (iVar9 < 0) {
    iVar9 = 0;
  }
  uVar6 = 0xffffffff;
  iVar2 = 0;
  for (uVar5 = 0; uVar5 < *(uint *)(param_2 + 0x14); uVar5 = uVar5 + 1) {
    iVar4 = *(int *)(iVar1 + uVar5 * 4);
    if (iVar8 <= iVar4) {
      iVar2 = iVar4 + -iVar8;
      *(uint *)(*(int *)(param_2 + 0xc) + uVar5 * 4) = (uint)(iVar9 * iVar2) / uVar7;
      uVar6 = uVar5;
    }
  }
  if (-1 < (int)uVar6) {
    *(uint *)(*(int *)(param_2 + 0xc) + uVar6 * 4) = iVar9 - (iVar9 * (uVar7 - iVar2)) / uVar7;
  }
  return;
}

