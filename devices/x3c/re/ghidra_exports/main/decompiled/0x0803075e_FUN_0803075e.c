// 0x0803075e FUN_0803075e size=496

void FUN_0803075e(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  uint extraout_r2;
  int iVar12;
  int iVar13;
  undefined8 uVar14;
  
  iVar1 = FUN_08030384(param_1);
  if (iVar1 != 0) {
    return;
  }
  iVar1 = FUN_0802ee0a(param_1);
  uVar2 = FUN_0802faf4(param_1,0);
  uVar3 = FUN_0802fafa(param_1,0);
  if (iVar1 == 0) {
    FUN_080304c2(param_1,uVar2,uVar3);
    return;
  }
  iVar4 = FUN_0802ecd0(iVar1);
  iVar5 = FUN_0802ec9c(iVar1);
  if (((uVar2 & 0x7fffffff) >> 0x1d == 1) && (uVar6 = uVar2 & 0x9fffffff, (int)uVar6 < 0x7d1)) {
    if (1000 < (int)uVar6) {
      uVar6 = 1000 - uVar6;
    }
    uVar2 = (int)(uVar6 * iVar4) / 100;
  }
  if (((uVar3 & 0x7fffffff) >> 0x1d == 1) && (uVar6 = uVar3 & 0x9fffffff, (int)uVar6 < 0x7d1)) {
    if (1000 < (int)uVar6) {
      uVar6 = 1000 - uVar6;
    }
    uVar3 = (int)(iVar5 * uVar6) / 100;
  }
  uVar6 = FUN_0802f9bc(param_1,0,0x6a);
  uVar14 = FUN_0802f9bc(param_1,0,0x6b);
  iVar7 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar14 >> 0x20),(int)uVar14);
  iVar8 = FUN_0802ed88(param_1);
  if (((uVar6 & 0x7fffffff) >> 0x1d == 1) && (uVar9 = uVar6 & 0x9fffffff, (int)uVar9 < 0x7d1)) {
    if (1000 < (int)uVar9) {
      uVar9 = 1000 - uVar9;
    }
    uVar6 = (int)(uVar9 * iVar7) / 100;
  }
  uVar9 = extraout_r2;
  if (((extraout_r2 & 0x7fffffff) >> 0x1d == 1) &&
     (uVar10 = extraout_r2 & 0x9fffffff, (int)uVar10 < 0x7d1)) {
    if (1000 < (int)uVar10) {
      uVar10 = 1000 - uVar10;
    }
    uVar9 = (int)(iVar8 * uVar10) / 100;
  }
  iVar12 = uVar6 + uVar2;
  iVar13 = uVar3 + uVar9;
  iVar11 = FUN_0802f7c8(param_1,0);
  if (iVar11 == 0) {
    iVar1 = FUN_0802f7e0(iVar1,0);
    if (iVar1 == 1) {
      iVar11 = 3;
    }
    else {
      iVar11 = 1;
    }
  }
  if (iVar11 == 1) {
    FUN_080304c2(param_1,iVar12,iVar13);
    return;
  }
  switch(iVar11) {
  case 2:
    iVar12 = iVar12 + (iVar4 / 2 - iVar7 / 2);
    break;
  case 3:
    iVar12 = iVar12 + (iVar4 - iVar7);
    break;
  case 4:
    iVar13 = iVar13 + (iVar5 - iVar8);
    break;
  case 5:
    iVar12 = iVar12 + (iVar4 / 2 - iVar7 / 2);
    iVar13 = iVar13 + (iVar5 - iVar8);
    break;
  case 6:
    iVar12 = iVar12 + (iVar4 - iVar7);
    iVar13 = iVar13 + (iVar5 - iVar8);
    break;
  case 7:
    iVar13 = iVar13 + (iVar5 / 2 - iVar8 / 2);
    break;
  case 8:
    iVar12 = iVar12 + (iVar4 - iVar7);
    iVar13 = iVar13 + (iVar5 / 2 - iVar8 / 2);
    break;
  case 9:
    iVar12 = iVar12 + (iVar4 / 2 - iVar7 / 2);
    iVar13 = iVar13 + (iVar5 / 2 - iVar8 / 2);
  }
  FUN_080304c2(param_1,iVar12,iVar13);
  return;
}

