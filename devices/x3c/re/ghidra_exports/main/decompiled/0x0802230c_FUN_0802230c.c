// 0x0802230c FUN_0802230c size=168

void FUN_0802230c(undefined4 param_1,int *param_2,int param_3,int *param_4)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar2 = FUN_0802f7da(param_1,0x20000);
  iVar2 = iVar2 / 2;
  sVar1 = FUN_08021eb0(param_1);
  iVar3 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
  iVar8 = iVar3 * (param_3 - iVar2) >> 0xf;
  iVar3 = FUN_080338d4((int)sVar1);
  iVar3 = iVar3 * (param_3 - iVar2) >> 0xf;
  iVar4 = FUN_0802f944(param_1,0x30000);
  iVar5 = FUN_0802f96e(param_1,0x30000);
  iVar6 = FUN_0802f9a4(param_1,0x30000);
  iVar7 = FUN_0802f90e(param_1,0x30000);
  *param_4 = ((*param_2 + iVar8) - iVar4) - iVar2;
  param_4[2] = *param_2 + iVar8 + iVar5 + iVar2;
  param_4[1] = ((param_2[1] + iVar3) - iVar6) - iVar2;
  param_4[3] = iVar7 + iVar2 + param_2[1] + iVar3;
  return;
}

