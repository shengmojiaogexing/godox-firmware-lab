// 0x08024f38 FUN_08024f38 size=342

void FUN_08024f38(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  
  iVar5 = *(int *)(param_1 + 0x4c);
  iVar3 = FUN_0802fc9e(iVar5,0x80,param_3,param_4,param_4);
  iVar2 = DAT_08025090;
  iVar1 = -DAT_08025090;
  if (iVar3 == 0) {
    FUN_08027518(param_1 + 0x54,DAT_08025090,DAT_08025090,iVar1,iVar1);
  }
  else {
    iVar3 = FUN_0802f0a8(iVar5);
    if (iVar3 == 1) {
      uVar4 = FUN_080212e8(iVar5,*(int *)(iVar5 + 0x18) + 1,iVar1,0);
      *(undefined4 *)(param_1 + 0x58) = uVar4;
      uVar4 = FUN_080212e8(iVar5,iVar2,*(int *)(iVar5 + 0x18) + -1,0);
      *(undefined4 *)(param_1 + 0x60) = uVar4;
    }
    else if (iVar3 == 2) {
      uVar4 = FUN_080212e8(iVar5,*(undefined4 *)(iVar5 + 0x20),iVar1,0);
      *(undefined4 *)(param_1 + 0x58) = uVar4;
      uVar4 = FUN_080212e8(iVar5,iVar2,*(undefined4 *)(iVar5 + 0x20),0);
      *(undefined4 *)(param_1 + 0x60) = uVar4;
    }
    else if (iVar3 == 3) {
      iVar3 = *(int *)(iVar5 + 0x18) + ((*(int *)(iVar5 + 0x20) - *(int *)(iVar5 + 0x18)) + 1) / 2;
      uVar4 = FUN_080212e8(iVar5,iVar3 + 1,iVar1,0);
      *(undefined4 *)(param_1 + 0x58) = uVar4;
      uVar4 = FUN_080212e8(iVar5,iVar2,iVar3 + -1,0);
      *(undefined4 *)(param_1 + 0x60) = uVar4;
    }
    else {
      *(int *)(param_1 + 0x58) = iVar2;
      *(int *)(param_1 + 0x60) = iVar1;
    }
    iVar3 = FUN_0802f094(iVar5);
    if (iVar3 == 1) {
      uVar4 = FUN_080211f0(iVar5,*(undefined4 *)(iVar5 + 0x14),iVar1,0);
      *(undefined4 *)(param_1 + 0x54) = uVar4;
      uVar4 = FUN_080211f0(iVar5,iVar2,*(undefined4 *)(iVar5 + 0x14),0);
      *(undefined4 *)(param_1 + 0x5c) = uVar4;
    }
    else if (iVar3 == 2) {
      uVar4 = FUN_080211f0(iVar5,*(undefined4 *)(iVar5 + 0x1c),iVar1,0);
      *(undefined4 *)(param_1 + 0x54) = uVar4;
      uVar4 = FUN_080211f0(iVar5,iVar2,*(undefined4 *)(iVar5 + 0x1c),0);
      *(undefined4 *)(param_1 + 0x5c) = uVar4;
    }
    else if (iVar3 == 3) {
      iVar3 = *(int *)(iVar5 + 0x14) + ((*(int *)(iVar5 + 0x1c) - *(int *)(iVar5 + 0x14)) + 1) / 2;
      uVar4 = FUN_080211f0(iVar5,iVar3 + 1,iVar1,0);
      *(undefined4 *)(param_1 + 0x54) = uVar4;
      uVar4 = FUN_080211f0(iVar5,iVar2,iVar3 + -1,0);
      *(undefined4 *)(param_1 + 0x5c) = uVar4;
    }
    else {
      *(int *)(param_1 + 0x54) = iVar2;
      *(int *)(param_1 + 0x5c) = iVar1;
    }
  }
  if (*(int *)(param_1 + 0x54) == 0) {
    *(int *)(param_1 + 0x54) = iVar2;
  }
  if (*(int *)(param_1 + 0x5c) == 0) {
    *(int *)(param_1 + 0x5c) = iVar1;
  }
  if (*(int *)(param_1 + 0x58) == 0) {
    *(int *)(param_1 + 0x58) = iVar2;
  }
  if (*(int *)(param_1 + 0x60) == 0) {
    *(int *)(param_1 + 0x60) = iVar1;
  }
  return;
}

