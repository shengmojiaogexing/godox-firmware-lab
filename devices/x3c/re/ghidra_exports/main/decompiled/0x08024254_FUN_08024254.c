// 0x08024254 FUN_08024254 size=234

void FUN_08024254(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = DAT_08024340;
  iVar2 = FUN_0802fc9e(*(undefined4 *)(DAT_08024340 + 4),4);
  if (iVar2 == 0) {
    return;
  }
  if (*(int *)(param_1 + 0x50) == *(int *)(iVar1 + 4)) {
    return;
  }
  iVar2 = FUN_0802ed7a();
  if (*(int *)(param_1 + 0x50) == 0) {
    iVar3 = 0;
  }
  else {
    iVar3 = FUN_0802ed7a();
  }
  if (iVar2 == iVar3) {
    if (iVar2 == 0) {
      if (*(int *)(param_1 + 0x50) != 0) {
        FUN_0802ad74(*(int *)(param_1 + 0x50),0xf,*(undefined4 *)(iVar1 + 8));
        iVar2 = FUN_08024ea0(param_1);
        if (iVar2 != 0) {
          return;
        }
      }
      FUN_0802ad74(*(undefined4 *)(iVar1 + 4),0xe,*(undefined4 *)(iVar1 + 8));
      iVar2 = FUN_08024ea0(param_1);
    }
    else {
      FUN_0802b670(*(undefined4 *)(iVar1 + 4));
      iVar2 = FUN_08024ea0(param_1);
    }
    goto joined_r0x08024336;
  }
  if ((iVar3 == 0) && (*(int *)(param_1 + 0x50) != 0)) {
    FUN_0802ad74(*(int *)(param_1 + 0x50),0xf,*(undefined4 *)(iVar1 + 8));
    iVar3 = FUN_08024ea0(param_1);
joined_r0x08024320:
    if (iVar3 != 0) {
      return;
    }
  }
  else {
    iVar4 = *(int *)(param_1 + 0x50);
    if (iVar4 != 0) {
      if (iVar3 == 0) {
        FUN_0802ad74(iVar4,0xf,*(undefined4 *)(iVar1 + 8));
        iVar3 = FUN_08024ea0(param_1);
      }
      else {
        FUN_0802ad74(iVar4,0x10,*(undefined4 *)(iVar1 + 8));
        iVar3 = FUN_08024ea0(param_1);
      }
      goto joined_r0x08024320;
    }
  }
  if (iVar2 == 0) {
    FUN_0802ad74(*(undefined4 *)(iVar1 + 4),0xe,*(undefined4 *)(iVar1 + 8));
    iVar2 = FUN_08024ea0(param_1);
  }
  else {
    FUN_0802b670(*(undefined4 *)(iVar1 + 4));
    iVar2 = FUN_08024ea0(param_1);
  }
joined_r0x08024336:
  if (iVar2 == 0) {
    *(undefined4 *)(param_1 + 0x50) = *(undefined4 *)(iVar1 + 4);
  }
  return;
}

