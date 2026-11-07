// 0x08004e8c FUN_08004e8c size=212

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_08004e8c(void)

{
  uint uVar1;
  int iVar2;
  char cVar3;
  bool bVar4;
  
  cVar3 = -0x4f;
  bVar4 = true;
  if (*DAT_08004f60 != '\0') {
    *DAT_08004f64 = 0;
    iVar2 = DAT_08004f68;
    uVar1 = 1;
    do {
      cVar3 = *(char *)(DAT_08004f68 + uVar1) + cVar3;
      uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 0x19);
    *DAT_08004f6c = cVar3;
    FUN_080026ee();
    FUN_08002fca(&DAT_48000000,0x8000);
    _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
    FUN_08004960(10);
    FUN_08002f0c(&DAT_48000000,0x8000);
    FUN_08004982(0x32);
    uVar1 = 0;
    do {
      FUN_080048ec(*(undefined1 *)(iVar2 + uVar1));
      FUN_08004982(0x14);
      uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 0x19);
    FUN_080048ec(cVar3);
    FUN_08004982(0x14);
    iVar2 = FUN_080048ec(0x55);
    if (iVar2 == 0xb0) {
      FUN_080070b0();
    }
    bVar4 = iVar2 != 0xb0;
    FUN_08002fca(&DAT_48000000,0x8000);
    _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
    _DAT_4800000c = _DAT_4800000c | 0x40000000;
    FUN_080026ac();
    FUN_08002fca(DAT_08004f70,0x20);
  }
  return bVar4;
}

