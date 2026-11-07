// 0x08004740 FUN_08004740 size=94

void FUN_08004740(void)

{
  ushort uVar1;
  undefined1 *puVar2;
  ushort *puVar3;
  uint uVar4;
  char *pcVar5;
  undefined2 *puVar6;
  char *pcVar7;
  int iVar8;
  
  pcVar5 = DAT_080047b0;
  uVar4 = DAT_080047ac;
  puVar3 = DAT_080047a8;
  puVar2 = DAT_080047a4;
  if (*DAT_080047a0 == '\x02') {
    *DAT_080047a0 = '\0';
    *puVar2 = 2;
    return;
  }
  uVar1 = *DAT_080047a8;
  *DAT_080047a8 = uVar1 + 1;
  puVar6 = DAT_080047b4;
  if (uVar4 <= (ushort)(uVar1 + 1)) {
    *puVar3 = 0;
    pcVar7 = DAT_080047b8;
    *puVar6 = 0;
    if (*pcVar7 == '\0') {
      *pcVar5 = '\x01';
      goto LAB_08004776;
    }
  }
  if (*pcVar5 != '\x01') {
    return;
  }
LAB_08004776:
  FUN_080070cc();
  iVar8 = FUN_08004e8c();
  if (iVar8 != 0) {
    FUN_08007294(DAT_080047bc);
    FUN_08004e8c();
  }
  *puVar2 = 1;
  *pcVar5 = '\0';
  return;
}

