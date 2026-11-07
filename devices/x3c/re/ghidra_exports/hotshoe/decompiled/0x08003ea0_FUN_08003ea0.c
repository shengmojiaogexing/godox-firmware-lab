// 0x08003ea0 FUN_08003ea0 size=42

void FUN_08003ea0(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = DAT_08003ed0;
  iVar2 = FUN_08003fe2(DAT_08003ed0,DAT_08003ecc);
  if (iVar2 != 0) {
    FUN_08006d68();
  }
  iVar2 = FUN_08004000(uVar1,0x40);
  if (iVar2 != 0) {
    FUN_08003f0c(uVar1,0x40);
  }
  return;
}

