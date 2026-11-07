// 0x080205b0 FUN_080205b0 size=66

void FUN_080205b0(void)

{
  int iVar1;
  ushort uVar2;
  uint uVar3;
  
  iVar1 = DAT_080205f4;
  if (*(char *)(DAT_080205f4 + 1) != '\0') {
    if ((int)((uint)*DAT_080205f8 << 0x18) < 0) {
      uVar3 = *DAT_080205fc;
      if (uVar3 != 0) {
        if (uVar3 < 3) {
          uVar3 = 0;
        }
        else {
          uVar3 = uVar3 - 2;
        }
        *DAT_080205fc = uVar3;
        if (uVar3 == 0) {
          *(undefined1 *)(iVar1 + 3) = 1;
        }
      }
    }
    else {
      uVar2 = *(short *)(DAT_080205f4 + 0xc) + 1;
      *(ushort *)(DAT_080205f4 + 0xc) = uVar2;
      if (0xe10 < uVar2) {
        *(undefined1 *)(iVar1 + 5) = 1;
        return;
      }
    }
  }
  return;
}

