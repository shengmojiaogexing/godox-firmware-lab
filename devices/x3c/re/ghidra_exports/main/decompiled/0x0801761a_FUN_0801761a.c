// 0x0801761a FUN_0801761a size=282

undefined4 FUN_0801761a(int *param_1,int *param_2,int param_3)

{
  bool bVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  
  bVar1 = false;
  local_20 = *param_1;
  if ((((local_20 <= *param_2) && (*param_2 <= param_1[2])) && (param_1[1] <= param_2[1])) &&
     (param_2[1] <= param_1[3])) {
    bVar1 = true;
  }
  if (bVar1) {
    if (param_3 < 1) {
      uVar2 = 1;
    }
    else {
      iVar3 = ((param_1[2] - local_20) + 1) / 2;
      iVar4 = ((param_1[3] - param_1[1]) + 1) / 2;
      if (iVar4 <= iVar3) {
        iVar3 = iVar4;
      }
      if (iVar3 < param_3) {
        param_3 = iVar3;
      }
      local_18 = *param_1 + param_3;
      local_1c = param_1[1];
      local_14 = param_1[1] + param_3;
      iVar3 = FUN_0801761a(&local_20,param_2,0);
      if (iVar3 == 0) {
        local_1c = param_1[3] - param_3;
        local_14 = param_1[3];
        iVar3 = FUN_0801761a(&local_20,param_2,0);
        if (iVar3 == 0) {
          local_20 = param_1[2] - param_3;
          local_18 = param_1[2];
          iVar3 = FUN_0801761a(&local_20,param_2,0);
          if (iVar3 == 0) {
            local_1c = param_1[1];
            local_14 = param_1[1] + param_3;
            iVar3 = FUN_0801761a(&local_20,param_2,0);
            if (iVar3 == 0) {
              uVar2 = 1;
            }
            else {
              local_20 = local_20 - param_3;
              local_14 = local_14 + param_3;
              uVar2 = FUN_08031a08(&local_20,param_2);
            }
          }
          else {
            local_20 = local_20 - param_3;
            local_1c = local_1c - param_3;
            uVar2 = FUN_08031a08(&local_20,param_2);
          }
        }
        else {
          local_18 = local_18 + param_3;
          local_1c = local_1c - param_3;
          uVar2 = FUN_08031a08(&local_20,param_2);
        }
      }
      else {
        local_18 = local_18 + param_3;
        local_14 = local_14 + param_3;
        uVar2 = FUN_08031a08(&local_20,param_2);
      }
    }
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}

