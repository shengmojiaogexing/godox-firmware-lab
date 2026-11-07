// 0x08030f80 FUN_08030f80 size=262

void FUN_08030f80(undefined4 param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_4c;
  undefined4 local_3c;
  int local_38;
  int local_34;
  int local_30;
  uint local_2c;
  
  if (param_2 != 0 || param_3 != 0) {
    if (param_4 == 1) {
      uVar2 = FUN_0802ed10(param_1);
      FUN_080269c8(&local_58);
      local_4c = DAT_08031090;
      local_58 = param_1;
      if (param_2 != 0) {
        iVar1 = FUN_08028248(uVar2);
        local_2c = FUN_08026a90((iVar1 << 1) >> 2,0,param_2);
        if (local_2c < 100) {
          local_2c = 100;
        }
        if (200 < local_2c) {
          local_2c = 200;
        }
        local_30 = FUN_0802f0cc(param_1);
        local_38 = -local_30;
        local_30 = param_2 - local_30;
        local_54 = DAT_0803108c;
        local_3c = DAT_08031094;
        local_34 = local_38;
        iVar1 = FUN_0802ad74(param_1,9,&local_58);
        if (iVar1 != 1) {
          return;
        }
        FUN_08026aac(&local_58);
      }
      if (param_3 != 0) {
        iVar1 = FUN_080282e6(uVar2);
        local_2c = FUN_08026a90((iVar1 << 1) >> 2,0,param_3);
        if (local_2c < 100) {
          local_2c = 100;
        }
        if (200 < local_2c) {
          local_2c = 200;
        }
        local_30 = FUN_0802f0dc(param_1);
        local_38 = -local_30;
        local_30 = param_3 - local_30;
        local_54 = DAT_08031088;
        local_3c = DAT_08031094;
        local_34 = local_38;
        iVar1 = FUN_0802ad74(param_1,9,&local_58);
        if (iVar1 == 1) {
          FUN_08026aac(&local_58);
        }
      }
    }
    else {
      FUN_08026938(param_1,DAT_08031088);
      FUN_08026938(param_1,DAT_0803108c);
      iVar1 = FUN_0802ad74(param_1,9,0);
      if ((iVar1 == 1) && (iVar1 = FUN_080180e2(param_1,param_2,param_3), iVar1 == 1)) {
        FUN_0802ad74(param_1,10,0);
        return;
      }
    }
  }
  return;
}

