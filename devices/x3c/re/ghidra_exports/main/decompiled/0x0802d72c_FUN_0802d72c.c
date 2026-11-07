// 0x0802d72c FUN_0802d72c size=52

int FUN_0802d72c(int param_1,int param_2)

{
  int iVar1;
  
  if (param_2 == 0) {
    FUN_0802d6e4(param_1);
    iVar1 = DAT_0802d760;
  }
  else {
    if (param_1 == DAT_0802d760) {
      iVar1 = FUN_0802d5c4();
      return iVar1;
    }
    iVar1 = FUN_08033804(*(undefined4 *)(DAT_0802d760 + 4),param_1,param_2);
    if (iVar1 == 0) {
      return 0;
    }
  }
  return iVar1;
}

