// 0x08007868 FUN_08007868 size=258

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08007868(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  int iVar3;
  
  puVar1 = DAT_08007974;
  iVar3 = DAT_08007970;
  if (*DAT_0800796c == '\0') {
    FUN_08002f0c(DAT_08007970,2,param_3,param_4,param_4);
    FUN_0800441c();
    FUN_08002fca(&DAT_48000000,0x100);
    _DAT_48000000 = _DAT_48000000 & 0xfff3ffff;
    _DAT_4800000c = _DAT_4800000c & 0xfff3ffff | 0x40000;
    iVar3 = FUN_08002fb6(&DAT_48000000,0x200);
    if (iVar3 == 0) {
      FUN_08002fca(&DAT_48000000,1);
      *puVar1 = 1;
      *DAT_08007988 = 0;
      FUN_0800431c();
      return;
    }
  }
  else if (*DAT_0800796c == '\x01') {
    *DAT_08007978 = 0;
    *DAT_0800797c = 0;
    puVar2 = DAT_08007980;
    *puVar1 = 0;
    *puVar2 = 0;
    _DAT_48000000 = _DAT_48000000 & 0xffffdfff | 0x1000;
    FUN_08002f0c(&DAT_48000000,0x40,param_3,param_4,param_4);
    _DAT_48000000 = _DAT_48000000 & 0xffff33ff;
    _DAT_4800000c = (_DAT_4800000c & ~(iVar3 << 5) | (iVar3 << 5) >> 1) & 0xfffff3ff | 0x400;
    FUN_08002f0c(iVar3,1);
    FUN_08002f0c(&DAT_48000000,2);
    FUN_080030fe();
    FUN_0800446c();
    FUN_080044bc();
    FUN_08002f0c(&DAT_48000000,0x100);
    FUN_08002fca(iVar3,2);
    iVar3 = DAT_08007984;
    *(byte *)(DAT_08007984 + 2) = *(byte *)(DAT_08007984 + 2) | 4;
    *(undefined1 *)(iVar3 + 8) = 0;
  }
  return;
}

