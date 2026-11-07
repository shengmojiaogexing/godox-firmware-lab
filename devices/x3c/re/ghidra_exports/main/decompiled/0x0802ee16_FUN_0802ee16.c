// 0x0802ee16 FUN_0802ee16 size=18

undefined4 FUN_0802ee16(int param_1)

{
  undefined8 uVar1;
  
  do {
    uVar1 = FUN_0802ee0a(param_1);
    param_1 = (int)uVar1;
  } while (param_1 != 0);
  return (int)((ulonglong)uVar1 >> 0x20);
}

