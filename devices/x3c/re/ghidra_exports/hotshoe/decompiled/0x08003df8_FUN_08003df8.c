// 0x08003df8 FUN_08003df8 size=76

undefined4 FUN_08003df8(void)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  
  FUN_08003c88();
  iVar2 = FUN_08003cb4();
  puVar1 = DAT_08003e44;
  if (iVar2 != 0) {
    *DAT_08003e44 = 0;
    puVar1[5] = DAT_08003e48;
    puVar1[1] = DAT_08003e4c;
    puVar1[2] = 0;
    puVar1[4] = DAT_08003e50;
    puVar1[7] = 0;
    puVar1[0xb] = 0;
    puVar1[0xf] = 0;
    puVar3 = DAT_08003e44 + 0x10;
    DAT_08003e44[0x11] = 0;
    puVar1[3] = 7;
    *puVar3 = 0;
    iVar2 = FUN_08003e54();
    if (iVar2 != 0) {
      puVar1[9] = 0xff;
      return 1;
    }
  }
  puVar1[9] = 0xff;
  return 0;
}

