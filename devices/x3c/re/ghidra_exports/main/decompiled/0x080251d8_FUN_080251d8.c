// 0x080251d8 FUN_080251d8 size=736

void FUN_080251d8(int param_1,int *param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  uint local_3c;
  int local_38;
  uint local_34;
  int iStack_30;
  int *piStack_2c;
  int local_28;
  
  iStack_30 = param_1;
  piStack_2c = param_2;
  local_28 = param_3;
  iVar1 = FUN_0802fcac(param_1,DAT_080254b8);
  if (iVar1 == 0) {
    iVar1 = FUN_08021fe0(param_1);
    iVar2 = FUN_080226f8(param_1);
    if ((iVar2 != 0) && (iVar1 != 0)) {
      iVar3 = FUN_08021fcc(param_1);
      iVar4 = FUN_080226e4(param_1);
      iVar10 = DAT_080254bc;
      local_3c = FUN_0802f9bc(param_1,0,*(undefined2 *)(DAT_080254bc + 0xe));
      uVar9 = local_3c & 0xff;
      local_3c = FUN_0802f9bc(param_1,0,*(undefined2 *)(iVar10 + 0x14));
      local_34 = local_3c & 0xff;
      iVar1 = iVar3 + iVar1 + -1;
      iVar10 = (*(int *)(param_2[2] + iVar1 * 4) + *(int *)(*param_2 + iVar1 * 4)) -
               *(int *)(*param_2 + iVar3 * 4);
      iVar1 = iVar4 + iVar2 + -1;
      local_38 = (*(int *)(param_2[3] + iVar1 * 4) + *(int *)(param_2[1] + iVar1 * 4)) -
                 *(int *)(param_2[1] + iVar4 * 4);
      iVar1 = FUN_0802f804(param_1,0);
      if (iVar1 == 1) {
        if (uVar9 == 0) {
          uVar9 = 2;
        }
        else if (uVar9 == 2) {
          uVar9 = 0;
        }
      }
      iVar1 = (*(int *)(param_1 + 0x1c) - *(int *)(param_1 + 0x14)) + 1;
      iVar2 = (*(int *)(param_1 + 0x20) - *(int *)(param_1 + 0x18)) + 1;
      if (uVar9 == 1) {
        iVar8 = *(int *)(*param_2 + iVar3 * 4) + (iVar10 - iVar1) / 2;
        *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) & 0xfbff;
      }
      else if (uVar9 == 2) {
        iVar8 = FUN_0802fc5e(param_1);
        iVar8 = (*(int *)(*param_2 + iVar3 * 4) + iVar10) - iVar8;
        *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) & 0xfbff;
      }
      else if (uVar9 == 3) {
        iVar8 = *(int *)(*param_2 + iVar3 * 4);
        *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) | 0x400;
        iVar1 = iVar10;
      }
      else {
        iVar8 = *(int *)(*param_2 + iVar3 * 4);
        *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) & 0xfbff;
      }
      if (local_34 == 1) {
        iVar10 = *(int *)(param_2[1] + iVar4 * 4) + (local_38 - iVar2) / 2;
        *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) & 0xfdff;
      }
      else if (local_34 == 2) {
        iVar10 = FUN_0802ed88(param_1);
        iVar10 = (local_38 + *(int *)(param_2[1] + iVar4 * 4)) - iVar10;
        *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) & 0xfdff;
      }
      else if (local_34 == 3) {
        iVar10 = *(int *)(param_2[1] + iVar4 * 4);
        *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) | 0x200;
        iVar2 = local_38;
      }
      else {
        iVar10 = *(int *)(param_2[1] + iVar4 * 4);
        *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) & 0xfdff;
      }
      iVar3 = FUN_0802fc5e(param_1);
      if ((iVar3 != iVar1) || (iVar3 = FUN_0802ed88(param_1), iVar3 != iVar2)) {
        local_48 = *(undefined4 *)(param_1 + 0x14);
        local_44 = *(undefined4 *)(param_1 + 0x18);
        local_40 = *(undefined4 *)(param_1 + 0x1c);
        local_3c = *(uint *)(param_1 + 0x20);
        FUN_080302d0(param_1);
        FUN_08027530(param_1 + 0x14,iVar1);
        FUN_08027526(param_1 + 0x14,iVar2);
        FUN_080302d0(param_1);
        FUN_0802ad74(param_1,0x29,&local_48);
        uVar5 = FUN_0802ee0a(param_1);
        FUN_0802ad74(uVar5,0x22,param_1);
      }
      uVar9 = FUN_0802f9bc(param_1,0,0x6a);
      local_3c = uVar9;
      uVar6 = FUN_0802f9bc(param_1,0,0x6b);
      local_3c = uVar6;
      iVar1 = FUN_0802fc5e(param_1);
      iVar2 = FUN_0802ed88(param_1);
      if (((uVar9 & 0x7fffffff) >> 0x1d == 1) && (uVar7 = uVar9 & 0x9fffffff, (int)uVar7 < 0x7d1)) {
        if (1000 < (int)uVar7) {
          uVar7 = 1000 - uVar7;
        }
        uVar9 = (int)(uVar7 * iVar1) / 100;
      }
      if (((uVar6 & 0x7fffffff) >> 0x1d == 1) && (uVar7 = uVar6 & 0x9fffffff, (int)uVar7 < 0x7d1)) {
        if (1000 < (int)uVar7) {
          uVar7 = 1000 - uVar7;
        }
        uVar6 = (int)(iVar2 * uVar7) / 100;
      }
      iVar1 = (iVar8 + uVar9 + *(int *)(local_28 + 8)) - *(int *)(param_1 + 0x14);
      iVar2 = (*(int *)(local_28 + 0xc) + iVar10 + uVar6) - *(int *)(param_1 + 0x18);
      if (iVar1 != 0 || iVar2 != 0) {
        FUN_080302d0(param_1);
        *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + iVar1;
        *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + iVar1;
        *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + iVar2;
        *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + iVar2;
        FUN_080302d0(param_1);
        FUN_08030460(param_1,iVar1,iVar2,0);
        return;
      }
    }
  }
  return;
}

