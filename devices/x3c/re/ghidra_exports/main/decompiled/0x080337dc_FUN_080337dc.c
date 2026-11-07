// 0x080337dc FUN_080337dc size=78

undefined4 FUN_080337dc(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined4 unaff_r4;
  undefined4 unaff_lr;
  
  iVar2 = FUN_08018e5a(param_2,4);
  iVar3 = FUN_0801969e(param_1,iVar2);
  if (iVar3 == 0) {
    return 0;
  }
  if (iVar2 != 0) {
    FUN_080198c6(param_1,iVar3,iVar2,param_1,unaff_r4,unaff_lr);
    FUN_08019702(iVar3);
    uVar1 = FUN_080198c2(iVar3);
    return uVar1;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

