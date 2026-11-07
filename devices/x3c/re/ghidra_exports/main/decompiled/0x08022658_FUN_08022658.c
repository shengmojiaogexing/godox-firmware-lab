// 0x08022658 FUN_08022658 size=122

void FUN_08022658(undefined4 param_1,int param_2,uint param_3,int *param_4)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  uVar2 = param_3 >> 1;
  iVar3 = ((int)(param_3 << 0x1f) >> 0x1f) + 1;
  iVar1 = FUN_080338d4((int)(short)(0x5a - (short)param_1));
  iVar4 = (int)(iVar1 * (param_2 - uVar2)) >> 7;
  iVar1 = FUN_080338d4(param_1);
  iVar1 = (int)(iVar1 * (param_2 - uVar2)) >> 7;
  if (iVar4 < 1) {
    iVar4 = iVar4 + 0x7f >> 8;
    *param_4 = iVar4 - uVar2;
    param_4[2] = (iVar4 + uVar2) - iVar3;
  }
  else {
    iVar4 = iVar4 + -0x7f >> 8;
    *param_4 = (iVar4 - uVar2) + iVar3;
    param_4[2] = iVar4 + uVar2;
  }
  if (iVar1 < 1) {
    iVar1 = iVar1 + 0x7f >> 8;
    param_4[1] = iVar1 - uVar2;
    param_4[3] = (iVar1 + uVar2) - iVar3;
  }
  else {
    iVar1 = iVar1 + -0x7f >> 8;
    param_4[1] = (iVar1 - uVar2) + iVar3;
    param_4[3] = iVar1 + uVar2;
  }
  return;
}

