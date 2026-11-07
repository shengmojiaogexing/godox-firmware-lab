// 0x08024a74 FUN_08024a74 size=168

void FUN_08024a74(int *param_1,int *param_2)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  
  piVar1 = *(int **)(*param_1 + 0x10);
  param_1[8] = *param_2;
  param_1[9] = param_2[1];
  uVar2 = (*(byte *)((int *)*piVar1 + 7) & 0x3f) >> 4;
  if ((uVar2 == 2) || (uVar2 == 3)) {
    *param_2 = (*(int *)*piVar1 - *param_2) + -1;
    param_2[1] = (*(int *)(*piVar1 + 4) - param_2[1]) + -1;
  }
  uVar2 = (*(byte *)(*piVar1 + 0x1c) & 0x3f) >> 4;
  if ((uVar2 == 1) || (uVar2 == 3)) {
    iVar3 = param_2[1];
    param_2[1] = *param_2;
    *param_2 = (*(int *)(*piVar1 + 4) - iVar3) + -1;
  }
  FUN_08028248(*(undefined4 *)(*param_1 + 0x10));
  FUN_080282e6(*(undefined4 *)(*param_1 + 0x10));
  if ((param_1[0x1f] != 0) && ((param_1[6] != *param_2 || (param_1[7] != param_2[1])))) {
    FUN_08031318(param_1[0x1f],*param_2,param_2[1]);
  }
  param_1[2] = *param_2;
  param_1[3] = param_2[1];
  if ((char)param_1[1] == '\x01') {
    FUN_08024b1c(param_1 + 1);
  }
  else {
    FUN_08024d38(param_1 + 1);
  }
  param_1[6] = param_1[2];
  param_1[7] = param_1[3];
  return;
}

