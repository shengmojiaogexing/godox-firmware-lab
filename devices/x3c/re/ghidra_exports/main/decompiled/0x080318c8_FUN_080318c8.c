// 0x080318c8 FUN_080318c8 size=316

void FUN_080318c8(int *param_1,int param_2,int param_3,int *param_4)

{
  short sVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  if ((param_2 != 0) || (param_3 != 0x100)) {
    iVar3 = *param_1;
    iVar6 = *param_4;
    *param_1 = iVar3 - iVar6;
    iVar4 = param_1[1];
    iVar7 = param_4[1];
    param_1[1] = iVar4 - iVar7;
    piVar2 = DAT_08031a04;
    if (param_2 == 0) {
      *param_1 = *param_4 + (param_3 * (iVar3 - iVar6) >> 8);
      param_1[1] = param_4[1] + (param_3 * (iVar4 - iVar7) >> 8);
    }
    else {
      if (*DAT_08031a04 != param_2) {
        iVar3 = param_2;
        if (0xe10 < param_2) {
          iVar3 = param_2 + -0xe10;
        }
        if (iVar3 < 0) {
          iVar3 = iVar3 + 0xe10;
        }
        sVar1 = (short)(iVar3 / 10);
        iVar3 = iVar3 % 10;
        iVar4 = FUN_080338d4((int)sVar1);
        iVar6 = FUN_080338d4((int)(short)(sVar1 + 1));
        iVar7 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
        iVar5 = FUN_080338d4((int)(short)(sVar1 + 0x5b));
        piVar2[1] = (iVar6 * iVar3 + (10 - iVar3) * iVar4) / 10 >> 5;
        piVar2[2] = (iVar5 * iVar3 + (10 - iVar3) * iVar7) / 10 >> 5;
        *piVar2 = param_2;
      }
      iVar3 = *param_1;
      iVar4 = param_1[1];
      if (param_3 == 0x100) {
        iVar6 = piVar2[2];
        iVar7 = piVar2[1];
        *param_1 = *param_4 + (iVar6 * iVar3 - iVar7 * iVar4 >> 10);
        param_1[1] = param_4[1] + (iVar6 * iVar4 + iVar3 * iVar7 >> 10);
      }
      else {
        iVar6 = piVar2[2];
        iVar7 = piVar2[1];
        *param_1 = *param_4 + (param_3 * (iVar6 * iVar3 - iVar7 * iVar4) >> 0x12);
        param_1[1] = param_4[1] + (param_3 * (iVar6 * iVar4 + iVar3 * iVar7) >> 0x12);
      }
    }
  }
  return;
}

