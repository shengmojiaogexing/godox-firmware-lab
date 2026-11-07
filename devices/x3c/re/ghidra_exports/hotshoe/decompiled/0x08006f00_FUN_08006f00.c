// 0x08006f00 FUN_08006f00 size=38

void FUN_08006f00(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = DAT_08006f28;
  iVar2 = FUN_08006fde(DAT_08006f28,1);
  if (iVar2 == 1) {
    FUN_08006f30(uVar1,1);
    if (*DAT_08006f2c == '\0') {
      FUN_08006ff4();
    }
  }
  return;
}

