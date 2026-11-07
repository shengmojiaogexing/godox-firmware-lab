// 0x080373c8 FUN_080373c8 size=40

undefined4 FUN_080373c8(void)

{
  int iVar1;
  undefined4 in_r3;
  uint uVar2;
  undefined4 uStack_10;
  
  uVar2 = 0;
  uStack_10 = in_r3;
  do {
    FUN_08037700();
    iVar1 = FUN_080376b4(0x20000000,&uStack_10,4);
    if (iVar1 == 0) {
      return 0;
    }
    uVar2 = uVar2 + 1;
  } while (uVar2 < 3);
  return 0xffffffff;
}

