// 0x0803bf60 FUN_0803bf60 size=312

longlong FUN_0803bf60(uint param_1,uint param_2,uint param_3,uint param_4)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  uint uVar6;
  int extraout_r1;
  int iVar7;
  uint uVar8;
  uint extraout_r3;
  uint unaff_r5;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  bool bVar16;
  
  uVar13 = DAT_0803c0ac & param_2 >> 4;
  bVar16 = uVar13 == 0;
  if (!bVar16) {
    unaff_r5 = DAT_0803c0ac & param_4 >> 4;
    bVar16 = unaff_r5 == 0;
  }
  if (!bVar16) {
    bVar16 = uVar13 == DAT_0803c0ac;
  }
  if (!bVar16) {
    bVar16 = unaff_r5 == DAT_0803c0ac;
  }
  if (bVar16) {
    if (uVar13 != DAT_0803c0ac && (DAT_0803c0ac & param_4 >> 4) != DAT_0803c0ac) {
      return (ulonglong)((param_2 ^ param_4) & 0x80000000) << 0x20;
    }
    FUN_0803c0b4();
    uVar13 = extraout_r1 << 2 ^ extraout_r3;
  }
  else {
    uVar6 = param_2 & ~(DAT_0803c0ac << 5) | 0x100000;
    uVar8 = param_4 & ~(DAT_0803c0ac << 5) | 0x100000;
    iVar14 = (uVar13 | (param_2 ^ param_4) >> 0x1f) + unaff_r5;
    lVar1 = (ulonglong)uVar6 * (ulonglong)param_3;
    uVar11 = (uint)((ulonglong)lVar1 >> 0x20);
    lVar2 = (ulonglong)param_1 * (ulonglong)uVar8;
    uVar9 = (uint)((ulonglong)lVar2 >> 0x20);
    iVar15 = iVar14 + -0x3fc0000;
    lVar4 = lVar1 + lVar2;
    uVar10 = (uint)((ulonglong)lVar4 >> 0x20);
    lVar3 = (ulonglong)uVar6 * (ulonglong)uVar8;
    uVar13 = (uint)lVar3;
    uVar12 = (uint)((ulonglong)param_1 * (ulonglong)param_3 >> 0x20);
    lVar5 = lVar4 + CONCAT44(uVar13,uVar12);
    uVar8 = (uint)lVar5;
    uVar6 = (uint)((ulonglong)lVar5 >> 0x20);
    uVar13 = (int)((ulonglong)lVar3 >> 0x20) +
             (uint)(CARRY4(uVar9,uVar11) ||
                   CARRY4(uVar9 + uVar11,(uint)CARRY4((uint)lVar2,(uint)lVar1))) +
             (uint)(CARRY4(uVar10,uVar13) ||
                   CARRY4(uVar10 + uVar13,(uint)CARRY4((uint)lVar4,uVar12)));
    if ((int)((ulonglong)param_1 * (ulonglong)param_3) != 0) {
      uVar8 = uVar8 | 1;
    }
    if ((uVar13 & 0x200) == 0) {
      uVar9 = uVar8 << 0xc;
      uVar13 = uVar13 * 0x1000 | uVar6 >> 0x14;
      uVar6 = uVar6 * 0x1000 | uVar8 >> 0x14;
      iVar7 = -4;
    }
    else {
      uVar9 = uVar8 << 0xb;
      uVar13 = uVar13 * 0x800 | uVar6 >> 0x15;
      uVar6 = uVar6 * 0x800 | uVar8 >> 0x15;
      iVar7 = -3;
    }
    uVar8 = iVar7 + (iVar15 >> 0x10);
    uVar13 = uVar13 + uVar8 * 0x100000 ^ iVar14 * -0x80000000;
    if (uVar9 != 0) {
      bVar16 = (uVar9 & 0x80000000) != 0;
      if ((uVar9 & 0x7fffffff) != 0) {
        uVar9 = 0;
      }
      uVar13 = uVar13 + CARRY4(uVar6,(uint)bVar16);
      uVar6 = uVar6 + bVar16 & ~(uVar9 >> 0x1f);
    }
    if (uVar8 < 0x7fe) {
      return CONCAT44(uVar13,uVar6);
    }
    if (iVar15 < 0x4000000) {
      return (ulonglong)(uVar13 + 0x60000000 & 0x80000000) << 0x20;
    }
    uVar13 = uVar13 + 0xa0000000;
  }
  return (ulonglong)((uint)((int)uVar13 < 0) << 0x1f | 0x7ff00000) << 0x20;
}

