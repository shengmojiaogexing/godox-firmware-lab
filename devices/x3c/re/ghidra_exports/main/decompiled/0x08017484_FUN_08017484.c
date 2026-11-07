// 0x08017484 FUN_08017484 size=64

undefined4 FUN_08017484(int *param_1,int *param_2,int *param_3)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = *param_3;
  if (*param_3 < *param_2) {
    iVar3 = *param_2;
  }
  *param_1 = iVar3;
  iVar4 = param_2[1];
  if (param_2[1] <= param_3[1]) {
    iVar4 = param_3[1];
  }
  param_1[1] = iVar4;
  iVar5 = param_3[2];
  if (param_2[2] < param_3[2]) {
    iVar5 = param_2[2];
  }
  param_1[2] = iVar5;
  iVar2 = param_2[3];
  if (param_3[3] <= param_2[3]) {
    iVar2 = param_3[3];
  }
  param_1[3] = iVar2;
  uVar1 = 1;
  if ((iVar5 < iVar3) || (iVar2 < iVar4)) {
    uVar1 = 0;
  }
  return uVar1;
}

