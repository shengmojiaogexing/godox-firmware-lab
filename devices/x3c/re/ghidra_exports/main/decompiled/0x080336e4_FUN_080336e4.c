// 0x080336e4 FUN_080336e4 size=118

uint FUN_080336e4(undefined4 param_1,uint param_2)

{
  int iVar1;
  undefined4 uVar2;
  int extraout_r2;
  
  iVar1 = FUN_08033800();
  iVar1 = FUN_08018e78(extraout_r2 - iVar1,4);
  if ((param_2 & 3) == 0) {
    if (iVar1 - 0xcU < DAT_0803375c) {
      uVar2 = FUN_08035eaa(param_2,0xfffffffc);
      FUN_08019832(uVar2,iVar1);
      FUN_08019814(uVar2);
      FUN_08019828(uVar2);
      FUN_0801964e(param_1,uVar2);
      uVar2 = FUN_08019690(uVar2);
      FUN_08019832(uVar2,0);
      FUN_0801983e(uVar2);
      FUN_0801981e(uVar2);
    }
    else {
      param_2 = 0;
    }
  }
  else {
    param_2 = 0;
  }
  return param_2;
}

