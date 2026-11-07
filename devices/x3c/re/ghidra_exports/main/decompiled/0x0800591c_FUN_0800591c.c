// 0x0800591c FUN_0800591c size=130

void FUN_0800591c(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = DAT_080059a4;
  iVar1 = DAT_080059a0;
  if (*(char *)(DAT_080059a0 + 0xb) == '\0') {
    if (*(char *)(param_1 + 8) == '\a') {
      *(byte *)(DAT_080059a0 + 1) = *(byte *)(DAT_080059a0 + 1) ^ 0x80;
      if ((int)((uint)*(byte *)(iVar1 + 1) << 0x18) < 0) {
        FUN_0802da8e(*(undefined4 *)(DAT_080059a4 + 4),1);
        return;
      }
      FUN_0802e34c(*(undefined4 *)(DAT_080059a4 + 4),1);
      return;
    }
    if ((*(char *)(param_1 + 8) == '\x0e') &&
       (iVar3 = FUN_0802fcb6(*(undefined4 *)(param_1 + 4),8), iVar3 != 0)) {
      *(byte *)(iVar1 + 1) = *(byte *)(iVar1 + 1) ^ 0x80;
      if ((int)((uint)*(byte *)(iVar1 + 1) << 0x18) < 0) {
        FUN_0802da8e(*(undefined4 *)(iVar2 + 4),1);
      }
      else {
        FUN_0802e34c(*(undefined4 *)(iVar2 + 4),1);
      }
      FUN_0802e34c(*(undefined4 *)(param_1 + 4),8);
      FUN_0802b8b2(*DAT_080059a8,0);
      return;
    }
  }
  return;
}

