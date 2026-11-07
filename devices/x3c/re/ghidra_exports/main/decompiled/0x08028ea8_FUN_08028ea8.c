// 0x08028ea8 FUN_08028ea8 size=704

int FUN_08028ea8(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  iVar2 = *(int *)(param_5 + 0xc);
  iVar5 = param_3 - iVar2;
  iVar4 = param_2 - *(int *)(param_5 + 8);
  iVar1 = *(int *)(param_5 + 0x10);
  iVar8 = param_5 + 0x18;
  iVar9 = param_5 + 0x50;
  if ((((iVar1 < 0xb4) && (iVar3 = *(int *)(param_5 + 0x14), iVar3 < 0xb4)) && (iVar1 != 0)) &&
     ((iVar3 != 0 && (iVar3 < iVar1)))) {
    if (param_3 < iVar2) {
      iVar1 = 1;
    }
    else {
      iVar2 = (iVar5 + 1) * *(int *)(param_5 + 0x3c) >> 10;
      if ((iVar1 - 0x10fU < 0x59) && (iVar2 < 0)) {
        iVar2 = 0;
      }
      else if ((iVar1 - 1U < 0x5a) && (iVar2 < 0)) {
        iVar2 = 0;
      }
      else if ((iVar1 - 0x5bU < 0xb3) && (0 < iVar2)) {
        iVar2 = 0;
      }
      if ((iVar3 - 0x10fU < 0x59) && (iVar2 < 0)) {
        iVar2 = 0;
      }
      else if ((iVar3 - 1U < 0x5a) && (iVar2 < 0)) {
        iVar2 = 0;
      }
      else if ((iVar3 - 0x5bU < 0xb3) && (0 < iVar2)) {
        iVar2 = 0;
      }
      iVar1 = 1;
      iVar4 = (((iVar5 * *(int *)(param_5 + 0x74) >> 10) - iVar2 >> 1) + iVar2) - iVar4;
      if (param_4 < iVar4) {
        iVar4 = param_4;
      }
      iVar2 = param_2;
      iVar5 = param_3;
      iVar3 = param_4;
      if ((0 < iVar4) &&
         (iVar1 = FUN_080293f0(param_1,param_2,param_3,iVar4,iVar8,param_1,param_2,param_3,param_4),
         iVar1 == 0)) {
        FUN_0802d8d6(param_1,iVar4);
      }
      if (param_4 < iVar4) {
        iVar4 = param_4;
      }
      if (iVar4 < 0) {
        iVar4 = 0;
      }
      iVar2 = FUN_080293f0(param_1 + iVar4,param_2 + iVar4,param_3,param_4 - iVar4,iVar9,param_1,
                           iVar2,iVar5,iVar3);
      if (iVar2 == 0) {
        FUN_0802d8d6(param_1 + iVar4,param_4 - iVar4);
      }
      if (iVar1 != iVar2) {
        iVar1 = 2;
      }
    }
  }
  else if (((iVar1 < 0xb5) || (iVar3 = *(int *)(param_5 + 0x14), iVar3 < 0xb5)) || (iVar1 <= iVar3))
  {
    iVar6 = 1;
    iVar7 = 1;
    iVar4 = param_2;
    iVar5 = param_3;
    iVar3 = param_4;
    if (iVar1 == 0xb4) {
      if (iVar2 <= param_3) {
        iVar6 = 3;
      }
    }
    else if (iVar1 == 0) {
      if (param_3 < iVar2) {
        iVar6 = 3;
      }
    }
    else if (((iVar1 < 0xb4) && (param_3 < iVar2)) || ((0xb4 < iVar1 && (iVar2 <= param_3)))) {
      iVar6 = 3;
    }
    else {
      iVar6 = FUN_080293f0(param_1,param_2,param_3,param_4,iVar8,param_1,param_2,param_3,param_4);
    }
    iVar1 = *(int *)(param_5 + 0x14);
    if (iVar1 == 0xb4) {
      if (param_3 < *(int *)(param_5 + 0xc)) {
        iVar7 = 3;
      }
    }
    else if (iVar1 == 0) {
      if (*(int *)(param_5 + 0xc) <= param_3) {
        iVar7 = 3;
      }
    }
    else if (((iVar1 < 0xb4) && (param_3 < *(int *)(param_5 + 0xc))) ||
            ((0xb4 < iVar1 && (*(int *)(param_5 + 0xc) <= param_3)))) {
      iVar7 = 3;
    }
    else {
      iVar7 = FUN_080293f0(param_1,param_2,param_3,param_4,iVar9,param_1,iVar4,iVar5,iVar3);
    }
    if ((iVar6 == 0) || (iVar7 == 0)) {
      iVar1 = 0;
    }
    else if ((iVar6 == 3) && (iVar7 == 3)) {
      iVar1 = 0;
    }
    else if ((iVar6 == 1) && (iVar7 == 1)) {
      iVar1 = 1;
    }
    else {
      iVar1 = 2;
    }
  }
  else if (iVar2 < param_3) {
    iVar1 = 1;
  }
  else {
    iVar2 = (iVar5 + 1) * *(int *)(param_5 + 0x3c) >> 10;
    if ((iVar1 - 0x10fU < 0x59) && (iVar2 < 0)) {
      iVar2 = 0;
    }
    else if ((iVar1 - 1U < 0x5a) && (iVar2 < 0)) {
      iVar2 = 0;
    }
    else if ((iVar1 - 0x5bU < 0xb3) && (0 < iVar2)) {
      iVar2 = 0;
    }
    if ((iVar3 - 0x10fU < 0x59) && (iVar2 < 0)) {
      iVar2 = 0;
    }
    else if ((iVar3 - 1U < 0x5a) && (iVar2 < 0)) {
      iVar2 = 0;
    }
    else if ((iVar3 - 0x5bU < 0xb3) && (0 < iVar2)) {
      iVar2 = 0;
    }
    iVar1 = 1;
    iVar4 = (((iVar5 * *(int *)(param_5 + 0x74) >> 10) - iVar2 >> 1) + iVar2) - iVar4;
    if (param_4 < iVar4) {
      iVar4 = param_4;
    }
    iVar2 = param_2;
    iVar5 = param_3;
    iVar3 = param_4;
    if ((0 < iVar4) &&
       (iVar1 = FUN_080293f0(param_1,param_2,param_3,iVar4,iVar9,param_1,param_2,param_3,param_4),
       iVar1 == 0)) {
      FUN_0802d8d6(param_1,iVar4);
    }
    if (param_4 < iVar4) {
      iVar4 = param_4;
    }
    if (iVar4 < 0) {
      iVar4 = 0;
    }
    iVar2 = FUN_080293f0(param_1 + iVar4,param_2 + iVar4,param_3,param_4 - iVar4,iVar8,param_1,iVar2
                         ,iVar5,iVar3);
    if (iVar2 == 0) {
      FUN_0802d8d6(param_1 + iVar4,param_4 - iVar4);
    }
    if (iVar1 != iVar2) {
      iVar1 = 2;
    }
  }
  return iVar1;
}

