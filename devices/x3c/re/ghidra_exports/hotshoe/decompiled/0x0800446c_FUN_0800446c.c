// 0x0800446c FUN_0800446c size=74

undefined8 FUN_0800446c(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 local_18;
  undefined4 uStack_14;
  
  local_18 = param_3;
  uStack_14 = param_4;
  FUN_08003950(0x20000);
  FUN_08003980(1);
  local_18 = CONCAT13(local_18._3_1_,0x200);
  uStack_14._0_2_ = CONCAT11(1,(undefined1)uStack_14);
  FUN_08002f10(&DAT_48000000,&local_18);
  FUN_080042d4(0,9);
  FUN_08002c90();
  *DAT_080044b8 = *DAT_080044b8 & 0xfffffdff;
  return 0x10000000200;
}

