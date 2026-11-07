// 0x08038042 FUN_08038042 size=130

undefined4
FUN_08038042(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            ,undefined4 param_6,undefined4 param_7)

{
  undefined4 uVar1;
  
  uVar1 = FUN_0803238c();
  FUN_080313b2(uVar1,param_6,param_7);
  thunk_FUN_08027790(uVar1,0,param_5);
  FUN_080315fa(uVar1,0,0x30000);
  FUN_080315fa(uVar1,0);
  FUN_080315fa(uVar1,0,0x20000);
  FUN_08031696(uVar1,0,0x20000);
  FUN_08031696(uVar1,0);
  if (param_2 == 0) {
    FUN_08031318(uVar1,param_3,param_4);
  }
  else {
    FUN_0802db44(uVar1,param_2,param_3,param_4);
  }
  return uVar1;
}

