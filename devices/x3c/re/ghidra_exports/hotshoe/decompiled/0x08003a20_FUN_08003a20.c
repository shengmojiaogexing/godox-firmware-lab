// 0x08003a20 FUN_08003a20 size=58

undefined8 FUN_08003a20(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 uStack_15;
  undefined4 local_14;
  undefined4 uStack_10;
  undefined4 local_c;
  
  local_18 = (undefined1)param_1;
  local_17 = (undefined1)((uint)param_1 >> 8);
  local_16 = (undefined1)((uint)param_1 >> 0x10);
  uStack_15 = (undefined1)((uint)param_1 >> 0x18);
  local_14 = param_2;
  uStack_10 = param_3;
  local_c = param_4;
  FUN_08003b78(&local_18);
  local_14 = DAT_08003a5c;
  local_18 = 2;
  local_17 = 2;
  local_16 = 1;
  FUN_08003af4(0,&local_18);
  local_c = 0x202;
  FUN_08003ba0(0,&local_c);
  FUN_08003c98();
  return CONCAT44(local_14,CONCAT13(uStack_15,CONCAT12(local_16,CONCAT11(local_17,local_18))));
}

