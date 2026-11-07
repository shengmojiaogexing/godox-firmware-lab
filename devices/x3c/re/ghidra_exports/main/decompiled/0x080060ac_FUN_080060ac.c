// 0x080060ac FUN_080060ac size=130

void FUN_080060ac(short param_1,int param_2,uint param_3,uint param_4)

{
  uint *puVar1;
  
  FUN_08035bc4(0x2a);
  FUN_08035be8((ushort)(param_1 + 0x10U) >> 8);
  FUN_08035be8((char)(param_1 + 0x10U));
  FUN_08035be8((param_2 + 0x10U & 0xffff) >> 8);
  FUN_08035be8(param_2 + 0x10U & 0xff);
  FUN_0801c118(2);
  puVar1 = DAT_08006130;
  *DAT_08006130 = *DAT_08006130 | 0x10;
  FUN_08035bc4(0x2b);
  FUN_08035be8(param_3 >> 8);
  FUN_08035be8(param_3 & 0xff);
  FUN_08035be8(param_4 >> 8);
  FUN_08035be8(param_4 & 0xff);
  FUN_0801c118(2);
  *puVar1 = *puVar1 | 0x10;
  FUN_08035bc4(0x2c);
  FUN_0801c118(2);
  *puVar1 = *puVar1 | 0x10;
  return;
}

