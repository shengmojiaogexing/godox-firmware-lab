// 0x08010acc FUN_08010acc size=72

undefined4
FUN_08010acc(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            ,undefined4 param_6)

{
  undefined4 uVar1;
  
  uVar1 = FUN_0802e40c();
  FUN_08030e2a(uVar1,0,DAT_08010b14);
  if (param_2 == 0) {
    FUN_08031318(uVar1,param_3,param_4);
  }
  else {
    FUN_0802db44(uVar1,param_2,param_3,param_4);
  }
  FUN_080313b2(uVar1,param_5,param_6);
  return uVar1;
}

