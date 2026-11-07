// 0x0802ed10 FUN_0802ed10 size=62

void FUN_0802ed10(int param_1)

{
  int iVar1;
  uint uVar2;
  
  if (*(int *)(param_1 + 4) != 0) {
    param_1 = FUN_0802ee16();
  }
  iVar1 = FUN_08017f30(DAT_0802ed50);
  do {
    if (iVar1 == 0) {
      return;
    }
    for (uVar2 = 0; uVar2 < *(uint *)(iVar1 + 0x24); uVar2 = uVar2 + 1) {
      if (*(int *)(*(int *)(iVar1 + 0xc) + uVar2 * 4) == param_1) {
        return;
      }
    }
    iVar1 = FUN_08017f5e(DAT_0802ed50,iVar1);
  } while( true );
}

