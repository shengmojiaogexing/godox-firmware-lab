// 0x08035fd0 FUN_08035fd0 size=54

void FUN_08035fd0(void)

{
  int iVar1;
  int iVar2;
  byte bVar3;
  
  iVar1 = DAT_08036008;
  *(undefined1 *)(DAT_08036008 + 7) = 2;
  bVar3 = 0;
  *DAT_0803600c = 0;
  FUN_08013cdc();
  do {
    iVar2 = FUN_08014634();
    FUN_0803ba4c();
    bVar3 = bVar3 + 1;
    if (5 < bVar3) break;
  } while (iVar2 != 0);
  *(undefined1 *)(iVar1 + 6) = 1;
  FUN_08010c00();
  return;
}

