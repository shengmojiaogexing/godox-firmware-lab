// 0x08019754 FUN_08019754 size=58

undefined4 FUN_08019754(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_08019688(param_2);
  if (iVar1 != 0) {
    iVar1 = FUN_080197e0(param_2);
    if (iVar1 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    iVar2 = FUN_08019670(iVar1);
    if (iVar2 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    FUN_080197f2(param_1,iVar1);
    param_2 = FUN_0801960c(iVar1,param_2);
  }
  return param_2;
}

