// 0x0801616c FUN_0801616c size=120

void FUN_0801616c(void)

{
  char cVar1;
  char cVar2;
  int iVar3;
  
  if (*DAT_080161e4 < 5) {
    *DAT_080161e4 = 5;
  }
  if (*DAT_080161e8 < 5) {
    *DAT_080161e8 = 5;
  }
  iVar3 = DAT_080161ec;
  cVar1 = *(char *)(DAT_080161ec + 5);
  cVar2 = *(char *)(DAT_080161ec + 7);
  if ((cVar1 == '\x01') && (cVar2 == '\0')) {
    FUN_08014814(0x20);
    FUN_08014864(0);
    *(undefined1 *)(iVar3 + 2) = 0x20;
    *(undefined1 *)(iVar3 + 3) = 0;
    *(undefined1 *)(iVar3 + 7) = 1;
  }
  else {
    if ((cVar1 == '\x01') && (cVar2 == '\x01')) {
      *(undefined1 *)(DAT_080161ec + 0x14) = 0xb4;
      *(undefined1 *)(iVar3 + 0x15) = 0xa0;
      *(char *)(iVar3 + 0x16) = *DAT_080161f0 + -1;
      *(undefined1 *)(iVar3 + 0x17) = *DAT_080161f4;
      FUN_0801708c();
      return;
    }
    if ((cVar1 == '\0') && (cVar2 == '\x01')) {
      *(undefined1 *)(DAT_080161ec + 7) = 0;
      return;
    }
  }
  return;
}

