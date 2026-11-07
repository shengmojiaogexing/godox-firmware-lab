// 0x08024ea0 FUN_08024ea0 size=18

uint FUN_08024ea0(int param_1)

{
  byte bVar1;
  
  bVar1 = *(byte *)(param_1 + 1);
  if ((int)((uint)bVar1 << 0x1e) < 0) {
    *(undefined4 *)(DAT_08024eb4 + 4) = 0;
  }
  return (bVar1 & 3) >> 1;
}

