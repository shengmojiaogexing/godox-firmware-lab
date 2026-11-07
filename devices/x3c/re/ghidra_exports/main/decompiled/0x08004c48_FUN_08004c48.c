// 0x08004c48 FUN_08004c48 size=48

void FUN_08004c48(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int local_20;
  int local_1c;
  undefined4 uStack_4;
  
  local_1c = param_1;
  if (param_2 != 0) {
    local_1c = param_1 + param_2 + -1;
  }
  local_20 = param_1;
  uStack_4 = param_4;
  FUN_0800502a(param_3,&local_20,&uStack_4,DAT_08004c78 + 0x8004c5e);
  if (param_2 != 0) {
    FUN_08005050(0,&local_20);
  }
  return;
}

