// 0x08007940 FUN_08007940 size=38

void FUN_08007940(void)

{
  int iVar1;
  byte bVar2;
  
  iVar1 = DAT_08007968;
  bVar2 = *(char *)(DAT_08007968 + 6) + 1;
  *(byte *)(DAT_08007968 + 6) = bVar2;
  if (0x19 < bVar2) {
    *(undefined1 *)(iVar1 + 6) = 0;
    bVar2 = *(char *)(iVar1 + 5) + 1;
    *(byte *)(iVar1 + 5) = bVar2;
    if (4 < bVar2) {
      *(undefined1 *)(iVar1 + 5) = *DAT_0800796c;
    }
  }
  return;
}

