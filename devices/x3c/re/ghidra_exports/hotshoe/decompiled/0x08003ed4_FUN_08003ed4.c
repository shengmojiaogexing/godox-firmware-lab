// 0x08003ed4 FUN_08003ed4 size=52

void FUN_08003ed4(void)

{
  int iVar1;
  
  FUN_08002fca(&DAT_48000000,2);
  FUN_080072b4(0xf);
  iVar1 = FUN_08002fb6(&DAT_48000000,4);
  if (iVar1 == 0) {
    *(uint *)(DAT_08003f08 + 0x14) = *(uint *)(DAT_08003f08 + 0x14) ^ 1;
  }
  FUN_08002fca(&DAT_48000000,2);
  return;
}

