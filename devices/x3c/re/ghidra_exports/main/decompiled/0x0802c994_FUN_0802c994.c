// 0x0802c994 FUN_0802c994 size=96

void FUN_0802c994(undefined4 param_1,int param_2)

{
  *(undefined4 *)(param_2 + 0x2c) = 0;
  *(byte *)(param_2 + 0x54) = *(byte *)(param_2 + 0x54) & 0xe7;
  *(undefined4 *)(param_2 + 0x34) = 0xffffffff;
  *(byte *)(param_2 + 0x54) = *(byte *)(param_2 + 0x54) & 0xf8;
  *(undefined4 *)(param_2 + 0x4c) = 0;
  *(undefined4 *)(param_2 + 0x50) = 0;
  *(undefined4 *)(param_2 + 0x38) = 0xffffffff;
  *(undefined4 *)(param_2 + 0x40) = 0;
  *(undefined4 *)(param_2 + 0x3c) = 0;
  *(undefined4 *)(param_2 + 0x44) = 0xffff;
  *(undefined4 *)(param_2 + 0x48) = 0xffff;
  *(undefined4 *)(param_2 + 0x30) = 0;
  *(byte *)(param_2 + 0x54) = *(byte *)(param_2 + 0x54) & 0xbf;
  FUN_0802e2d2(param_2,2);
  FUN_0802d2c0(param_2,0);
  FUN_0802d33c(param_2,&DAT_0802c9f4);
  return;
}

