// 0x0800ce2c FUN_0800ce2c size=150

void FUN_0800ce2c(uint param_1,uint param_2,uint param_3,uint param_4)

{
  undefined4 uVar1;
  
  uVar1 = DAT_0800cecc;
  if (*(int *)(DAT_0800cec4 + 0xc) == DAT_0800cec8) {
    FUN_080060ac(param_1,param_3,param_2,param_4);
    return;
  }
  FUN_08022e34(DAT_0800cecc,0x10);
  FUN_08025e94(0x2a);
  FUN_08025e9c(param_1 >> 8);
  FUN_08025e9c(param_1 & 0xff);
  FUN_08025e9c(param_3 >> 8);
  FUN_08025e9c(param_3 & 0xff);
  FUN_0801c118(5);
  FUN_08022e38(uVar1,0x10);
  FUN_08022e34(uVar1,0x10);
  FUN_08025e94(0x2b);
  FUN_08025e9c(param_2 >> 8);
  FUN_08025e9c(param_2 & 0xff);
  FUN_08025e9c(param_4 >> 8);
  FUN_08025e9c(param_4 & 0xff);
  FUN_0801c118(5);
  FUN_08022e38(uVar1,0x10);
  return;
}

