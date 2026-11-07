// 0x08004c7c FUN_08004c7c size=104

undefined4 FUN_08004c7c(undefined4 *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = 0;
  do {
    param_1[8] = iVar1;
LAB_08004c88:
    iVar1 = (*(code *)param_1[3])(param_1);
    if (iVar1 == 0) {
LAB_08004cde:
      return param_1[8];
    }
    if (iVar1 == 0x25) {
      uVar3 = 0;
      iVar1 = (*(code *)param_1[3])(param_1);
      if (iVar1 == 0) goto LAB_08004cde;
      if (iVar1 - 0x41U < 0x1a) {
        iVar1 = iVar1 + 0x20;
        uVar3 = 0x800;
      }
      *param_1 = uVar3;
      iVar2 = FUN_08004ac0(param_1,iVar1,param_2);
      if (iVar2 == 0) goto LAB_08004c94;
      if (iVar2 == 1) {
        param_2 = param_2 + 4;
      }
      else {
        param_2 = (param_2 + 7U & 0xfffffff8) + 8;
      }
      goto LAB_08004c88;
    }
LAB_08004c94:
    (*(code *)param_1[1])(iVar1,param_1[2]);
    iVar1 = param_1[8] + 1;
  } while( true );
}

