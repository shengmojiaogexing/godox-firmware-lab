// 0x0802c1ba FUN_0802c1ba size=44

void FUN_0802c1ba(undefined4 param_1,int param_2)

{
  byte bVar1;
  
  bVar1 = *(byte *)(param_2 + 0x4e) & 3;
  if ((bVar1 == 1) || (bVar1 == 2)) {
    FUN_0802d6e4(*(undefined4 *)(param_2 + 0x2c));
    *(undefined4 *)(param_2 + 0x2c) = 0;
    *(byte *)(param_2 + 0x4e) = *(byte *)(param_2 + 0x4e) | 3;
  }
  return;
}

