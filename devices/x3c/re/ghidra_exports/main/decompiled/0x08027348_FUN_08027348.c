// 0x08027348 FUN_08027348 size=120

void FUN_08027348(int param_1,uint param_2)

{
  short sVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  
  if (0x168 < param_2) {
    param_2 = param_2 - 0x168 & 0xffff;
  }
  sVar1 = *(short *)(param_1 + 0x2e);
  sVar2 = *(short *)(param_1 + 0x30) - sVar1;
  iVar4 = (int)sVar2;
  sVar3 = *(short *)(param_1 + 0x30) - (short)param_2;
  iVar5 = (int)sVar3;
  if (iVar4 < 0) {
    iVar4 = (int)(short)(sVar2 + 0x168);
  }
  if (iVar5 < 0) {
    iVar5 = (int)(short)(sVar3 + 0x168);
  }
  if ((iVar5 - iVar4) + 0xb4U < 0x169) {
    if (iVar5 < iVar4) {
      FUN_080250f4(param_1,sVar1,param_2,0x20000);
    }
    else if (iVar4 < iVar5) {
      FUN_080250f4(param_1,param_2,sVar1,0x20000);
    }
  }
  else {
    FUN_080302d0(param_1);
  }
  FUN_08025194(param_1);
  *(short *)(param_1 + 0x2e) = (short)param_2;
  FUN_08025194(param_1);
  return;
}

