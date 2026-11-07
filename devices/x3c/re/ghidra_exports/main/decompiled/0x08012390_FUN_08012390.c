// 0x08012390 FUN_08012390 size=124

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08012390(void)

{
  undefined2 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined2 uVar4;
  undefined1 auStack_18 [12];
  
  uVar4 = 0;
  uVar2 = FUN_08010acc(0,0,0,0,0x170,0x1c0);
  uVar3 = FUN_08027a78();
  FUN_0803157c(uVar2,uVar3,0);
  FUN_08031594(uVar2,0xff,0);
  uVar1 = FUN_08027f68();
  uVar3 = FUN_0802553e(uVar2,1,0x1e,0x14,DAT_08012414,CONCAT22(uVar4,uVar1),s_CH_21_0801240c);
  FUN_08004c48(auStack_18,10,s_CH__d_0801241b + 1,*_DAT_08012418);
  FUN_0802d33c(uVar3,auStack_18);
  FUN_0802832e(uVar2);
  return;
}

