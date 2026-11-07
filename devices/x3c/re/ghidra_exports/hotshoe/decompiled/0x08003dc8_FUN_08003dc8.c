// 0x08003dc8 FUN_08003dc8 size=44

undefined4 FUN_08003dc8(uint param_1)

{
  uint uVar1;
  
  if ((int)(param_1 * 0x20000000) < 0) {
    uVar1 = (*(uint *)(DAT_08003df4 + 0x40) & 7) >> 2;
  }
  else {
    uVar1 = *(uint *)(DAT_08003df4 + 8);
  }
  if ((uVar1 == 0) || ((*(uint *)(DAT_08003df4 + 0xc) & param_1 >> 4) == 0)) {
    return 0;
  }
  return 1;
}

