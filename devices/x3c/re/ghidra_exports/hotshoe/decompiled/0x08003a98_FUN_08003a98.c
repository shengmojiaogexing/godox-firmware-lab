// 0x08003a98 FUN_08003a98 size=86

undefined4 FUN_08003a98(byte *param_1)

{
  int iVar1;
  int iVar2;
  int extraout_r3;
  
  FUN_08003c88();
  iVar2 = FUN_08003cb4();
  iVar1 = DAT_08003af0;
  if (iVar2 == 0) {
    *(undefined4 *)(DAT_08003af0 + 0x24) = 0xff;
    return 0;
  }
  *(uint *)(DAT_08003af0 + 8) = *(uint *)(DAT_08003af0 + 8) & 0xffffffbf | (*param_1 & 1) << 6;
  *(uint *)(iVar1 + 0x10) = *(uint *)(iVar1 + 0x10) & 0xffff8000 | *(ushort *)(param_1 + 8) & 0x7fff
  ;
  *(uint *)(iVar1 + 0x10) = *(uint *)(iVar1 + 0x10) & 0xff80ffff | (param_1[4] & 0x7f) << 0x10;
  FUN_08003c78();
  *(undefined4 *)(extraout_r3 + 0x24) = 0xff;
  return 1;
}

