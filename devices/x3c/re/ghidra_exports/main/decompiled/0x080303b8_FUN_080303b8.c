// 0x080303b8 FUN_080303b8 size=68

undefined4 FUN_080303b8(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar1 = FUN_08028298(0);
  do {
    if (iVar1 == 0) {
      return 0;
    }
    for (uVar3 = 0; uVar3 < *(uint *)(iVar1 + 0x24); uVar3 = uVar3 + 1) {
      iVar2 = *(int *)(*(int *)(iVar1 + 0xc) + uVar3 * 4);
      if (iVar2 == param_1) {
        return 1;
      }
      iVar2 = FUN_08035e70(iVar2,param_1);
      if (iVar2 != 0) {
        return 1;
      }
    }
    iVar1 = FUN_08028298(iVar1);
  } while( true );
}

