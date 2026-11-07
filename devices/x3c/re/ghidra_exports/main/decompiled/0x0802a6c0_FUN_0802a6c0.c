// 0x0802a6c0 FUN_0802a6c0 size=362

void FUN_0802a6c0(int param_1,undefined2 *param_2,int *param_3,int *param_4)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  undefined4 local_7c;
  undefined1 local_77;
  undefined2 local_76;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  
  if ((*(int *)(param_2 + 2) != 0) && (2 < *(byte *)(param_2 + 8))) {
    local_30 = *param_3;
    iVar1 = *param_4;
    if ((local_30 != iVar1) || (param_3[1] != param_4[1])) {
      if (iVar1 <= local_30) {
        local_30 = iVar1;
      }
      local_30 = local_30 - *(int *)(param_2 + 2) / 2;
      local_28 = *param_3;
      if (*param_3 <= *param_4) {
        local_28 = *param_4;
      }
      local_28 = local_28 + *(int *)(param_2 + 2) / 2;
      local_2c = param_3[1];
      if (param_4[1] <= param_3[1]) {
        local_2c = param_4[1];
      }
      local_2c = local_2c - *(int *)(param_2 + 2) / 2;
      local_24 = param_3[1];
      if (param_3[1] <= param_4[1]) {
        local_24 = param_4[1];
      }
      local_24 = local_24 + *(int *)(param_2 + 2) / 2;
      iVar1 = FUN_08017484(&local_30,&local_30,*(undefined4 *)(param_1 + 8));
      if (iVar1 != 0) {
        uVar3 = *(undefined4 *)(param_1 + 8);
        *(int **)(param_1 + 8) = &local_30;
        if (param_3[1] == param_4[1]) {
          FUN_0801dfe4(param_1,param_2,param_3,param_4);
        }
        else if (*param_3 == *param_4) {
          FUN_0801e440(param_1,param_2,param_3,param_4);
        }
        else {
          FUN_0801e124(param_1,param_2,param_3,param_4);
        }
        if (((int)((uint)*(byte *)((int)param_2 + 0x11) << 0x1c) < 0) ||
           ((int)((uint)*(byte *)((int)param_2 + 0x11) << 0x1d) < 0)) {
          FUN_08029a38(&local_7c);
          local_76 = *param_2;
          local_7c = 0x7fff;
          local_77 = *(undefined1 *)(param_2 + 8);
          iVar1 = *(int *)(param_2 + 2) >> 1;
          iVar2 = ((*(int *)(param_2 + 2) << 0x1f) >> 0x1f) + 1;
          if ((int)((uint)*(byte *)((int)param_2 + 0x11) << 0x1d) < 0) {
            local_8c = *param_3 - iVar1;
            local_88 = param_3[1] - iVar1;
            local_84 = (*param_3 + iVar1) - iVar2;
            local_80 = (param_3[1] + iVar1) - iVar2;
            FUN_08029a14(param_1,&local_7c,&local_8c);
          }
          if ((int)((uint)*(byte *)((int)param_2 + 0x11) << 0x1c) < 0) {
            local_8c = *param_4 - iVar1;
            local_88 = param_4[1] - iVar1;
            local_84 = (*param_4 + iVar1) - iVar2;
            local_80 = (param_4[1] + iVar1) - iVar2;
            FUN_08029a14(param_1,&local_7c,&local_8c);
          }
        }
        *(undefined4 *)(param_1 + 8) = uVar3;
      }
    }
  }
  return;
}

