// 0x08019b00 FUN_08019b00 size=258

void FUN_08019b00(int param_1,undefined4 *param_2)

{
  undefined1 uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  uint uVar9;
  undefined4 uVar10;
  
  iVar2 = FUN_0802ebc8(param_1,0);
  if (iVar2 == 0) {
    FUN_0802d8d6(param_2,0x20);
    return;
  }
  FUN_08019fe8(param_1,param_2);
  FUN_08019c0c(param_1,param_2);
  uVar3 = FUN_0802f92c(param_1,0);
  uVar4 = FUN_0802f98c(param_1,0);
  iVar5 = FUN_0802f804(param_1,0);
  iVar6 = FUN_0802fae8(param_1,0);
  iVar7 = FUN_0802f8a8(param_1,0);
  iVar2 = DAT_08019c04;
  if ((iVar6 == DAT_08019c04) && (-1 < (int)((uint)*(ushort *)(param_1 + 0x2a) << 0x15))) {
    uVar10 = 1;
  }
  else {
    uVar10 = 0;
  }
  uVar8 = FUN_0802ecd0(param_1);
  iVar6 = DAT_08019c08;
  uVar9 = FUN_0802f9bc(param_1,0,*(undefined2 *)(DAT_08019c08 + 4));
  uVar3 = FUN_08022f40(uVar8,uVar10,uVar9 & 0xff,uVar3,param_2[4],param_2[2],*param_2,iVar5 == 1,
                       uVar9);
  param_2[6] = uVar3;
  if ((iVar7 == iVar2) && (-1 < (int)((uint)*(ushort *)(param_1 + 0x2a) << 0x16))) {
    uVar3 = 1;
  }
  else {
    uVar3 = 0;
  }
  uVar10 = FUN_0802ec9c(param_1);
  uVar1 = FUN_0802f9bc(param_1,0,*(undefined2 *)(iVar6 + 8));
  uVar3 = FUN_08022f40(uVar10,uVar3,uVar1,uVar4,param_2[5],param_2[3],param_2[1],0,uVar9);
  param_2[7] = uVar3;
  return;
}

