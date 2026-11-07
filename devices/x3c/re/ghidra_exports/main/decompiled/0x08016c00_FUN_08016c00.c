// 0x08016c00 FUN_08016c00 size=74

void FUN_08016c00(void)

{
  undefined4 uVar1;
  uint *puVar2;
  ushort *puVar3;
  ushort uVar4;
  int iVar5;
  uint uVar6;
  
  uVar1 = DAT_08016c4c;
  iVar5 = FUN_08039916(DAT_08016c4c,0x10);
  if (iVar5 != 0) {
    uVar4 = FUN_080398c8(uVar1,6);
    puVar2 = DAT_08016c50;
    uVar6 = *DAT_08016c50;
    *DAT_08016c50 = uVar6 + 1;
    puVar3 = DAT_08016c54;
    DAT_08016c54[uVar6] = uVar4;
    FUN_08039910(uVar1,0x10);
    if (1 < *puVar2) {
      *DAT_08016c58 = (0x10000 - (uint)*puVar3) + (uint)puVar3[1] & 0xffff;
    }
  }
  return;
}

