// 0x08031fba FUN_08031fba size=362

void FUN_08031fba(int param_1,uint param_2,int param_3)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int extraout_r3;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  int local_64;
  undefined4 local_60;
  undefined4 local_58;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  int local_3c;
  uint local_38;
  uint local_24;
  undefined1 auStack_20 [16];
  
  uVar5 = (uint)*(ushort *)(param_1 + 0x2c);
  if ((*(byte *)(param_1 + 0x32) & 1) != 0) {
    uVar4 = uVar5 / 7;
    uVar6 = *(ushort *)(param_1 + 0x2e) / uVar4;
    if (param_2 < uVar4) {
      uVar7 = (uint)*(ushort *)(param_1 + 0x2e) - uVar6 * uVar4;
      iVar10 = (int)(short)uVar7;
      uVar9 = iVar10 - param_2;
      if ((int)uVar9 < 1) {
        uVar9 = param_2 - iVar10;
      }
      if (uVar4 >> 1 < uVar9) {
        if (param_2 < (uVar7 & 0xffff)) {
          param_2 = param_2 + uVar4;
        }
        else {
          param_2 = param_2 - uVar4;
        }
      }
      param_2 = uVar4 * uVar6 + param_2 & 0xffff;
    }
  }
  if (uVar5 <= param_2) {
    param_2 = uVar5 - 1;
  }
  *(short *)(param_1 + 0x2e) = (short)param_2;
  *(short *)(param_1 + 0x30) = (short)param_2;
  iVar10 = FUN_080223b4(param_1);
  if (iVar10 != 0) {
    uVar1 = FUN_0802cd72();
    iVar2 = FUN_0802e0f8(iVar10,0,uVar1);
    if (iVar2 == 1) {
      FUN_0803172a(iVar10,0);
    }
    else if (iVar2 == 2) {
      iVar2 = FUN_0802ecd0(param_1);
      iVar3 = FUN_0802fc5e(iVar10);
      FUN_0803172a(iVar10,(iVar2 - iVar3) / 2);
    }
    else if (iVar2 == 3) {
      iVar2 = FUN_0802ecd0(param_1);
      iVar3 = FUN_0802fc5e(iVar10);
      FUN_0803172a(iVar10,iVar2 - iVar3);
    }
    iVar2 = FUN_0802faa0(param_1,0);
    iVar3 = FUN_0802fab2(param_1,0);
    iVar8 = *(int *)(iVar2 + 8);
    iVar2 = FUN_0802ec9c(param_1);
    local_24 = FUN_0802f9bc(param_1,0,100);
    uVar5 = local_24 & 0xffff;
    if ((param_3 == 0) || (uVar5 == 0)) {
      FUN_08024eb8(param_1);
    }
    iVar2 = (iVar2 / 2 - iVar8 / 2) - (uint)*(ushort *)(param_1 + 0x2e) * (iVar8 + iVar3);
    if ((param_3 == 0) || (uVar5 == 0)) {
      FUN_08026938(iVar10,DAT_080367f8);
      iVar3 = FUN_0802edcc(iVar10,8,auStack_20 + 0x10,0);
      if (((iVar3 == 1) && (extraout_r3 != iVar2)) || (iVar3 == 0)) {
        FUN_080316f4(iVar10,iVar2,0);
      }
      return;
    }
    FUN_080269c8(&local_64);
    local_60 = DAT_080367f8;
    local_64 = iVar10;
    local_44 = FUN_0802fc68(iVar10);
    local_58 = DAT_080367fc;
    local_48 = DAT_08036800;
    local_40 = local_44;
    local_3c = iVar2;
    local_38 = uVar5;
    FUN_08026aac(&local_64);
  }
  return;
}

