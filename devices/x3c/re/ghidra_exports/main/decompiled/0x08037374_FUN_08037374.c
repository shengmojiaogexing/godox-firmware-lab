// 0x08037374 FUN_08037374 size=50

undefined4 FUN_08037374(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined1 local_c;
  undefined1 local_b;
  undefined1 local_a;
  undefined1 local_9;
  
  _local_c = CONCAT13((char)param_1,
                      CONCAT12((char)((uint)param_1 >> 8),
                               CONCAT11((char)((uint)param_1 >> 0x10),(char)((uint)param_1 >> 0x18))
                              ));
  iVar1 = FUN_0800c7f8(0x5c,&local_c,4,param_2,param_3);
  if (iVar1 == 0) {
    return 0;
  }
  return 0xffffffff;
}

