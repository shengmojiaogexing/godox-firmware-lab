// 0x08004f74 FUN_08004f74 size=202

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_08004f74(void)

{
  undefined1 *puVar1;
  int iVar2;
  uint extraout_r2;
  uint uVar3;
  uint uVar4;
  undefined8 uVar5;
  
  uVar4 = 0xd1;
  FUN_080026ee();
  FUN_08002fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff | 0x40000000;
  FUN_08004960(10);
  FUN_08002f0c(&DAT_48000000,0x8000);
  FUN_08004982(0x32);
  FUN_080048ec(0xd0);
  puVar1 = DAT_08005040;
  *DAT_08005040 = 0xd0;
  FUN_08004982(0x14);
  uVar3 = 1;
  do {
    iVar2 = FUN_080048ec(0xff);
    puVar1[uVar3] = (char)iVar2;
    uVar4 = iVar2 + uVar4 & 0xff;
    FUN_08004982(0x14);
    uVar3 = uVar3 + 1 & 0xff;
  } while (uVar3 < 0x18);
  uVar5 = FUN_080048ec(0xff);
  FUN_08004960(0x14,(int)((ulonglong)uVar5 >> 0x20),(int)uVar5);
  if (uVar4 == extraout_r2) {
    FUN_08007a38();
    *DAT_08005044 = 1;
  }
  FUN_08002fca(&DAT_48000000,0x8000);
  _DAT_48000000 = _DAT_48000000 & 0x3fffffff;
  _DAT_4800000c = _DAT_4800000c | 0x40000000;
  FUN_080026ac();
  FUN_08002fca(DAT_08005048,0x20);
  return uVar4 != extraout_r2;
}

