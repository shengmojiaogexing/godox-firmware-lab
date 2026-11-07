// 0x0801d0c4 FUN_0801d0c4 size=98

void FUN_0801d0c4(int *param_1,int *param_2,int param_3)

{
  undefined2 uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  undefined2 *puVar5;
  undefined2 *puVar6;
  
  uVar3 = ((param_2[3] - param_2[1]) + 1) * ((param_2[2] - *param_2) + 1);
  for (uVar4 = 0; uVar3 = uVar3 - 1, uVar4 < uVar3; uVar4 = uVar4 + 1) {
    puVar5 = (undefined2 *)(param_3 + uVar3 * 2);
    uVar1 = *puVar5;
    puVar6 = (undefined2 *)(param_3 + uVar4 * 2);
    *puVar5 = *puVar6;
    *puVar6 = uVar1;
  }
  iVar2 = param_2[3];
  param_2[3] = (param_1[1] - param_2[1]) + -1;
  param_2[1] = (param_1[1] - iVar2) + -1;
  iVar2 = param_2[2];
  param_2[2] = (*param_1 - *param_2) + -1;
  *param_2 = (*param_1 - iVar2) + -1;
  return;
}

