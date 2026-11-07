// 0x08019850 FUN_08019850 size=114

undefined4 FUN_08019850(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  
  uVar1 = FUN_080198c2(param_1);
  uVar1 = FUN_08035eaa(uVar1,param_2 + -4);
  iVar2 = FUN_08019848(param_1);
  iVar6 = (iVar2 - param_2) + -4;
  iVar2 = FUN_080198c2(uVar1);
  uVar3 = FUN_080198c2(uVar1);
  iVar4 = FUN_08018e88(uVar3,4);
  if (iVar2 != iVar4) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  iVar2 = FUN_08019848(param_1);
  if (iVar2 != iVar6 + param_2 + 4) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  FUN_08019832(uVar1,iVar6);
  uVar5 = FUN_08019848(uVar1);
  if (0xb < uVar5) {
    FUN_08019832(param_1,param_2);
    FUN_080196ea(uVar1);
    return uVar1;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

