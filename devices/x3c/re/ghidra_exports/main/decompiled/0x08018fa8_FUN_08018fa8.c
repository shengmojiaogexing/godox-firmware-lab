// 0x08018fa8 FUN_08018fa8 size=36

void FUN_08018fa8(void)

{
  undefined1 *puVar1;
  int iVar2;
  
  puVar1 = DAT_08018fcc;
  *DAT_08018fcc = 1;
  iVar2 = FUN_08017f30(DAT_08018fd0);
  if (iVar2 == 0) {
    FUN_080336ac(*(undefined4 *)(puVar1 + 8));
    return;
  }
  FUN_080336c4(*(undefined4 *)(puVar1 + 8));
  return;
}

