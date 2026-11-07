// 0x080f86ee FUN_080f86ee size=52

undefined8 FUN_080f86ee(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined3 local_18;
  undefined1 uStack_15;
  undefined1 uStack_14;
  undefined1 local_13;
  undefined2 uStack_12;
  
  _local_18 = CONCAT13((char)((uint)param_3 >> 0x18),0x8000);
  uStack_12 = (undefined2)((uint)param_4 >> 0x10);
  _uStack_14 = CONCAT11(1,(char)param_4);
  FUN_080f8f10(&DAT_48000000,&local_18);
  FUN_080fa2d4(0,0xf);
  FUN_080f8c90();
  return 0x10000008000;
}

