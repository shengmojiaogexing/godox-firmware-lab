// 0x0801b8cc FUN_0801b8cc size=398

void FUN_0801b8cc(int *param_1,int param_2,int param_3,int param_4,int param_5,undefined4 param_6,
                 int param_7,int param_8,int param_9)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  undefined2 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  if ((param_7 == 4) || (param_7 == 6)) {
    uVar1 = FUN_080274b0(param_1);
    FUN_0802d926(param_9,uVar1);
    iVar6 = param_1[1];
    param_2 = iVar6 * param_5 * 2 + *param_1 * 2 + param_2;
    iVar7 = (param_1[2] - *param_1) + 1;
    iVar5 = param_8;
    for (; iVar6 <= param_1[3]; iVar6 = iVar6 + 1) {
      FUN_0802d764(iVar5,param_2,iVar7 * 2);
      param_2 = param_2 + param_5 * 2;
      iVar5 = iVar5 + iVar7 * 2;
    }
    if (param_7 == 6) {
      for (uVar2 = 0; uVar2 < uVar1; uVar2 = uVar2 + 1) {
        if (*(short *)(param_8 + uVar2 * 2) == 0) {
          *(undefined1 *)(param_9 + uVar2) = 0;
        }
      }
    }
  }
  else if (param_7 == 5) {
    iVar7 = param_1[1];
    puVar4 = (undefined2 *)(iVar7 * param_5 * 3 + *param_1 * 3 + param_2);
    iVar6 = (param_1[2] - *param_1) + 1;
    iVar5 = param_1[3];
    for (iVar8 = 0; iVar8 < (iVar5 - iVar7) + 1; iVar8 = iVar8 + 1) {
      for (iVar3 = 0; iVar3 < iVar6; iVar3 = iVar3 + 1) {
        *(undefined1 *)(param_9 + iVar3) = *(undefined1 *)(puVar4 + 1);
        *(undefined2 *)(param_8 + iVar3 * 2) = *puVar4;
        puVar4 = (undefined2 *)((int)puVar4 + 3);
      }
      param_8 = param_8 + iVar6 * 2;
      param_9 = param_9 + iVar6;
      puVar4 = (undefined2 *)((int)puVar4 + (param_5 - iVar6) * 3);
    }
  }
  else if (param_7 == 0x14) {
    iVar8 = param_2 + param_1[1] * param_5 * 2 + *param_1 * 2;
    iVar7 = (param_1[3] - param_1[1]) + 1;
    iVar6 = (param_1[2] - *param_1) + 1;
    for (iVar5 = 0; iVar5 < iVar7; iVar5 = iVar5 + 1) {
      FUN_0802d764(param_8,iVar8,iVar6 * 2);
      param_8 = param_8 + iVar6 * 2;
      iVar8 = iVar8 + param_5 * 2;
    }
    iVar8 = param_5 * param_1[1] + *param_1 + param_3 * 2 * param_4 + param_2;
    for (iVar5 = 0; iVar5 < iVar7; iVar5 = iVar5 + 1) {
      FUN_0802d764(param_9,iVar8,iVar6);
      param_9 = param_9 + iVar6;
      iVar8 = iVar8 + param_5;
    }
  }
  return;
}

