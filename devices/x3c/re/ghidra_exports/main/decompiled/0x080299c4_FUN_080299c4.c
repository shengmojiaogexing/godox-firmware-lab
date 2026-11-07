// 0x080299c4 FUN_080299c4 size=42

undefined4 FUN_080299c4(int param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  
  iVar1 = DAT_080299f0;
  uVar3 = 0;
  uVar2 = 0;
  do {
    if (*(int *)(iVar1 + uVar2 * 8 + 4) == param_1) {
      uVar3 = *(undefined4 *)(iVar1 + uVar2 * 8);
      FUN_080299f4(uVar2);
    }
    uVar2 = uVar2 + 1 & 0xff;
  } while (uVar2 < 0x10);
  return uVar3;
}

