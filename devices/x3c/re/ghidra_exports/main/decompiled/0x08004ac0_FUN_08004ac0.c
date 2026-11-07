// 0x08004ac0 FUN_08004ac0 size=52

undefined4 FUN_08004ac0(int *param_1,int param_2,uint *param_3)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined *puVar4;
  
  if (param_2 != 100) {
    if (param_2 != 99) {
      return 0;
    }
    *(char *)(param_1 + 9) = (char)*param_3;
    *(undefined1 *)((int)param_1 + 0x25) = 0;
    if (param_1[5] == 0) {
      FUN_08004ce4(param_1,param_1 + 9,1);
    }
    return 1;
  }
  uVar3 = 0;
  uVar1 = *param_3;
  puVar4 = (undefined *)0x8004da0;
  if ((int)uVar1 < 0) {
    uVar1 = -uVar1;
    puVar4 = &DAT_08004da4;
  }
  else if (*param_1 << 0x1e < 0) {
    puVar4 = (undefined *)0x8004da8;
  }
  else {
    if (-1 < *param_1 << 0x1d) goto LAB_08004d70;
    puVar4 = (undefined *)0x8004dac;
  }
  uVar3 = 1;
LAB_08004d70:
  iVar2 = 0;
  for (; uVar1 != 0; uVar1 = uVar1 / 10) {
    *(char *)((int)param_1 + iVar2 + 0x24) = (char)uVar1 + (char)(uVar1 / 10) * -10 + '0';
    iVar2 = iVar2 + 1;
  }
  uVar3 = FUN_08004f6e(param_1,iVar2,puVar4,uVar3);
  return uVar3;
}

