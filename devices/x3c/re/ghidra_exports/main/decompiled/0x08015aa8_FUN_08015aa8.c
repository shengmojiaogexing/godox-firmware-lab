// 0x08015aa8 FUN_08015aa8 size=142

int FUN_08015aa8(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,
                undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  int iVar1;
  
  iVar1 = FUN_0803238c();
  *(byte *)(iVar1 + 0x94) = *(byte *)(iVar1 + 0x94) | 0x40;
  FUN_080313b2(iVar1,param_6,param_7);
  thunk_FUN_08027790(iVar1,0,param_5);
  FUN_080315fa(iVar1,0,0x30000);
  FUN_080315fa(iVar1,0);
  FUN_080315fa(iVar1,0,0x20000);
  FUN_08031696(iVar1,0,0x20000);
  FUN_08031696(iVar1,0);
  if (param_2 == 0) {
    FUN_08031318(iVar1,param_3,param_4);
  }
  else {
    FUN_0802db44(iVar1,param_2,param_3,param_4);
  }
  return iVar1;
}

