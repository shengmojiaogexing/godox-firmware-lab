// 0x08036a14 FUN_08036a14 size=140

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08036a14(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined2 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined2 uVar4;
  
  FUN_08033540(*DAT_08036aa0);
  FUN_08033540(*DAT_08036aa4);
  FUN_08033540(*(undefined4 *)(_DAT_08036aa8 + 0x40));
  uVar4 = 0;
  uVar2 = FUN_08010acc(0,9,0,0,0x170,0x1c0,param_3,param_4);
  uVar3 = FUN_08027ac8();
  FUN_0803157c(uVar2,uVar3,0);
  FUN_08031594(uVar2,0xff,0);
  uVar1 = FUN_08027fb8();
  FUN_0802553e(uVar2,9,0,0,DAT_08036ab8,CONCAT22(uVar4,uVar1),s_RESET_OK_08036aab + 1);
  FUN_0802821c();
  uVar3 = FUN_080282b0();
  FUN_0802832e(uVar2);
  FUN_0802e428(uVar3);
  return;
}

