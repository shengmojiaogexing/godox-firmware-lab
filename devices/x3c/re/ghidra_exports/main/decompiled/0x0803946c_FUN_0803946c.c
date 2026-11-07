// 0x0803946c FUN_0803946c size=132

void FUN_0803946c(void)

{
  longlong lVar1;
  uint *puVar2;
  int iVar3;
  uint *puVar4;
  uint uVar5;
  
  iVar3 = FUN_0801be08();
  puVar2 = DAT_080394f4;
  if (iVar3 == 0) {
    if (((int)(DAT_080394f8[0x26] << 0x11) < 0) && ((int)(DAT_080394f8[0x26] << 0x13) < 0)) {
      *DAT_080394f4 = DAT_08039500;
    }
    else {
      *DAT_080394f4 = DAT_080394f0;
    }
  }
  else if (iVar3 == 1) {
    *DAT_080394f4 = DAT_080394f0 << 1;
  }
  else if (iVar3 == 2) {
    puVar4 = DAT_080394f8 + -1;
    uVar5 = DAT_080394f0;
    if ((int)(*puVar4 << 9) < 0) {
      uVar5 = DAT_080394f0 << 1;
    }
    lVar1 = (ulonglong)uVar5 * (ulonglong)((*puVar4 & 0x7fff) >> 6);
    uVar5 = FUN_08004b58((int)lVar1,(int)((ulonglong)lVar1 >> 0x20),
                         (uint)*(byte *)(DAT_080394fc + -6 + ((*puVar4 & 0x7ffff) >> 0x10)) *
                         (*puVar4 & 0xf),0);
    *puVar2 = uVar5;
  }
  else {
    *DAT_080394f4 = DAT_080394f0;
  }
  *puVar2 = *puVar2 >> *(sbyte *)(DAT_080394fc + ((*DAT_080394f8 & 0xff) >> 4));
  return;
}

