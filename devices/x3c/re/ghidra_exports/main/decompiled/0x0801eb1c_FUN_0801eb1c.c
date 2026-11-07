// 0x0801eb1c FUN_0801eb1c size=462

undefined8 FUN_0801eb1c(int *param_1,int param_2,undefined4 param_3,int param_4)

{
  short sVar1;
  short sVar2;
  int iVar3;
  undefined4 uVar4;
  int *local_20;
  int local_1c;
  int local_18;
  int local_14;
  
  uVar4 = *(undefined4 *)(param_1[7] + 8);
  sVar1 = (short)param_1[3];
  local_20 = param_1;
  local_1c = param_2;
  local_18 = param_3;
  local_14 = param_4;
  if (((sVar1 == 0) && (*(short *)((int)param_1 + 0xe) == 0)) &&
     (*(ushort *)(param_1 + 2) < *(ushort *)((int)param_1 + 10))) {
    iVar3 = FUN_080338d4((int)(short)param_1[2]);
    local_1c = *(int *)(*param_1 + 4) + ((param_1[1] - param_1[4]) * iVar3 >> 0xf);
    iVar3 = FUN_080338d4((int)(short)((short)param_1[2] + 0x5a));
    local_18 = *(int *)*param_1 + (param_1[1] * iVar3 >> 0xf);
    iVar3 = FUN_080338d4((int)*(short *)((int)param_1 + 10));
    local_14 = *(int *)(*param_1 + 4) + (param_1[1] * iVar3 >> 0xf);
    iVar3 = FUN_080338d4((int)(short)(*(short *)((int)param_1 + 10) + 0x5a));
    local_20 = (int *)(*(int *)*param_1 + ((param_1[1] - param_1[4]) * iVar3 >> 0xf));
    iVar3 = FUN_08017484(&local_20,&local_20,uVar4);
    if (iVar3 != 0) {
      *(int ***)(param_1[7] + 8) = &local_20;
      FUN_08029a14(param_1[7],param_1[5],param_1[6]);
    }
  }
  else if ((sVar1 == 0) || (sVar2 = *(short *)((int)param_1 + 0xe), sVar2 == 0)) {
    if (sVar1 == 0) {
      local_20 = *(int **)*param_1;
      local_14 = *(int *)(*param_1 + 4) + param_1[1];
      iVar3 = FUN_080338d4((int)(short)param_1[2]);
      local_1c = *(int *)(*param_1 + 4) + ((param_1[1] - param_1[4]) * iVar3 >> 0xf);
      iVar3 = FUN_080338d4((int)(short)((short)param_1[2] + 0x5a));
      local_18 = *(int *)*param_1 + (param_1[1] * iVar3 >> 0xf);
      iVar3 = FUN_08017484(&local_20,&local_20,uVar4);
      if (iVar3 != 0) {
        *(int ***)(param_1[7] + 8) = &local_20;
        FUN_08029a14(param_1[7],param_1[5],param_1[6]);
      }
    }
    if (*(short *)((int)param_1 + 0xe) == 0) {
      local_18 = *(int *)*param_1 + param_1[1];
      local_1c = *(int *)(*param_1 + 4);
      iVar3 = FUN_080338d4((int)*(short *)((int)param_1 + 10));
      local_14 = *(int *)(*param_1 + 4) + (param_1[1] * iVar3 >> 0xf);
      iVar3 = FUN_080338d4((int)(short)(*(short *)((int)param_1 + 10) + 0x5a));
      local_20 = (int *)(*(int *)*param_1 + ((param_1[1] - param_1[4]) * iVar3 >> 0xf));
      iVar3 = FUN_08017484(&local_20,&local_20,uVar4);
      if (iVar3 != 0) {
        *(int ***)(param_1[7] + 8) = &local_20;
        FUN_08029a14(param_1[7],param_1[5],param_1[6]);
      }
    }
  }
  else if (((((sVar1 == sVar2) && (*(ushort *)((int)param_1 + 10) < *(ushort *)(param_1 + 2))) ||
            ((sVar1 == 2 && (sVar2 == 1)))) || ((sVar1 == 3 && (sVar2 == 2)))) ||
          ((sVar1 == 3 && (sVar2 == 1)))) {
    local_20 = *(int **)*param_1;
    local_1c = *(int *)(*param_1 + 4);
    local_18 = *(int *)*param_1 + param_1[1];
    local_14 = *(int *)(*param_1 + 4) + param_1[1];
    iVar3 = FUN_08017484(&local_20,&local_20,uVar4);
    if (iVar3 != 0) {
      *(int ***)(param_1[7] + 8) = &local_20;
      FUN_08029a14(param_1[7],param_1[5],param_1[6]);
    }
  }
  *(undefined4 *)(param_1[7] + 8) = uVar4;
  return CONCAT44(local_1c,local_20);
}

