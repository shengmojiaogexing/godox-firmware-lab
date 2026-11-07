// 0x080246fc FUN_080246fc size=260

void FUN_080246fc(int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  if ((*(int *)(param_1 + 0x4c) == 0) && ((*(byte *)(param_1 + 0x6d) & 1) == 0)) {
    iVar6 = *(int *)(param_1 + 0x44);
    while ((iVar6 != 0 && (iVar1 = FUN_0802fc9e(iVar6,0x8000), iVar1 != 0))) {
      iVar6 = FUN_0802ee0a(iVar6);
    }
    if (iVar6 != 0) {
      iVar1 = *(int *)(param_1 + 0x24);
      if (iVar1 < 1) {
        iVar2 = -iVar1;
      }
      else {
        iVar2 = iVar1;
      }
      piVar3 = *(int **)(DAT_08024800 + 8);
      uVar4 = (uint)*(byte *)(*piVar3 + 0x1a);
      if (iVar2 < (int)uVar4) {
        iVar2 = *(int *)(param_1 + 0x28);
        if (iVar2 < 1) {
          iVar2 = -iVar2;
        }
        if (iVar2 < (int)uVar4) {
          *(undefined4 *)(param_1 + 100) = 0;
          *(undefined4 *)(param_1 + 0x68) = 0;
        }
      }
      iVar1 = *(int *)(param_1 + 100) + iVar1;
      *(int *)(param_1 + 100) = iVar1;
      iVar2 = *(int *)(param_1 + 0x68) + *(int *)(param_1 + 0x28);
      *(int *)(param_1 + 0x68) = iVar2;
      if (iVar1 < 1) {
        iVar5 = -iVar1;
      }
      else {
        iVar5 = iVar1;
      }
      uVar4 = (uint)*(byte *)(*piVar3 + 0x1b);
      if (iVar5 <= (int)uVar4) {
        if (iVar2 < 1) {
          iVar5 = -iVar2;
        }
        else {
          iVar5 = iVar2;
        }
        if (iVar5 <= (int)uVar4) {
          return;
        }
      }
      *(undefined1 *)(param_1 + 0x6d) = 1;
      if (iVar1 < 1) {
        iVar5 = -iVar1;
      }
      else {
        iVar5 = iVar1;
      }
      if (iVar2 < 1) {
        iVar7 = -iVar2;
      }
      else {
        iVar7 = iVar2;
      }
      if (iVar7 < iVar5) {
        if (iVar1 < 1) {
          *(byte *)(param_1 + 0x6c) = (*(byte *)(param_1 + 0x6c) & 0xf) + 0x10;
        }
        else {
          *(byte *)(param_1 + 0x6c) = (*(byte *)(param_1 + 0x6c) & 0xf) + 0x20;
        }
      }
      else if (iVar2 < 1) {
        *(byte *)(param_1 + 0x6c) = (*(byte *)(param_1 + 0x6c) & 0xf) + 0x40;
      }
      else {
        *(byte *)(param_1 + 0x6c) = (*(byte *)(param_1 + 0x6c) & 0xf) + 0x80;
      }
      FUN_0802ad74(iVar6,0xc);
      FUN_08024ea0(param_1);
      return;
    }
  }
  return;
}

