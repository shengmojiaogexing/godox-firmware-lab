// 0x08022f40 FUN_08022f40 size=224

int FUN_08022f40(int param_1,int param_2,int param_3,int param_4,uint param_5,int param_6,
                int *param_7,int param_8)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  
  iVar5 = 0;
  uVar4 = param_5 - 1;
  if (param_2 == 0) {
    if ((((param_3 == 5) || (param_3 == 6)) || (param_3 == 4)) && (param_4 = 0, param_5 == 1)) {
      param_3 = 1;
    }
    for (uVar1 = 0; uVar1 < param_5; uVar1 = uVar1 + 1) {
      iVar5 = iVar5 + param_4 + *(int *)(param_6 + uVar1 * 4);
    }
    iVar5 = iVar5 - param_4;
    switch(param_3) {
    case 0:
      *param_7 = 0;
      break;
    case 1:
      *param_7 = (param_1 - iVar5) / 2;
      break;
    case 2:
      *param_7 = param_1 - iVar5;
      break;
    case 4:
      param_4 = (param_1 - iVar5) / (int)(param_5 + 1);
      *param_7 = param_4;
      break;
    case 5:
      param_4 = (param_1 - iVar5) / (int)param_5;
      *param_7 = param_4 / 2;
      break;
    case 6:
      *param_7 = 0;
      param_4 = (param_1 - iVar5) / (int)uVar4;
    }
  }
  else {
    *param_7 = 0;
  }
  for (uVar1 = 0; uVar1 < uVar4; uVar1 = uVar1 + 1) {
    param_7[uVar1 + 1] = param_7[uVar1] + *(int *)(param_6 + uVar1 * 4) + param_4;
  }
  iVar5 = param_7[uVar4];
  iVar2 = *(int *)(param_6 + uVar4 * 4);
  iVar3 = *param_7;
  if (param_8 != 0) {
    for (uVar4 = 0; uVar4 < param_5; uVar4 = uVar4 + 1) {
      param_7[uVar4] = (param_1 - param_7[uVar4]) - *(int *)(param_6 + uVar4 * 4);
    }
  }
  return (iVar5 + iVar2) - iVar3;
}

