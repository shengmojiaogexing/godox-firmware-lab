// 0x08034ab8 FUN_08034ab8 size=360

void FUN_08034ab8(short *param_1,int *param_2,int param_3,short *param_4,int param_5,uint param_6,
                 byte *param_7,int param_8,int param_9)

{
  short sVar1;
  short sVar2;
  short sVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  code *pcVar8;
  int iVar9;
  uint uVar10;
  int local_3c;
  
  iVar9 = (param_2[2] - *param_2) + 1;
  iVar5 = (param_2[3] - param_2[1]) + 1;
  if (param_9 == 1) {
    pcVar8 = DAT_08034c24;
  }
  else if (param_9 == 2) {
    pcVar8 = DAT_08034c28;
  }
  else {
    if (param_9 != 3) {
      return;
    }
    pcVar8 = DAT_08034c20;
  }
  if (param_7 == (byte *)0x0) {
    sVar1 = *param_1;
    sVar2 = *param_4;
    sVar3 = (*pcVar8)(sVar2,sVar1,param_6);
    for (iVar7 = 0; iVar7 < iVar5; iVar7 = iVar7 + 1) {
      for (iVar6 = 0; iVar6 < iVar9; iVar6 = iVar6 + 1) {
        if ((sVar2 != param_4[iVar6]) || (sVar1 != param_1[iVar6])) {
          sVar1 = param_1[iVar6];
          sVar2 = param_4[iVar6];
          sVar3 = (*pcVar8)(sVar2,sVar1,param_6);
        }
        param_1[iVar6] = sVar3;
      }
      param_1 = param_1 + param_3;
      param_4 = param_4 + param_5;
    }
  }
  else {
    sVar1 = *param_1;
    sVar2 = *param_4;
    if (*param_7 < 0xfd) {
      uVar10 = *param_7 * param_6 >> 8;
    }
    else {
      uVar10 = param_6;
    }
    sVar3 = (*pcVar8)(sVar2,sVar1,uVar10);
    for (local_3c = 0; local_3c < iVar5; local_3c = local_3c + 1) {
      for (iVar7 = 0; iVar7 < iVar9; iVar7 = iVar7 + 1) {
        uVar4 = (uint)param_7[iVar7];
        if (uVar4 != 0) {
          if (uVar4 < 0xfd) {
            uVar4 = uVar4 * param_6 >> 8;
          }
          else {
            uVar4 = param_6;
          }
          if (((sVar2 != param_4[iVar7]) || (sVar1 != param_1[iVar7])) || (uVar10 != uVar4)) {
            sVar1 = param_1[iVar7];
            sVar2 = param_4[iVar7];
            sVar3 = (*pcVar8)(sVar2,sVar1,uVar4);
            uVar10 = uVar4;
          }
          param_1[iVar7] = sVar3;
        }
      }
      param_1 = param_1 + param_3;
      param_4 = param_4 + param_5;
      param_7 = param_7 + param_8;
    }
  }
  return;
}

