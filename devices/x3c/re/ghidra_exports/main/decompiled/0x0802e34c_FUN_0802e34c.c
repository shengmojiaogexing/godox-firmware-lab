// 0x0802e34c FUN_0802e34c size=440

void FUN_0802e34c(int param_1,uint param_2)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  undefined4 *puVar10;
  int *local_44;
  int local_40;
  uint local_3c;
  int local_38;
  uint local_34;
  int local_30;
  int iStack_2c;
  uint local_28;
  
  local_28 = (uint)*(ushort *)(param_1 + 0x28) & ~param_2;
  if (*(ushort *)(param_1 + 0x28) == local_28) {
    return;
  }
  local_34 = (uint)*(ushort *)(param_1 + 0x28);
  if (local_34 != local_28) {
    *(short *)(param_1 + 0x28) = (short)local_28;
    iStack_2c = param_1;
    local_38 = FUN_080182cc(param_1,local_34,local_28);
    if (local_38 != 0) {
      iVar2 = FUN_0802d624(0x280);
      FUN_0802d8d6(iVar2,0x280);
      uVar6 = 0;
      uVar9 = 0;
      while ((uVar9 < (*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3 && (uVar6 < 0x20))) {
        puVar10 = (undefined4 *)(*(int *)(param_1 + 0xc) + uVar9 * 8);
        iVar8 = uVar9 * 8 + 4;
        local_3c = FUN_08031788(*(uint *)(*(int *)(param_1 + 0xc) + iVar8) & 0xffffff);
        local_40 = FUN_08031782(*(uint *)(*(int *)(param_1 + 0xc) + iVar8) & 0xffffff);
        if (((local_3c & ~local_28) == 0) &&
           ((-1 < (int)(puVar10[1] << 6) &&
            (iVar8 = FUN_08032ad0(*puVar10,0x66,&local_44), piVar1 = local_44, iVar8 == 1)))) {
          iVar8 = 0;
          while ((*(short *)(*piVar1 + iVar8 * 2) != 0 && (uVar6 < 0x20))) {
            for (uVar7 = 0; uVar7 < uVar6; uVar7 = uVar7 + 1) {
              local_30 = iVar2 + uVar7 * 0x14;
              uVar5 = *(undefined4 *)(local_30 + 4);
              uVar3 = FUN_08031788(uVar5);
              iVar4 = FUN_08031782(uVar5);
              if (((*(short *)(*piVar1 + iVar8 * 2) == *(short *)(local_30 + 8)) &&
                  (iVar4 == local_40)) && (local_3c <= uVar3)) break;
            }
            if (uVar7 == uVar6) {
              *(short *)(iVar2 + uVar6 * 0x14) = (short)piVar1[3];
              iVar4 = iVar2 + uVar6 * 0x14;
              *(short *)(iVar4 + 2) = (short)piVar1[4];
              *(int *)(iVar4 + 0xc) = piVar1[2];
              *(undefined2 *)(iVar4 + 8) = *(undefined2 *)(*piVar1 + iVar8 * 2);
              *(int *)(iVar4 + 0x10) = piVar1[1];
              *(uint *)(iVar4 + 4) = puVar10[1] & 0xffffff;
              uVar6 = uVar6 + 1;
            }
            iVar8 = iVar8 + 1;
          }
        }
        uVar9 = uVar9 + 1;
      }
      for (uVar9 = 0; uVar9 < uVar6; uVar9 = uVar9 + 1) {
        iVar8 = iVar2 + uVar9 * 0x14;
        uVar5 = FUN_08031782(*(undefined4 *)(iVar8 + 4));
        FUN_08018180(param_1,uVar5,local_34,local_28,iVar8);
      }
      FUN_0802d6c0(iVar2);
      if (local_38 == 1) {
        FUN_080302d0(param_1);
        return;
      }
      if (local_38 == 3) {
        FUN_08030ca4(param_1,0xf0000,0xffff);
        return;
      }
      if (local_38 == 2) {
        FUN_080302d0(param_1);
        FUN_08030c20(param_1);
        return;
      }
    }
  }
  return;
}

