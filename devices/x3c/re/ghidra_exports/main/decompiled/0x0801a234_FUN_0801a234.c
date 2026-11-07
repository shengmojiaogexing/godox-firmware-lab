// 0x0801a234 FUN_0801a234 size=82

void FUN_0801a234(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 local_10;
  
  if (*(int *)(DAT_0801a288 + 0xc) == DAT_0801a28c) {
    local_10 = param_4;
    FUN_08035bc4(0x51);
    FUN_08035be8(param_1);
    FUN_0801c118(2);
    *DAT_0801a290 = *DAT_0801a290 | 0x10;
    return;
  }
  local_10._2_2_ = (undefined2)((uint)param_4 >> 0x10);
  local_10._0_2_ = CONCAT11(3,(char)param_1);
  FUN_08014418(0x51,&local_10,2);
  FUN_0801c0fe(10);
  FUN_08014418(0x51,&local_10,2);
  return;
}

