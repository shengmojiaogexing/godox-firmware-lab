// 0x080198c6 FUN_080198c6 size=66

void FUN_080198c6(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_08019670(param_2);
  if (iVar1 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  iVar1 = FUN_08019634(param_2,param_3);
  if (iVar1 == 0) {
    return;
  }
  uVar2 = FUN_08019850(param_2,param_3);
  FUN_08019690(param_2);
  FUN_0801981e(uVar2);
  FUN_0801964e(param_1,uVar2);
  return;
}

