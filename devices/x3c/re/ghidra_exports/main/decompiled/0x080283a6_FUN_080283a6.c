// 0x080283a6 FUN_080283a6 size=944

void FUN_080283a6(int param_1,int param_2,int param_3,uint param_4,uint param_5,int param_6,
                 int param_7,int *param_8)

{
  short sVar2;
  short sVar1;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  
  iVar9 = param_1 + param_3;
  iVar10 = param_2 + param_3;
  iVar6 = param_1 - param_3;
  iVar8 = param_2 - param_3;
  if (param_5 == param_4 + 0x168) {
    *param_8 = iVar6;
    param_8[1] = iVar8;
    param_8[2] = iVar9;
    param_8[3] = iVar10;
  }
  else {
    if (0x168 < param_4) {
      param_4 = param_4 - 0x168 & 0xffff;
    }
    if (0x168 < param_5) {
      param_5 = param_5 - 0x168 & 0xffff;
    }
    iVar4 = param_3 - param_6;
    if (param_7 == 0) {
      iVar7 = 0;
    }
    else {
      iVar7 = param_6 / 2 + 1;
    }
    uVar3 = param_4 / 0x5a & 0xff;
    uVar5 = param_5 / 0x5a & 0xff;
    if (uVar3 == 4) {
      uVar3 = 3;
    }
    if (uVar5 == 4) {
      uVar5 = 3;
    }
    sVar1 = (short)param_4;
    sVar2 = (short)param_5;
    if ((uVar3 == uVar5) && (param_4 <= param_5)) {
      if (uVar3 == 0) {
        iVar6 = FUN_080338d4((int)sVar1);
        param_8[1] = (param_2 + (iVar4 * iVar6 >> 0xf)) - iVar7;
        iVar6 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
        param_8[2] = param_1 + (iVar6 * param_3 >> 0xf) + iVar7;
        iVar6 = FUN_080338d4((int)sVar2);
        param_8[3] = param_2 + (iVar6 * param_3 >> 0xf) + iVar7;
        iVar6 = FUN_080338d4((int)(short)(sVar2 + 0x5a));
        *param_8 = (param_1 + (iVar4 * iVar6 >> 0xf)) - iVar7;
      }
      else if (uVar3 == 1) {
        iVar6 = FUN_080338d4((int)sVar1);
        param_8[3] = param_2 + (iVar6 * param_3 >> 0xf) + iVar7;
        iVar6 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
        param_8[2] = param_1 + (iVar4 * iVar6 >> 0xf) + iVar7;
        iVar6 = FUN_080338d4((int)sVar2);
        param_8[1] = (param_2 + (iVar4 * iVar6 >> 0xf)) - iVar7;
        iVar6 = FUN_080338d4((int)(short)(sVar2 + 0x5a));
        *param_8 = (param_1 + (iVar6 * param_3 >> 0xf)) - iVar7;
      }
      else if (uVar3 == 2) {
        iVar6 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
        *param_8 = (param_1 + (iVar6 * param_3 >> 0xf)) - iVar7;
        iVar6 = FUN_080338d4((int)sVar1);
        param_8[3] = param_2 + (iVar4 * iVar6 >> 0xf) + iVar7;
        iVar6 = FUN_080338d4((int)sVar2);
        param_8[1] = (param_2 + (iVar6 * param_3 >> 0xf)) - iVar7;
        iVar6 = FUN_080338d4((int)(short)(sVar2 + 0x5a));
        param_8[2] = param_1 + (iVar4 * iVar6 >> 0xf) + iVar7;
      }
      else if (uVar3 == 3) {
        iVar6 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
        *param_8 = (param_1 + (iVar4 * iVar6 >> 0xf)) - iVar7;
        iVar6 = FUN_080338d4((int)sVar1);
        param_8[1] = (param_2 + (iVar6 * param_3 >> 0xf)) - iVar7;
        iVar6 = FUN_080338d4((int)(short)(sVar2 + 0x5a));
        param_8[2] = param_1 + (iVar6 * param_3 >> 0xf) + iVar7;
        iVar6 = FUN_080338d4((int)sVar2);
        param_8[3] = param_2 + (iVar4 * iVar6 >> 0xf) + iVar7;
      }
    }
    else if ((uVar3 == 0) && (uVar5 == 1)) {
      iVar6 = FUN_080338d4((int)(short)(sVar2 + 0x5a));
      *param_8 = (param_1 + (iVar6 * param_3 >> 0xf)) - iVar7;
      iVar6 = FUN_080338d4((int)sVar2);
      iVar8 = FUN_080338d4();
      if (iVar6 < iVar8) {
        iVar6 = FUN_080338d4((int)sVar2);
      }
      else {
        iVar6 = FUN_080338d4((int)sVar1);
      }
      param_8[1] = (param_2 + (iVar4 * iVar6 >> 0xf)) - iVar7;
      iVar6 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
      param_8[2] = param_1 + (iVar6 * param_3 >> 0xf) + iVar7;
      param_8[3] = iVar10 + iVar7;
    }
    else if ((uVar3 == 1) && (uVar5 == 2)) {
      *param_8 = iVar6 - iVar7;
      iVar6 = FUN_080338d4((int)sVar2);
      param_8[1] = (param_2 + (iVar6 * param_3 >> 0xf)) - iVar7;
      iVar6 = FUN_080338d4();
      iVar8 = FUN_080338d4((int)(short)(sVar2 + 0x5a));
      if (iVar8 < iVar6) {
        iVar6 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
      }
      else {
        iVar6 = FUN_080338d4((int)(short)(sVar2 + 0x5a));
      }
      param_8[2] = param_1 + (iVar4 * iVar6 >> 0xf) + iVar7;
      iVar6 = FUN_080338d4((int)sVar1);
      param_8[3] = param_2 + (iVar6 * param_3 >> 0xf) + iVar7;
    }
    else if ((uVar3 == 2) && (uVar5 == 3)) {
      iVar6 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
      *param_8 = (param_1 + (iVar6 * param_3 >> 0xf)) - iVar7;
      param_8[1] = iVar8 - iVar7;
      iVar6 = FUN_080338d4((int)(short)(sVar2 + 0x5a));
      param_8[2] = param_1 + (iVar6 * param_3 >> 0xf) + iVar7;
      iVar6 = FUN_080338d4((int)sVar2);
      iVar8 = FUN_080338d4((int)sVar1);
      if (iVar4 * iVar8 < iVar6 * iVar4) {
        iVar6 = FUN_080338d4((int)sVar2);
      }
      else {
        iVar6 = FUN_080338d4((int)sVar1);
      }
      param_8[3] = param_2 + (iVar4 * iVar6 >> 0xf) + iVar7;
    }
    else if ((uVar3 == 3) && (uVar5 == 0)) {
      iVar6 = FUN_080338d4();
      iVar8 = FUN_080338d4();
      if (iVar6 < iVar8) {
        iVar6 = FUN_080338d4((int)(short)(sVar2 + 0x5a));
      }
      else {
        iVar6 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
      }
      *param_8 = (param_1 + (iVar4 * iVar6 >> 0xf)) - iVar7;
      iVar6 = FUN_080338d4((int)sVar1);
      param_8[1] = (param_2 + (iVar6 * param_3 >> 0xf)) - iVar7;
      param_8[2] = iVar9 + iVar7;
      iVar6 = FUN_080338d4((int)sVar2);
      param_8[3] = param_2 + (iVar6 * param_3 >> 0xf) + iVar7;
    }
    else {
      *param_8 = iVar6;
      param_8[1] = iVar8;
      param_8[2] = iVar9;
      param_8[3] = iVar10;
    }
  }
  return;
}

