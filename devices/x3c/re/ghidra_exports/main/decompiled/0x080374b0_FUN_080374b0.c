// 0x080374b0 FUN_080374b0 size=170

undefined4 FUN_080374b0(void)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  
  FUN_080376b2();
  FUN_08037700();
  iVar2 = FUN_080373c8();
  if (iVar2 == 0) {
    FUN_080377d4();
    FUN_080378f0();
    puVar1 = DAT_0803755c;
    if (*(char *)(DAT_0803755c + 4) != '\0') {
      FUN_0803ba4c();
      FUN_0800ce2c(0x76,0x94,0xfb,299);
      uVar3 = DAT_08037560;
      FUN_08022e34(DAT_08037560,0x10);
      FUN_08025e94(0x2c);
      iVar2 = DAT_08037564;
      uVar5 = 0;
      do {
        uVar4 = 0;
        do {
          iVar6 = uVar5 * 0x10c + uVar4 * 2;
          FUN_08025e9c(*(undefined1 *)(iVar6 + iVar2 + 1));
          FUN_08025e9c(*(undefined1 *)(iVar2 + iVar6));
          uVar4 = uVar4 + 1 & 0xffff;
        } while (uVar4 < 0x86);
        uVar5 = uVar5 + 1 & 0xffff;
      } while (uVar5 < 0x98);
      FUN_0801c118(5);
      FUN_08022e38(uVar3,0x10);
      FUN_08037894(puVar1[5],*(undefined2 *)(puVar1 + 6));
      FUN_080377d4();
    }
    FUN_08037700();
    uVar3 = 0;
    *puVar1 = 0;
  }
  else {
    uVar3 = 0xffffffff;
  }
  return uVar3;
}

