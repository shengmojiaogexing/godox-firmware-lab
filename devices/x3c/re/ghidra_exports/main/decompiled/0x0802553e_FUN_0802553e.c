// 0x0802553e FUN_0802553e size=94

undefined4
FUN_0802553e(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            ,undefined4 param_6,undefined4 param_7)

{
  undefined4 uVar1;
  undefined1 auStack_50 [52];
  
  uVar1 = FUN_0802c9fc();
  FUN_080316cc(uVar1,param_5,0);
  FUN_080316b4(uVar1,param_6,0);
  FUN_08004c48(auStack_50,0x32,param_7);
  FUN_0802d33c(uVar1,auStack_50);
  if (param_2 == 0) {
    FUN_08031318(uVar1,param_3,param_4);
  }
  else {
    FUN_0802db44(uVar1,param_2,param_3,param_4);
  }
  return uVar1;
}

