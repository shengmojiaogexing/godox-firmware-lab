// 0x080fcd68 FUN_080fcd68 size=134

void FUN_080fcd68(void)

{
  char cVar1;
  undefined4 uVar2;
  char *pcVar3;
  char *pcVar4;
  undefined1 uVar5;
  int iVar6;
  
  if (*DAT_080fcdf0 == '\0') {
    FUN_080f8f0c(&DAT_48000000,2);
    uVar2 = DAT_080fcdf4;
    uVar5 = FUN_080fa040(DAT_080fcdf4);
    *(undefined1 *)(DAT_080fcdf8 + 1) = uVar5;
    iVar6 = FUN_080fa000(uVar2,0x40);
    if (iVar6 == 1) {
      FUN_080f9f0c(uVar2,0x40);
    }
    FUN_080f8fca(&DAT_48000000,8);
    pcVar3 = DAT_080fce00;
    *DAT_080fcdfc = 1;
    pcVar4 = DAT_080fce04;
    cVar1 = *pcVar3;
    if ((cVar1 == -0x5b) || (cVar1 == -0x58)) {
      FUN_080fa508();
    }
    else if ((((*DAT_080fce04 == -0x5b) || (cVar1 == -0x3c)) || (cVar1 == -0x4c)) &&
            (*DAT_080fce08 != '\'')) {
      FUN_080fa4bc();
    }
    FUN_080fc618();
    if (*DAT_080fce0c == '\0') {
      *pcVar4 = *pcVar3;
    }
  }
  return;
}

