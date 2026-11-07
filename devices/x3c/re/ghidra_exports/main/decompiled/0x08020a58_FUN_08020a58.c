// 0x08020a58 FUN_08020a58 size=292

void FUN_08020a58(short *param_1,int *param_2,int param_3,undefined4 param_4,uint param_5,
                 byte *param_6,int param_7,int param_8)

{
  short sVar1;
  short sVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  code *pcVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  
  iVar7 = (param_2[2] - *param_2) + 1;
  iVar4 = (param_2[3] - param_2[1]) + 1;
  if (param_8 == 1) {
    pcVar6 = DAT_08020b80;
  }
  else if (param_8 == 2) {
    pcVar6 = DAT_08020b84;
  }
  else {
    if (param_8 != 3) {
      return;
    }
    pcVar6 = DAT_08020b7c;
  }
  if (param_6 == (byte *)0x0) {
    sVar1 = *param_1;
    sVar2 = (*pcVar6)(param_4,sVar1,param_5);
    for (iVar8 = 0; iVar8 < iVar4; iVar8 = iVar8 + 1) {
      for (iVar5 = 0; iVar5 < iVar7; iVar5 = iVar5 + 1) {
        if (sVar1 != param_1[iVar5]) {
          sVar1 = param_1[iVar5];
          sVar2 = (*pcVar6)(param_4,sVar1,param_5);
        }
        param_1[iVar5] = sVar2;
      }
      param_1 = param_1 + param_3;
    }
  }
  else {
    uVar9 = 0;
    sVar1 = *param_1;
    if (*param_6 < 0xfd) {
      uVar3 = *param_6 * param_5 >> 8;
    }
    else {
      uVar3 = param_5;
    }
    sVar2 = (*pcVar6)(param_4,sVar1,uVar3);
    for (iVar8 = 0; iVar8 < iVar4; iVar8 = iVar8 + 1) {
      for (iVar5 = 0; iVar5 < iVar7; iVar5 = iVar5 + 1) {
        uVar3 = (uint)param_6[iVar5];
        if (uVar3 != 0) {
          if ((uVar3 != uVar9) || (sVar1 != param_1[iVar5])) {
            if (uVar3 < 0xfd) {
              uVar9 = uVar3 * param_5 >> 8;
            }
            else {
              uVar9 = param_5;
            }
            sVar2 = (*pcVar6)(param_4,param_1[iVar5],uVar9);
            uVar9 = (uint)param_6[iVar5];
            sVar1 = param_1[iVar5];
          }
          param_1[iVar5] = sVar2;
        }
      }
      param_1 = param_1 + param_3;
      param_6 = param_6 + param_7;
    }
  }
  return;
}

