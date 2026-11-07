// 0x080372ee FUN_080372ee size=88

undefined4 FUN_080372ee(int param_1,int *param_2,byte *param_3)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = *param_2;
  uVar1 = *(uint *)(param_1 + iVar3 * 4 + 0x14) & -1 << (uint)*param_3;
  if (uVar1 == 0) {
    if ((*(uint *)(param_1 + 0x10) & -1 << (iVar3 + 1U & 0xff)) == 0) {
      return 0;
    }
    iVar3 = FUN_08039894();
    *param_2 = iVar3;
    uVar1 = *(uint *)(param_1 + iVar3 * 4 + 0x14);
  }
  if (uVar1 != 0) {
    iVar2 = FUN_08039894();
    *(int *)param_3 = iVar2;
    return *(undefined4 *)(param_1 + iVar3 * 0x80 + iVar2 * 4 + 0x40);
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

