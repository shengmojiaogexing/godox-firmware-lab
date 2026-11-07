// 0x0802c7cc FUN_0802c7cc size=142

void FUN_0802c7cc(undefined4 *param_1,int param_2)

{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = DAT_0802c85c;
  if (param_1 != (undefined4 *)0x0) {
    *(byte *)((int)param_1 + 5) = *(byte *)((int)param_1 + 5) | 2;
    if (*(undefined4 **)(iVar1 + 8) == param_1) {
      *(undefined4 *)(iVar1 + 4) = 0;
    }
    if ((*(char *)*param_1 == '\x01') || (*(char *)*param_1 == '\x02')) {
      if ((param_2 == 0) || (param_1[0x15] == param_2)) {
        param_1[0x15] = 0;
      }
      if ((param_2 == 0) || (param_1[0x12] == param_2)) {
        param_1[0x12] = 0;
      }
      if ((param_2 == 0) || (param_1[0x13] == param_2)) {
        param_1[0x13] = 0;
      }
    }
    return;
  }
  puVar2 = (undefined4 *)FUN_0802c610(0);
  while (puVar2 != (undefined4 *)0x0) {
    *(byte *)((int)puVar2 + 5) = *(byte *)((int)puVar2 + 5) | 2;
    if ((*(char *)*puVar2 == '\x01') || (*(char *)*puVar2 == '\x02')) {
      if ((param_2 == 0) || (puVar2[0x15] == param_2)) {
        puVar2[0x15] = 0;
      }
      if ((param_2 == 0) || (puVar2[0x12] == param_2)) {
        puVar2[0x12] = 0;
      }
      if ((param_2 == 0) || (puVar2[0x13] == param_2)) {
        puVar2[0x13] = 0;
      }
    }
    puVar2 = (undefined4 *)FUN_0802c610();
  }
  *(undefined4 *)(iVar1 + 4) = 0;
  return;
}

