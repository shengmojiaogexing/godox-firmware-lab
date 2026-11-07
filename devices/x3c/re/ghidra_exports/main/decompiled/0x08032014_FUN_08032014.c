// 0x08032014 FUN_08032014 size=56

void FUN_08032014(undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int extraout_r3;
  undefined1 auStack_98 [128];
  
  iVar1 = FUN_0802faa0(param_1,0);
  iVar2 = FUN_0802fab2(param_1,0);
  iVar3 = FUN_0802f870(param_1,0);
  iVar2 = param_2 * (*(int *)(iVar1 + 8) + iVar2) + iVar3 * 2;
  iVar1 = FUN_0802edcc(param_1,4,auStack_98 + 0x88,0);
  if (((iVar1 == 1) && (extraout_r3 != iVar2)) || (iVar1 == 0)) {
    FUN_080315dc(param_1,iVar2,0);
  }
  return;
}

