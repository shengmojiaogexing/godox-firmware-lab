// 0x080376b4 FUN_080376b4 size=74

int FUN_080376b4(int param_1,int param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  do {
    if (param_3 == 0) {
      return 0;
    }
    if (param_3 < 0x21) {
      uVar3 = param_3;
    }
    else {
      uVar3 = 0x20;
    }
    iVar4 = -1;
    uVar2 = 0;
    do {
      iVar1 = FUN_08037374(param_1,param_2,uVar3);
      if (iVar1 == 0) {
        iVar4 = 0;
        break;
      }
      uVar2 = uVar2 + 1;
    } while (uVar2 < 3);
    if (iVar4 != 0) {
      return iVar4;
    }
    param_1 = param_1 + uVar3;
    param_2 = param_2 + uVar3;
    param_3 = param_3 - uVar3 & 0xffff;
  } while( true );
}

