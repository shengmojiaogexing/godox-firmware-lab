// 0x0802c5ac FUN_0802c5ac size=72

int * FUN_0802c5ac(int param_1)

{
  undefined4 uVar1;
  int *piVar2;
  
  if (*(int *)(param_1 + 0x10) == 0) {
    uVar1 = FUN_0802821c();
    *(undefined4 *)(param_1 + 0x10) = uVar1;
  }
  if (*(int *)(param_1 + 0x10) == 0) {
    return (int *)0x0;
  }
  piVar2 = (int *)FUN_08017f8c(DAT_0802c5f4);
  if (piVar2 != (int *)0x0) {
    FUN_0802d8d6(piVar2,0x88);
    *piVar2 = param_1;
    *(byte *)((int)piVar2 + 5) = *(byte *)((int)piVar2 + 5) | 2;
    uVar1 = FUN_08033500(DAT_0802c5f8,0xf,piVar2);
    *(undefined4 *)(*piVar2 + 0x14) = uVar1;
    return piVar2;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

