// 0x08033da4 FUN_08033da4 size=68

undefined4 FUN_08033da4(int param_1,int *param_2,undefined4 param_3,int param_4)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 uVar3;
  byte bVar4;
  int local_18;
  
  bVar4 = 0;
  *param_2 = *param_2 + -1;
  puVar1 = DAT_08033de8;
  local_18 = param_4;
  do {
    if (3 < bVar4) {
      return 0;
    }
    iVar2 = (*(code *)*puVar1)(*param_2 + param_1);
    if (iVar2 == 0) {
      if (*param_2 == 0) {
        return 0;
      }
      *param_2 = *param_2 + -1;
    }
    bVar4 = bVar4 + 1;
  } while (iVar2 == 0);
  local_18 = *param_2;
  uVar3 = (*(code *)puVar1[1])(param_1,&local_18);
  return uVar3;
}

