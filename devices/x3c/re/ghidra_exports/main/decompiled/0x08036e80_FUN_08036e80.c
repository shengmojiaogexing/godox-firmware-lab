// 0x08036e80 FUN_08036e80 size=646

void FUN_08036e80(int *param_1,int param_2,int *param_3,int param_4)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int *piVar10;
  int iVar11;
  
  iVar1 = FUN_0802ee0a(param_2);
  iVar2 = FUN_0802fc9e(iVar1,0x10);
  if (iVar2 != 0) {
    uVar3 = FUN_0802eec8(iVar1);
    iVar2 = 0;
    iVar4 = FUN_0802f0a8(iVar1);
    if (iVar4 == 0) {
      piVar10 = param_1;
    }
    else {
      piVar10 = (int *)(param_2 + 0x14);
    }
    iVar5 = FUN_0802f86a(iVar1,0);
    iVar6 = FUN_0802f99e(iVar1,0);
    iVar6 = iVar6 + iVar5;
    iVar7 = FUN_0802f908(iVar1,0);
    iVar7 = iVar7 + iVar5;
    iVar11 = ((*(int *)(iVar1 + 0x18) + iVar6) - piVar10[1]) - param_3[1];
    iVar8 = param_3[1] - ((*(int *)(iVar1 + 0x20) - iVar7) - piVar10[3]);
    iVar9 = FUN_0802ed88(iVar1);
    if ((iVar11 < 0) || (iVar8 < 0)) {
      if (iVar11 < 1) {
        if (0 < iVar8) {
          iVar2 = -iVar8;
          iVar8 = FUN_0802ee28(iVar1);
          if (iVar8 + iVar2 < 0) {
            iVar2 = 0;
          }
        }
      }
      else {
        iVar8 = FUN_0802f0bc(iVar1);
        iVar2 = iVar11;
        if (iVar8 - iVar11 < 0) {
          iVar2 = 0;
        }
      }
    }
    if (iVar4 == 1) {
      iVar2 = (*(int *)(iVar1 + 0x18) + iVar6) - piVar10[1];
    }
    else if (iVar4 == 2) {
      iVar2 = (*(int *)(iVar1 + 0x20) - iVar7) - piVar10[3];
    }
    else if (iVar4 == 3) {
      iVar2 = iVar6 + ((iVar9 - iVar6) - iVar7) / 2 + *(int *)(iVar1 + 0x18) +
              (-(((piVar10[3] - piVar10[1]) + 1) / 2) - piVar10[1]);
    }
    iVar4 = 0;
    iVar6 = FUN_0802f094(iVar1);
    if (iVar6 != 0) {
      param_1 = (int *)(param_2 + 0x14);
    }
    iVar7 = FUN_0802f93e(iVar1,0);
    iVar7 = iVar7 + iVar5;
    iVar8 = FUN_0802f968(iVar1,0);
    iVar5 = iVar5 + iVar8;
    iVar8 = ((*(int *)(iVar1 + 0x14) + iVar7) - *param_1) - *param_3;
    iVar9 = *param_3 - ((*(int *)(iVar1 + 0x1c) - iVar5) - param_1[2]);
    if ((iVar8 < 0) || (iVar9 < 0)) {
      if (iVar8 < 1) {
        if (0 < iVar9) {
          iVar4 = -iVar9;
          iVar8 = FUN_0802efdc(iVar1);
          if (iVar8 + iVar4 < 0) {
            iVar4 = 0;
          }
        }
      }
      else {
        iVar9 = FUN_0802ef1c(iVar1);
        iVar4 = iVar8;
        if (iVar9 - iVar8 < 0) {
          iVar4 = 0;
        }
      }
    }
    iVar8 = FUN_0802fc5e(iVar1);
    if (iVar6 == 1) {
      iVar4 = (*(int *)(iVar1 + 0x14) + iVar7) - *param_1;
    }
    else if (iVar6 == 2) {
      iVar4 = (*(int *)(iVar1 + 0x1c) - iVar5) - param_1[2];
    }
    else if (iVar6 == 3) {
      iVar4 = iVar7 + ((iVar8 - iVar7) - iVar5) / 2 + *(int *)(iVar1 + 0x14) +
              (-(((param_1[2] - *param_1) + 1) / 2) - *param_1);
    }
    iVar5 = FUN_08026938(iVar1,DAT_08037108);
    iVar6 = FUN_08026938(iVar1,DAT_0803710c);
    if ((iVar5 == 0 && iVar6 == 0) || (iVar5 = FUN_0802ad74(iVar1,10,0), iVar5 == 1)) {
      if (((uVar3 & 1) == 0) && (iVar4 < 0)) {
        iVar4 = 0;
      }
      if ((-1 < (int)(uVar3 << 0x1e)) && (0 < iVar4)) {
        iVar4 = 0;
      }
      if ((-1 < (int)(uVar3 << 0x1d)) && (iVar2 < 0)) {
        iVar2 = 0;
      }
      if ((-1 < (int)(uVar3 << 0x1c)) && (0 < iVar2)) {
        iVar2 = 0;
      }
      if (param_4 == 0) {
        iVar5 = 0;
      }
      else {
        iVar5 = iVar4;
      }
      *param_3 = iVar5 + *param_3;
      if (param_4 == 0) {
        iVar5 = 0;
      }
      else {
        iVar5 = iVar2;
      }
      param_3[1] = iVar5 + param_3[1];
      FUN_08030f80(iVar1,iVar4,iVar2,param_4);
      return;
    }
  }
  return;
}

