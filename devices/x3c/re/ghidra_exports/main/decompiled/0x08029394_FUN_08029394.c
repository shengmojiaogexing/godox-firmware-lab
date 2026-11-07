// 0x08029394 FUN_08029394 size=88

undefined4 FUN_08029394(int param_1)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  
  piVar1 = DAT_080293ec;
  if (param_1 == 0) {
    if (*DAT_080293ec == 0) {
      return 0;
    }
    return 1;
  }
  uVar3 = 0;
  do {
    iVar2 = piVar1[uVar3 * 2];
    if (iVar2 != 0) {
      if (*(char *)(iVar2 + 4) != '\x02') {
        return 1;
      }
      if ((*(byte *)(iVar2 + 0x1c) & 1) == 0) {
        iVar2 = FUN_080174c4(param_1,iVar2 + 8,*(undefined4 *)(iVar2 + 0x18));
        if (iVar2 == 0) {
          return 1;
        }
      }
      else {
        iVar2 = FUN_08017586(param_1,iVar2 + 8,*(undefined4 *)(iVar2 + 0x18));
        if (iVar2 == 0) {
          return 1;
        }
      }
    }
    uVar3 = uVar3 + 1 & 0xff;
  } while (uVar3 < 0x10);
  return 0;
}

