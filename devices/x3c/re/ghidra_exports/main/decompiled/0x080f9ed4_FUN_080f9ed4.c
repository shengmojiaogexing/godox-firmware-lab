// 0x080f9ed4 FUN_080f9ed4 size=52

void FUN_080f9ed4(void)

{
  int iVar1;
  
  FUN_080f8fca(&DAT_48000000,2);
  FUN_080fd2b4(0xf);
  iVar1 = FUN_080f8fb6(&DAT_48000000,4);
  if (iVar1 == 0) {
    *(uint *)(DAT_080f9f08 + 0x14) = *(uint *)(DAT_080f9f08 + 0x14) ^ 1;
  }
  FUN_080f8fca(&DAT_48000000,2);
  return;
}

