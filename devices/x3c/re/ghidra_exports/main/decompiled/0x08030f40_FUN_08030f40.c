// 0x08030f40 FUN_08030f40 size=60

void FUN_08030f40(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  
  if (*DAT_08030f7c != '\0') {
    for (iVar1 = FUN_08028298(0); iVar1 != 0; iVar1 = FUN_08028298(iVar1)) {
      for (uVar2 = 0; uVar2 < *(uint *)(iVar1 + 0x24); uVar2 = uVar2 + 1) {
        FUN_080369bc(param_1,*(undefined4 *)(*(int *)(iVar1 + 0xc) + uVar2 * 4));
      }
    }
  }
  return;
}

