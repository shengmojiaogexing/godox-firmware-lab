// 0x08030958 FUN_08030958 size=706

undefined4 FUN_08030958(int param_1)

{
  bool bVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  undefined4 uVar11;
  int iVar12;
  bool bVar13;
  bool bVar14;
  undefined1 auStack_4c [16];
  undefined1 auStack_3c [16];
  undefined4 local_2c;
  undefined4 local_28;
  
  if (((int)((uint)*(ushort *)(param_1 + 0x2a) << 0x15) < 0) &&
     ((int)((uint)*(ushort *)(param_1 + 0x2a) << 0x16) < 0)) {
    uVar2 = 0;
  }
  else {
    iVar3 = FUN_0802ee0a(param_1);
    if (iVar3 == 0) {
      uVar2 = 0;
    }
    else {
      local_28 = FUN_0802ef1c(param_1);
      uVar7 = DAT_08030c1c;
      bVar13 = false;
      if ((int)((uint)*(ushort *)(param_1 + 0x2a) << 0x15) < 0) {
        iVar4 = FUN_0802fc5e(param_1);
      }
      else {
        uVar8 = FUN_0802fadc(param_1,0);
        bVar13 = uVar8 == uVar7;
        if (((uVar8 & 0x7fffffff) >> 0x1d == 1) && ((int)(uVar8 & 0x9fffffff) < 0x7d1)) {
          bVar14 = true;
        }
        else {
          bVar14 = false;
        }
        iVar4 = FUN_0802ecd0(iVar3);
        if (bVar13) {
          uVar8 = FUN_08019e48(param_1);
        }
        else if (bVar14) {
          if (((int)((uint)*(ushort *)(iVar3 + 0x2a) << 0x15) < 0) ||
             (uVar5 = FUN_0802fadc(iVar3,0), uVar5 != uVar7)) {
            uVar8 = uVar8 & 0x9fffffff;
            if (1000 < (int)uVar8) {
              uVar8 = 1000 - uVar8;
            }
            uVar8 = (int)(iVar4 * uVar8) / 100;
          }
          else {
            iVar6 = FUN_0802f864(param_1,0);
            iVar12 = FUN_0802f938(param_1,0);
            iVar9 = FUN_0802f962(param_1,0);
            uVar8 = iVar9 + iVar6 + iVar12 + iVar6;
          }
        }
        uVar2 = FUN_0802f9bc(param_1,0,2);
        uVar11 = FUN_0802f9bc(param_1,0,3);
        iVar4 = FUN_080279d6(uVar8,uVar2,uVar11,iVar4,uVar11);
      }
      local_2c = FUN_0802f0bc(param_1);
      bVar14 = false;
      if ((int)((uint)*(ushort *)(param_1 + 0x2a) << 0x16) < 0) {
        iVar6 = FUN_0802ed88(param_1);
      }
      else {
        uVar7 = FUN_0802f89c(param_1,0);
        bVar14 = uVar7 == DAT_08030c1c;
        if (((uVar7 & 0x7fffffff) >> 0x1d == 1) && ((int)(uVar7 & 0x9fffffff) < 0x7d1)) {
          bVar1 = true;
        }
        else {
          bVar1 = false;
        }
        iVar6 = FUN_0802ec9c(iVar3);
        if (bVar14) {
          uVar7 = FUN_08019d68(param_1);
        }
        else if (bVar1) {
          if (((int)((uint)*(ushort *)(iVar3 + 0x2a) << 0x16) < 0) ||
             (uVar8 = FUN_0802f89c(iVar3,0), uVar8 != DAT_08030c1c)) {
            uVar7 = uVar7 & 0x9fffffff;
            if (1000 < (int)uVar7) {
              uVar7 = 1000 - uVar7;
            }
            uVar7 = (int)(uVar7 * iVar6) / 100;
          }
          else {
            iVar12 = FUN_0802f864(param_1,0);
            iVar9 = FUN_0802f998(param_1,0);
            iVar10 = FUN_0802f902(param_1,0);
            uVar7 = iVar10 + iVar12 + iVar9 + iVar12;
          }
        }
        uVar2 = FUN_0802f9bc(param_1,0,5);
        uVar11 = FUN_0802f9bc(param_1,0,6);
        iVar6 = FUN_08027970(uVar7,uVar2,uVar11,iVar6,uVar11);
      }
      if ((bool)(bVar13 | bVar14)) {
        FUN_08031170(param_1,local_28,local_2c,0);
      }
      iVar12 = FUN_0802fc5e(param_1);
      if ((iVar12 == iVar4) && (iVar12 = FUN_0802ed88(param_1), iVar12 == iVar6)) {
        uVar2 = 0;
      }
      else {
        FUN_080302d0(param_1);
        FUN_0802ed04(param_1,auStack_3c);
        FUN_0802ec40(iVar3,auStack_4c);
        iVar12 = FUN_080174c4(auStack_3c,auStack_4c,0);
        if (iVar12 == 0) {
          FUN_08031248(iVar3);
        }
        *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x18) + iVar6 + -1;
        iVar6 = FUN_0802f7e0(param_1,0);
        if (iVar6 == 1) {
          *(int *)(param_1 + 0x14) = (*(int *)(param_1 + 0x1c) - iVar4) + 1;
        }
        else {
          *(int *)(param_1 + 0x1c) = *(int *)(param_1 + 0x14) + iVar4 + -1;
        }
        FUN_0802ad74(param_1,0x29,auStack_3c);
        FUN_0802ad74(iVar3,0x22,param_1);
        FUN_080302d0(param_1);
        FUN_080305d2(param_1,0);
        iVar4 = FUN_080174c4(param_1 + 0x14,auStack_4c,0);
        if ((iVar12 != 0) || (iVar4 != 0)) {
          FUN_08031248(iVar3);
        }
        FUN_08030c20(param_1);
        uVar2 = 1;
      }
    }
  }
  return uVar2;
}

