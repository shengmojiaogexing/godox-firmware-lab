// 0x080313b2 FUN_080313b2 size=68

void FUN_080313b2(undefined4 param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  int extraout_r3;
  undefined1 auStack_90 [128];
  
  FUN_080316fe(param_1);
  iVar1 = FUN_0802edcc(param_1,4,auStack_90 + 0x80,0);
  if (((iVar1 == 1) && (extraout_r3 != param_3)) || (iVar1 == 0)) {
    FUN_080315dc(param_1,param_3,0);
  }
  return;
}

