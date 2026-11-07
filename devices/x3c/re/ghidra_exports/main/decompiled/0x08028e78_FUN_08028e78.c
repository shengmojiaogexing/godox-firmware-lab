// 0x08028e78 FUN_08028e78 size=44

uint FUN_08028e78(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = DAT_08028ea4;
  uVar2 = 0;
  do {
    if (*(int *)(DAT_08028ea4 + uVar2 * 8) == 0) break;
    uVar2 = uVar2 + 1 & 0xff;
  } while (uVar2 < 0x10);
  if (uVar2 < 0x10) {
    *(undefined4 *)(DAT_08028ea4 + uVar2 * 8) = param_1;
    *(undefined4 *)(iVar1 + uVar2 * 8 + 4) = param_2;
    return uVar2;
  }
  return 0xffffffff;
}

