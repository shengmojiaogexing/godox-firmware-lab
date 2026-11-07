// 0x0801dfe4 FUN_0801dfe4 size=318

void FUN_0801dfe4(int param_1,undefined2 *param_2,int *param_3,int *param_4)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int *local_50 [2];
  undefined2 local_48;
  int local_44;
  undefined1 local_40;
  int *local_3c;
  undefined1 local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  
  local_28 = (int)(*(int *)(param_2 + 2) - 1U) >> 1;
  local_34 = *param_3;
  if (*param_4 <= *param_3) {
    local_34 = *param_4;
  }
  local_2c = *param_3;
  if (*param_3 <= *param_4) {
    local_2c = *param_4;
  }
  local_2c = local_2c + -1;
  local_30 = param_3[1] - ((*(int *)(param_2 + 2) - 1U & 1) + local_28);
  local_28 = param_3[1] + local_28;
  iVar3 = FUN_08017484(&local_34,&local_34,*(undefined4 *)(param_1 + 8));
  if (iVar3 != 0) {
    if ((*(int *)(param_2 + 6) == 0) || (*(int *)(param_2 + 4) == 0)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    bVar2 = true;
    iVar3 = FUN_08029394(&local_34);
    if (iVar3 == 0) {
      if (bVar1) {
        bVar2 = false;
      }
    }
    else {
      bVar2 = false;
    }
    FUN_0802d8d6(local_50,0x1c);
    iVar3 = local_28;
    local_3c = &local_34;
    local_48 = *param_2;
    local_38 = *(undefined1 *)(param_2 + 8);
    local_50[0] = local_3c;
    if (bVar2) {
      FUN_08029de4(param_1,local_50);
    }
    else {
      iVar10 = (local_2c - local_34) + 1;
      local_28 = local_30;
      iVar9 = 0;
      if (bVar1) {
        iVar9 = local_34 -
                (*(int *)(param_2 + 6) + *(int *)(param_2 + 4)) *
                (local_34 / (*(int *)(param_2 + 6) + *(int *)(param_2 + 4)));
      }
      iVar4 = FUN_0802d624(iVar10);
      local_44 = iVar4;
      for (iVar8 = local_30; iVar8 <= iVar3; iVar8 = iVar8 + 1) {
        FUN_0802d926(iVar4,iVar10);
        iVar5 = FUN_08029220(iVar4,local_34,iVar8,iVar10);
        local_40 = (undefined1)iVar5;
        if ((bVar1) && (iVar5 != 0)) {
          iVar5 = iVar9;
          for (iVar6 = 0; iVar6 < iVar10; iVar6 = iVar6 + 1) {
            iVar7 = *(int *)(param_2 + 4);
            if (iVar7 < iVar5) {
              if (iVar5 < iVar7 + *(int *)(param_2 + 6)) {
                *(undefined1 *)(iVar4 + iVar6) = 0;
              }
              else {
                iVar5 = 0;
              }
            }
            else {
              iVar7 = (int)(short)((short)iVar7 - (short)iVar5);
              iVar6 = iVar6 + iVar7;
              iVar5 = iVar5 + iVar7;
            }
            iVar5 = iVar5 + 1;
          }
          local_40 = 2;
        }
        FUN_08029de4(param_1,local_50);
        local_30 = local_30 + 1;
        local_28 = local_28 + 1;
      }
      FUN_0802d6c0(iVar4);
    }
  }
  return;
}

