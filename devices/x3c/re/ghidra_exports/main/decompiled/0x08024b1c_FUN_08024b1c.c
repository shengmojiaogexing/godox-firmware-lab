// 0x08024b1c FUN_08024b1c size=534

void FUN_08024b1c(int param_1)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  
  iVar5 = DAT_08024d34;
  iVar2 = *(int *)(param_1 + 0x44);
  *(int *)(DAT_08024d34 + 4) = iVar2;
  if ((int)((uint)*(byte *)(param_1 + 1) << 0x1c) < 0) {
    return;
  }
  uVar7 = *(undefined4 *)(**(int **)(iVar5 + 8) + 0x10);
  bVar1 = false;
  if (iVar2 == 0) {
    uVar3 = FUN_08028270(uVar7);
    iVar2 = FUN_0802c89a(uVar3,param_1 + 4);
    *(int *)(iVar5 + 4) = iVar2;
    if (iVar2 == 0) {
      uVar3 = FUN_08028284(uVar7);
      uVar3 = FUN_0802c89a(uVar3,param_1 + 4);
      *(undefined4 *)(iVar5 + 4) = uVar3;
    }
    if (*(int *)(iVar5 + 4) == 0) {
      uVar7 = FUN_080282b0(uVar7);
      uVar7 = FUN_0802c89a(uVar7,param_1 + 4);
      *(undefined4 *)(iVar5 + 4) = uVar7;
    }
    bVar1 = true;
  }
  else if ((*(int *)(param_1 + 0x4c) == 0) && (iVar2 = FUN_0802fc9e(iVar2,0x2000), iVar2 == 0)) {
    uVar3 = FUN_08028270(uVar7);
    iVar2 = FUN_0802c89a(uVar3,param_1 + 4);
    *(int *)(iVar5 + 4) = iVar2;
    if (iVar2 == 0) {
      uVar3 = FUN_08028284(uVar7);
      uVar3 = FUN_0802c89a(uVar3,param_1 + 4);
      *(undefined4 *)(iVar5 + 4) = uVar3;
    }
    if (*(int *)(iVar5 + 4) == 0) {
      uVar7 = FUN_080282b0(uVar7);
      uVar7 = FUN_0802c89a(uVar7,param_1 + 4);
      *(undefined4 *)(iVar5 + 4) = uVar7;
    }
    bVar1 = true;
  }
  if ((bVar1) && (*(int *)(param_1 + 0x48) != 0)) {
    *(undefined4 *)(param_1 + 0x34) = 0;
    *(undefined4 *)(param_1 + 0x38) = 0;
    FUN_08017c32(param_1);
    iVar2 = FUN_08024ea0(param_1);
    if (iVar2 != 0) {
      return;
    }
  }
  iVar2 = *(int *)(param_1 + 0x44);
  if (iVar2 != *(int *)(iVar5 + 4)) {
    *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_1 + 4);
    *(undefined4 *)(param_1 + 0x18) = *(undefined4 *)(param_1 + 8);
    if (iVar2 != 0) {
      FUN_0802ad74(iVar2,3,*(undefined4 *)(iVar5 + 8));
      iVar2 = FUN_08024ea0(param_1);
      if (iVar2 != 0) {
        return;
      }
    }
    iVar2 = *(int *)(iVar5 + 4);
    *(int *)(param_1 + 0x44) = iVar2;
    *(int *)(param_1 + 0x48) = iVar2;
    if (iVar2 != 0) {
      uVar7 = FUN_080334d8();
      *(undefined4 *)(param_1 + 0x70) = uVar7;
      *(byte *)(param_1 + 1) = *(byte *)(param_1 + 1) & 0xfe;
      *(undefined4 *)(param_1 + 0x2c) = 0;
      *(undefined4 *)(param_1 + 0x30) = 0;
      *(undefined1 *)(param_1 + 0x6c) = 0;
      *(undefined1 *)(param_1 + 0x6d) = 0;
      *(undefined4 *)(param_1 + 100) = 0;
      *(undefined4 *)(param_1 + 0x68) = 0;
      *(undefined4 *)(param_1 + 0x24) = 0;
      *(undefined4 *)(param_1 + 0x28) = 0;
      FUN_0802ad74(*(undefined4 *)(iVar5 + 4),1,*(undefined4 *)(iVar5 + 8));
      iVar2 = FUN_08024ea0(param_1);
      if (iVar2 != 0) {
        return;
      }
      if ((int)((uint)*(byte *)(*(int *)(iVar5 + 8) + 5) << 0x1c) < 0) {
        return;
      }
      FUN_08024254(*(int *)(iVar5 + 8) + 4);
      iVar2 = FUN_08024ea0(param_1);
      if (iVar2 != 0) {
        return;
      }
    }
  }
  iVar2 = *(int *)(param_1 + 4) - *(int *)(param_1 + 0x14);
  *(int *)(param_1 + 0x24) = iVar2;
  iVar6 = *(int *)(param_1 + 8) - *(int *)(param_1 + 0x18);
  *(int *)(param_1 + 0x28) = iVar6;
  iVar2 = (iVar2 + *(int *)(param_1 + 0x34)) / 2;
  *(int *)(param_1 + 0x34) = iVar2;
  iVar6 = (iVar6 + *(int *)(param_1 + 0x38)) / 2;
  *(int *)(param_1 + 0x38) = iVar6;
  *(int *)(param_1 + 0x3c) = iVar2;
  *(int *)(param_1 + 0x40) = iVar6;
  if (*(int *)(iVar5 + 4) != 0) {
    FUN_0802ad74(*(int *)(iVar5 + 4),2,*(undefined4 *)(iVar5 + 8));
    iVar2 = FUN_08024ea0(param_1);
    if ((iVar2 == 0) && (-1 < (int)((uint)*(byte *)(*(int *)(iVar5 + 8) + 5) << 0x1c))) {
      FUN_08017ade(param_1);
      iVar2 = FUN_08024ea0(param_1);
      if (iVar2 == 0) {
        FUN_080246fc(param_1);
        iVar2 = FUN_08024ea0(param_1);
        if (iVar2 == 0) {
          if (((*(int *)(param_1 + 0x4c) == 0) && ((*(byte *)(param_1 + 1) & 1) == 0)) &&
             (uVar4 = FUN_080334c0(*(undefined4 *)(param_1 + 0x70)),
             *(ushort *)(**(int **)(iVar5 + 8) + 0x1c) < uVar4)) {
            FUN_0802ad74(*(undefined4 *)(iVar5 + 4),5);
            iVar2 = FUN_08024ea0(param_1);
            if (iVar2 != 0) {
              return;
            }
            *(byte *)(param_1 + 1) = *(byte *)(param_1 + 1) | 1;
            uVar7 = FUN_080334d8();
            *(undefined4 *)(param_1 + 0x74) = uVar7;
          }
          if (((*(int *)(param_1 + 0x4c) == 0) && ((*(byte *)(param_1 + 1) & 1) != 0)) &&
             (uVar4 = FUN_080334c0(*(undefined4 *)(param_1 + 0x74)),
             *(ushort *)(**(int **)(iVar5 + 8) + 0x1e) < uVar4)) {
            FUN_0802ad74(*(undefined4 *)(iVar5 + 4),6);
            iVar5 = FUN_08024ea0(param_1);
            if (iVar5 == 0) {
              uVar7 = FUN_080334d8();
              *(undefined4 *)(param_1 + 0x74) = uVar7;
            }
          }
        }
      }
    }
  }
  return;
}

