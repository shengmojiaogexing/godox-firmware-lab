// 0x080379cc FUN_080379cc size=106

int FUN_080379cc(int param_1,int param_2,uint param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  char local_34 [28];
  
  do {
    if (param_3 == 0) {
      return 0;
    }
    iVar4 = 0;
    do {
      if (param_3 < 0x1c) {
        uVar3 = param_3;
      }
      else {
        uVar3 = 0x1c;
      }
      FUN_08037a36(param_1,param_2,uVar3 & 0xffff);
      iVar1 = FUN_080376b4(param_1,local_34,uVar3 & 0xffff);
      for (iVar2 = 0; iVar2 < (int)uVar3; iVar2 = iVar2 + 1) {
        if (local_34[iVar2] != *(char *)(param_2 + iVar2)) {
          iVar1 = -4;
          break;
        }
      }
    } while ((iVar1 != 0) && (iVar4 = iVar4 + 1, iVar4 < 3));
    if (iVar1 != 0) {
      return iVar1;
    }
    param_1 = param_1 + uVar3;
    param_2 = param_2 + uVar3;
    param_3 = param_3 - uVar3 & 0xffff;
  } while( true );
}

