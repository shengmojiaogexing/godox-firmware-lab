// 0x08003d1c FUN_08003d1c size=44

void FUN_08003d1c(void)

{
  int iVar1;
  
  iVar1 = FUN_08003dc8(0x1000);
  if (iVar1 == 1) {
    FUN_08003a88(0x1000);
    FUN_08002c84(0x20000);
    if (*DAT_08003d48 != '\0') {
      *DAT_08003d4c = 1;
    }
  }
  return;
}

