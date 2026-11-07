// 0x08019c0c FUN_08019c0c size=336

void FUN_08019c0c(undefined4 param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  
  iVar1 = FUN_08021fbc(param_1);
  iVar2 = FUN_0802ecd0(param_1);
  iVar3 = FUN_0801ba5a(iVar1);
  param_2[4] = iVar3;
  uVar4 = FUN_0802d624(iVar3 << 2);
  *param_2 = uVar4;
  uVar4 = FUN_0802d624(param_2[4] << 2);
  param_2[2] = uVar4;
  iVar3 = DAT_08019d5c;
  for (uVar7 = 0; uVar7 < (uint)param_2[4]; uVar7 = uVar7 + 1) {
    if (*(int *)(iVar1 + uVar7 * 4) == iVar3) {
      iVar10 = DAT_08019d60;
      for (uVar8 = 0; uVar5 = FUN_0802ebf4(param_1), uVar8 < uVar5; uVar8 = uVar8 + 1) {
        uVar4 = FUN_0802ebc8(param_1,uVar8);
        iVar9 = FUN_0802fcac(uVar4,DAT_08019d64);
        if ((((iVar9 == 0) && (iVar9 = FUN_08021fe0(uVar4), iVar9 == 1)) &&
            (uVar5 = FUN_08021fcc(uVar4), uVar5 == uVar7)) &&
           (iVar9 = FUN_0802fc5e(uVar4), iVar10 <= iVar9)) {
          iVar10 = FUN_0802fc5e(uVar4);
        }
      }
      if (iVar10 < 0) {
        *(undefined4 *)(param_2[2] + uVar7 * 4) = 0;
      }
      else {
        *(int *)(param_2[2] + uVar7 * 4) = iVar10;
      }
    }
  }
  uVar8 = 0;
  iVar10 = 0;
  iVar9 = DAT_08019d5c + 1;
  for (uVar7 = 0; uVar7 < (uint)param_2[4]; uVar7 = uVar7 + 1) {
    iVar6 = *(int *)(iVar1 + uVar7 * 4);
    if (iVar6 < iVar9) {
      if (iVar6 == iVar3) {
        iVar10 = iVar10 + *(int *)(param_2[2] + uVar7 * 4);
      }
      else {
        *(int *)(param_2[2] + uVar7 * 4) = iVar6;
        iVar10 = iVar10 + iVar6;
      }
    }
    else {
      uVar8 = iVar6 + uVar8 + -iVar9;
    }
  }
  iVar3 = FUN_0802f92c(param_1,0);
  iVar10 = (iVar3 * (1 - param_2[4]) + iVar2) - iVar10;
  if (iVar10 < 0) {
    iVar10 = 0;
  }
  uVar7 = 0xffffffff;
  iVar3 = 0;
  for (uVar5 = 0; uVar5 < (uint)param_2[4]; uVar5 = uVar5 + 1) {
    iVar2 = *(int *)(iVar1 + uVar5 * 4);
    if (iVar9 <= iVar2) {
      iVar3 = iVar2 + -iVar9;
      *(uint *)(param_2[2] + uVar5 * 4) = (uint)(iVar10 * iVar3) / uVar8;
      uVar7 = uVar5;
    }
  }
  if (-1 < (int)uVar7) {
    *(uint *)(param_2[2] + uVar7 * 4) = iVar10 - (iVar10 * (uVar8 - iVar3)) / uVar8;
  }
  return;
}

