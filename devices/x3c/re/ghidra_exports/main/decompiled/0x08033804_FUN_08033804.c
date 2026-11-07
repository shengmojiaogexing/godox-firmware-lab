// 0x08033804 FUN_08033804 size=200

int FUN_08033804(undefined4 param_1,int param_2,uint param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = 0;
  if ((param_2 == 0) || (param_3 != 0)) {
    if (param_2 == 0) {
      iVar6 = FUN_080337dc(param_1,param_3);
    }
    else {
      uVar1 = FUN_0801964a(param_2);
      uVar2 = FUN_0801978e(uVar1);
      uVar3 = FUN_08019848(uVar1);
      iVar6 = FUN_08019848(uVar2);
      uVar4 = FUN_08018e5a(param_3,4);
      if ((uVar3 < param_3) && (uVar4 == 0)) {
        return 0;
      }
      iVar5 = FUN_08019670(uVar1);
      if (iVar5 != 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      if ((uVar3 < uVar4) &&
         ((iVar5 = FUN_08019670(uVar2), iVar5 == 0 || (iVar6 + uVar3 + 4 < uVar4)))) {
        iVar6 = FUN_080337dc(param_1,param_3);
        if (iVar6 != 0) {
          if (param_3 <= uVar3) {
            uVar3 = param_3;
          }
          FUN_0802d764(iVar6,param_2,uVar3);
          FUN_0803379a(param_1,param_2);
        }
      }
      else {
        if (uVar3 < uVar4) {
          FUN_0801971a(param_1,uVar1);
          FUN_08019702(uVar1);
        }
        FUN_08019908(param_1,uVar1,uVar4);
        iVar6 = param_2;
      }
    }
  }
  else {
    FUN_0803379a(param_1,param_2);
  }
  return iVar6;
}

