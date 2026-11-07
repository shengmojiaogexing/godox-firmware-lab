// 0x0802d20c FUN_0802d20c size=86

void FUN_0802d20c(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  if (((*(byte *)(param_1 + 0x54) & 7) == 1) && (*(int *)(param_1 + 0x34) != -1)) {
    iVar1 = (*(code *)*DAT_0802d264)(*(undefined4 *)(param_1 + 0x2c),*(int *)(param_1 + 0x34) + -3);
    uVar3 = 0;
    iVar2 = FUN_0802cb78(param_1);
    while( true ) {
      if (*(char *)(*(int *)(param_1 + 0x2c) + iVar1 + uVar3) == '\0') break;
      *(undefined1 *)(*(int *)(param_1 + 0x2c) + iVar1 + uVar3) = *(undefined1 *)(iVar2 + uVar3);
      uVar3 = uVar3 + 1 & 0xff;
    }
    *(undefined1 *)(*(int *)(param_1 + 0x2c) + iVar1 + uVar3) = *(undefined1 *)(iVar2 + uVar3);
    FUN_0802ca36(param_1);
    *(undefined4 *)(param_1 + 0x34) = 0xffffffff;
  }
  return;
}

