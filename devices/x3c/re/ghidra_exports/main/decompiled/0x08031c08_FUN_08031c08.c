// 0x08031c08 FUN_08031c08 size=68

void FUN_08031c08(undefined4 param_1,int param_2)

{
  *(byte *)(param_2 + 0x32) = *(byte *)(param_2 + 0x32) & 0xfe;
  *(undefined2 *)(param_2 + 0x2c) = 0;
  *(undefined2 *)(param_2 + 0x2e) = 0;
  *(undefined2 *)(param_2 + 0x30) = 0;
  FUN_0802e2d2(param_2,0x10);
  FUN_0802e2d2(param_2,0x200);
  FUN_0802e148(DAT_08031c4c,param_2);
  FUN_0802e218();
  FUN_08031ee0(param_2,s_Option_1_Option_2_Option_3_Optio_08031c50,0);
  return;
}

