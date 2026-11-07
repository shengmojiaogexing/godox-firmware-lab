// 0x08023028 FUN_08023028 size=200

void FUN_08023028(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined1 auStack_40 [8];
  int local_38;
  int local_34;
  undefined1 auStack_30 [32];
  
  iVar1 = FUN_08021fbc(param_1);
  iVar2 = FUN_080226d4(param_1);
  if ((iVar1 != 0) && (iVar2 != 0)) {
    FUN_08019b00(param_1,auStack_30);
    FUN_0802d8d6(auStack_40,0x10);
    iVar1 = FUN_0802f9bc(param_1,0,0x32);
    iVar2 = FUN_0802f9bc(param_1,0,0x12);
    iVar3 = FUN_0802f9bc(param_1,0,0x10);
    local_38 = FUN_0802f0cc(param_1);
    local_38 = (*(int *)(param_1 + 0x14) + iVar2 + iVar1) - local_38;
    local_34 = FUN_0802f0dc(param_1);
    local_34 = (*(int *)(param_1 + 0x18) + iVar1 + iVar3) - local_34;
    uVar4 = 0;
    while( true ) {
      if ((uint)(*(int **)(param_1 + 8))[1] <= uVar4) break;
      FUN_080251d8(*(undefined4 *)(**(int **)(param_1 + 8) + uVar4 * 4),auStack_30,auStack_40);
      uVar4 = uVar4 + 1;
    }
    FUN_08019fc8(auStack_30);
    iVar1 = FUN_0802fae8(param_1,0);
    iVar2 = FUN_0802f8a8(param_1,0);
    if ((iVar1 == DAT_080230f0) || (iVar2 == DAT_080230f0)) {
      FUN_08030958(param_1);
    }
    FUN_0802ad74(param_1,0x2b,0);
  }
  return;
}

