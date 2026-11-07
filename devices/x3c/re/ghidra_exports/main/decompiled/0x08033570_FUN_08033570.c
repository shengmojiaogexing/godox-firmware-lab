// 0x08033570 FUN_08033570 size=72

undefined4 FUN_08033570(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  if ((*(byte *)(param_1 + 0x14) & 1) != 0) {
    return 0;
  }
  uVar2 = 0;
  iVar1 = FUN_080336ca(param_1);
  if (iVar1 == 0) {
    iVar1 = *(int *)(param_1 + 0x10);
    if (0 < iVar1) {
      *(int *)(param_1 + 0x10) = iVar1 + -1;
    }
    uVar2 = FUN_080334d8();
    *(undefined4 *)(param_1 + 4) = uVar2;
    if ((*(code **)(param_1 + 8) != (code *)0x0) && (iVar1 != 0)) {
      (**(code **)(param_1 + 8))(param_1);
    }
    uVar2 = 1;
  }
  if ((*(char *)(DAT_080335b8 + 3) == '\0') && (*(int *)(param_1 + 0x10) == 0)) {
    FUN_08033540(param_1);
  }
  return uVar2;
}

