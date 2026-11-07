// 0x08019ae0 FUN_08019ae0 size=30

short FUN_08019ae0(int param_1,uint param_2)

{
  short sVar1;
  uint uVar2;
  
  sVar1 = 0;
  for (uVar2 = 0; uVar2 < param_2 >> 1; uVar2 = uVar2 + 1 & 0xffff) {
    sVar1 = sVar1 + *(short *)(param_1 + uVar2 * 2);
  }
  return sVar1;
}

