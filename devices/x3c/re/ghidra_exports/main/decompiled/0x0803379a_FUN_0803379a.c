// 0x0803379a FUN_0803379a size=66

undefined4 FUN_0803379a(undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  uVar4 = 0;
  if (param_2 != 0) {
    iVar1 = FUN_0801964a();
    iVar2 = FUN_08019670(iVar1);
    if (iVar2 != 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    uVar4 = *(undefined4 *)(iVar1 + 4);
    FUN_080196ea(iVar1);
    uVar3 = FUN_08019754(param_1,iVar1);
    uVar3 = FUN_0801971a(param_1,uVar3);
    FUN_0801964e(param_1,uVar3);
  }
  return uVar4;
}

