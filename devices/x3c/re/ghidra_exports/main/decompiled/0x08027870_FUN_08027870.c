// 0x08027870 FUN_08027870 size=110

void FUN_08027870(undefined4 param_1,undefined4 param_2,undefined4 *param_3,int param_4,int param_5)

{
  int local_50;
  undefined4 local_4c;
  undefined4 local_44;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  
  if (param_5 == 0) {
    *param_3 = param_2;
    FUN_080302d0(param_1);
    return;
  }
  if (*(int *)(param_4 + 0xc) == -1) {
    *(undefined4 *)(param_4 + 4) = *param_3;
    *(undefined4 *)(param_4 + 8) = param_2;
  }
  else {
    *(undefined4 *)(param_4 + 4) = *(undefined4 *)(param_4 + 8);
    *(undefined4 *)(param_4 + 8) = param_2;
  }
  *param_3 = param_2;
  FUN_08026938(param_4,0);
  FUN_080269c8(&local_50);
  local_4c = DAT_080278e0;
  local_30 = 0;
  local_2c = 0;
  local_28 = 0x100;
  local_44 = DAT_080278e4;
  local_50 = param_4;
  local_24 = FUN_0802f9bc(param_1,0,100);
  FUN_08026aac(&local_50);
  return;
}

