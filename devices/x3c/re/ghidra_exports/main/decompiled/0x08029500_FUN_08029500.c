// 0x08029500 FUN_08029500 size=320

void FUN_08029500(undefined4 *param_1,int param_2,int param_3,int param_4,int param_5,uint param_6)

{
  int iVar1;
  int iVar2;
  int iVar3;
  byte bVar4;
  int iVar5;
  
  FUN_0802d8d6(param_1,0x38);
  if ((param_3 == param_5) && (param_6 == 3)) {
    param_3 = param_3 + -1;
    param_5 = param_5 + -1;
  }
  iVar5 = param_3;
  iVar3 = param_2;
  if (param_5 < param_3) {
    iVar5 = param_5;
    param_5 = param_3;
    iVar3 = param_4;
    param_4 = param_2;
  }
  param_1[2] = iVar3;
  param_1[3] = iVar5;
  param_1[4] = param_4;
  param_1[5] = param_5;
  *(char *)(param_1 + 6) = (char)param_6;
  param_1[7] = iVar3;
  param_1[8] = iVar5;
  iVar1 = param_4 - iVar3;
  if (iVar1 < 1) {
    iVar3 = iVar3 - param_4;
  }
  else {
    iVar3 = iVar1;
  }
  iVar2 = param_5 - iVar5;
  if (iVar2 < 1) {
    iVar5 = iVar5 - param_5;
  }
  else {
    iVar5 = iVar2;
  }
  if (iVar5 < iVar3) {
    bVar4 = *(byte *)(param_1 + 0xd) | 1;
  }
  else {
    bVar4 = *(byte *)(param_1 + 0xd) & 0xfe;
  }
  *(byte *)(param_1 + 0xd) = bVar4;
  param_1[10] = 0;
  param_1[9] = 0;
  *param_1 = DAT_08029640;
  *(undefined1 *)(param_1 + 1) = 0;
  if ((bVar4 & 1) == 0) {
    if (iVar2 != 0) {
      param_1[9] = iVar1 * (0x100000 / iVar2) >> 10;
    }
    if (iVar1 != 0) {
      param_1[10] = iVar2 * (0x100000 / iVar1) >> 10;
    }
    param_1[0xb] = param_1[9];
  }
  else {
    if (iVar1 != 0) {
      param_1[10] = iVar2 * (0x100000 / iVar1) >> 10;
    }
    if (iVar2 != 0) {
      param_1[9] = iVar1 * (0x100000 / iVar2) >> 10;
    }
    param_1[0xb] = param_1[10];
  }
  if ((param_6 & 3) == 0) {
    *(byte *)(param_1 + 0xd) = bVar4 & 0xfd;
  }
  else if ((param_6 & 3) == 1) {
    *(byte *)(param_1 + 0xd) = bVar4 | 2;
  }
  else if ((param_6 & 3) == 2) {
    if ((int)param_1[0xb] < 1) {
      *(byte *)(param_1 + 0xd) = bVar4 & 0xfd;
    }
    else {
      *(byte *)(param_1 + 0xd) = bVar4 | 2;
    }
  }
  else if ((~param_6 & 3) == 0) {
    if ((int)param_1[0xb] < 1) {
      *(byte *)(param_1 + 0xd) = bVar4 | 2;
    }
    else {
      *(byte *)(param_1 + 0xd) = bVar4 & 0xfd;
    }
  }
  iVar3 = (int)param_1[0xb] >> 2;
  param_1[0xc] = iVar3;
  if ((int)param_1[0xb] < 0) {
    param_1[0xc] = -iVar3;
  }
  return;
}

