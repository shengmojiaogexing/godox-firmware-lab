// 0x0802e384 FUN_0802e384 size=134

void FUN_0802e384(undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  
  iVar5 = *(int *)(param_2 + 4);
  if (iVar5 != 0) {
    iVar1 = FUN_0802ef1c(iVar5);
    iVar2 = FUN_0802f0bc(iVar5);
    iVar3 = FUN_0802f9bc(iVar5,0,0x10);
    iVar2 = (iVar3 + *(int *)(iVar5 + 0x18)) - iVar2;
    *(int *)(param_2 + 0x18) = iVar2;
    *(int *)(param_2 + 0x20) = iVar2 + -1;
    iVar3 = FUN_0802f9bc(iVar5,0,0x12);
    iVar1 = (iVar3 + *(int *)(iVar5 + 0x14)) - iVar1;
    *(int *)(param_2 + 0x14) = iVar1;
    *(int *)(param_2 + 0x1c) = iVar1 + -1;
  }
  *(undefined4 *)(param_2 + 0x24) = 0x1002;
  if (iVar5 != 0) {
    *(undefined4 *)(param_2 + 0x24) = 0x3002;
    *(uint *)(param_2 + 0x24) = *(uint *)(param_2 + 0x24) | 0x300;
  }
  uVar4 = *(uint *)(param_2 + 0x24);
  *(uint *)(param_2 + 0x24) = uVar4 | 0x874;
  if (iVar5 != 0) {
    *(uint *)(param_2 + 0x24) = uVar4 | 0x8874;
  }
  return;
}

