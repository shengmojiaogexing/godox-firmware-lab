// 0x08031318 FUN_08031318 size=68

void FUN_08031318(undefined4 param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  int extraout_r3;
  undefined1 auStack_90 [128];
  
  FUN_0803172a(param_1);
  iVar1 = FUN_0802edcc(param_1,8,auStack_90 + 0x80,0);
  if (((iVar1 == 1) && (extraout_r3 != param_3)) || (iVar1 == 0)) {
    FUN_080316f4(param_1,param_3,0);
  }
  return;
}

