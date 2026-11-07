// 0x0802d5f8 FUN_0802d5f8 size=40

void FUN_0802d5f8(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar1 = DAT_0802d620;
  uVar3 = 0;
  do {
    if (*(int *)(iVar1 + uVar3 * 8) != 0) {
      FUN_0802d6e4();
      *(undefined4 *)(iVar1 + uVar3 * 8) = 0;
      iVar2 = iVar1 + uVar3 * 8;
      *(undefined1 *)(iVar2 + 6) = 0;
      *(undefined2 *)(iVar2 + 4) = 0;
    }
    uVar3 = uVar3 + 1 & 0xff;
  } while (uVar3 < 0x10);
  return;
}

