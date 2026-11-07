// 0x080084ac FUN_080084ac size=146

void FUN_080084ac(void)

{
  char *pcVar1;
  undefined2 *puVar2;
  undefined1 *puVar3;
  undefined2 *puVar4;
  int iVar5;
  
  iVar5 = FUN_08020984(0x80);
  if (iVar5 != 0) {
    if (*DAT_08008540 != '\0') {
      FUN_080393c4();
    }
    FUN_0800923c();
    *DAT_08008544 = 1;
    FUN_08020978(0x80);
  }
  iVar5 = FUN_08020984(0x100);
  puVar4 = DAT_08008554;
  puVar3 = DAT_08008550;
  puVar2 = DAT_0800854c;
  pcVar1 = DAT_08008548;
  if (iVar5 == 0) {
    iVar5 = FUN_08020984(0x200);
    if (iVar5 != 0) {
      *(undefined1 *)(DAT_0800855c + 0x10) = 1;
      *puVar3 = 1;
      *puVar4 = 2000;
      *DAT_08008558 = 0;
      if (*pcVar1 != '\0') {
        *puVar2 = 100;
      }
    }
  }
  else {
    if (*DAT_08008548 != '\0') {
      *DAT_0800854c = 100;
    }
    *puVar3 = 1;
    *puVar4 = 2000;
    *DAT_08008558 = 0;
  }
  FUN_08020978(0x100);
  FUN_08020978(0x200);
  return;
}

