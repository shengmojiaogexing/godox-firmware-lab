// 0x08017c32 FUN_08017c32 size=494

void FUN_08017c32(int param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  byte bVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int local_30;
  int *local_2c;
  
  iVar8 = *(int *)(param_1 + 0x4c);
  if ((iVar8 != 0) && ((*(byte *)(param_1 + 0x6c) & 0xf) != 0)) {
    local_30 = param_3;
    local_2c = (int *)param_4;
    local_2c = (int *)FUN_0802c5fc();
    bVar1 = *(byte *)(*local_2c + 0x19);
    iVar3 = FUN_0802fc9e(iVar8,0x40);
    if (iVar3 == 0) {
      *(undefined4 *)(param_1 + 0x38) = 0;
      *(undefined4 *)(param_1 + 0x34) = 0;
    }
    iVar3 = FUN_0802f094(iVar8);
    iVar4 = FUN_0802f0a8(iVar8);
    bVar2 = *(byte *)(param_1 + 0x6c) & 0xf;
    if (bVar2 == 0xc) {
      *(undefined4 *)(param_1 + 0x34) = 0;
      if (iVar4 == 0) {
        *(int *)(param_1 + 0x38) = (int)((100 - (uint)bVar1) * *(int *)(param_1 + 0x38)) / 100;
        uVar5 = FUN_0802ee28(iVar8);
        uVar6 = FUN_0802f0bc(iVar8);
        local_30 = 0xc;
        uVar5 = FUN_0801fd5a(iVar8,*(undefined4 *)(param_1 + 0x38),uVar6,uVar5);
        *(undefined4 *)(param_1 + 0x38) = uVar5;
        FUN_08030f80(iVar8,0,uVar5);
      }
      else {
        local_30 = FUN_08037184(param_1);
        *(undefined4 *)(param_1 + 0x38) = 0;
        FUN_08037110(param_1,0,&local_30);
        iVar7 = FUN_080212e8(iVar8,0xe0000001,0x1fffffff,local_30);
        FUN_08030f80(iVar8,0,local_30 + iVar7,1);
      }
    }
    else if (bVar2 == 3) {
      *(undefined4 *)(param_1 + 0x38) = 0;
      if (iVar3 == 0) {
        *(int *)(param_1 + 0x34) = (int)((100 - (uint)bVar1) * *(int *)(param_1 + 0x34)) / 100;
        uVar5 = FUN_0802ef1c(iVar8);
        uVar6 = FUN_0802efdc(iVar8);
        local_30 = 3;
        uVar5 = FUN_0801fd5a(iVar8,*(undefined4 *)(param_1 + 0x34),uVar5,uVar6);
        *(undefined4 *)(param_1 + 0x34) = uVar5;
        FUN_08030f80(iVar8,uVar5,0);
      }
      else {
        local_30 = FUN_0803715c(param_1);
        *(undefined4 *)(param_1 + 0x34) = 0;
        FUN_08037110(param_1,&local_30,0);
        iVar7 = FUN_080211f0(iVar8,0xe0000001,0x1fffffff,local_30);
        FUN_08030f80(iVar8,local_30 + iVar7,0,1);
      }
    }
    if ((*(int *)(param_1 + 0x34) == 0) && (*(int *)(param_1 + 0x38) == 0)) {
      if (iVar4 == 0) {
        iVar4 = FUN_0802f0bc(iVar8);
        iVar7 = FUN_0802ee28(iVar8);
        if ((0 < iVar4) || (0 < iVar7)) {
          if (iVar4 < 0) {
            FUN_08030f80(iVar8,0,iVar4,1);
          }
          else if (iVar7 < 0) {
            FUN_08030f80(iVar8,0,-iVar7,1);
          }
        }
      }
      if (iVar3 == 0) {
        iVar3 = FUN_0802ef1c(iVar8);
        iVar4 = FUN_0802efdc(iVar8);
        if ((0 < iVar3) || (0 < iVar4)) {
          if (iVar3 < 0) {
            FUN_08030f80(iVar8,iVar3,0,1);
          }
          else if (iVar4 < 0) {
            FUN_08030f80(iVar8,-iVar4,0,1);
          }
        }
      }
      FUN_0802ad74(iVar8,10,local_2c);
      if (-1 < (int)((uint)*(byte *)(param_1 + 1) << 0x1e)) {
        *(byte *)(param_1 + 0x6c) = *(byte *)(param_1 + 0x6c) & 0xf0;
        *(undefined4 *)(param_1 + 0x4c) = 0;
      }
    }
  }
  return;
}

