// 0x08026938 FUN_08026938 size=90

undefined4 FUN_08026938(int param_1,int param_2)

{
  int *piVar1;
  int *piVar2;
  undefined4 uVar3;
  
  uVar3 = 0;
  piVar1 = (int *)FUN_08017f30(DAT_08026994);
  while (piVar1 != (int *)0x0) {
    piVar2 = (int *)FUN_08017f5e(DAT_08026994,piVar1);
    if (((*piVar1 == param_1) || (param_1 == 0)) && ((piVar1[1] == param_2 || (param_2 == 0)))) {
      FUN_0801801e(DAT_08026994,piVar1);
      if ((code *)piVar1[4] != (code *)0x0) {
        (*(code *)piVar1[4])(piVar1);
      }
      FUN_0802d6e4(piVar1);
      FUN_08018fa8();
      uVar3 = 1;
    }
    piVar1 = piVar2;
  }
  return uVar3;
}

