// 0x0802da8e FUN_0802da8e size=14

void FUN_0802da8e(int param_1,uint param_2)

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
  int *piStack_44;
  int iStack_40;
  uint uStack_3c;
  int iStack_38;
  uint uStack_34;
  int iStack_30;
  int iStack_2c;
  uint uStack_28;
  
  uStack_28 = param_2 | *(ushort *)(param_1 + 0x28);
  if (*(ushort *)(param_1 + 0x28) == uStack_28) {
    return;
  }
  uStack_34 = (uint)*(ushort *)(param_1 + 0x28);
  if (uStack_34 != uStack_28) {
    *(short *)(param_1 + 0x28) = (short)uStack_28;
    iStack_2c = param_1;
    iStack_38 = FUN_080182cc(param_1,uStack_34,uStack_28);
    if (iStack_38 != 0) {
      iVar2 = FUN_0802d624(0x280);
      FUN_0802d8d6(iVar2,0x280);
      uVar6 = 0;
      uVar9 = 0;
      while ((uVar9 < (*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3 && (uVar6 < 0x20))) {
        puVar10 = (undefined4 *)(*(int *)(param_1 + 0xc) + uVar9 * 8);
        iVar8 = uVar9 * 8 + 4;
        uStack_3c = FUN_08031788(*(uint *)(*(int *)(param_1 + 0xc) + iVar8) & 0xffffff);
        iStack_40 = FUN_08031782(*(uint *)(*(int *)(param_1 + 0xc) + iVar8) & 0xffffff);
        if (((uStack_3c & ~uStack_28) == 0) &&
           ((-1 < (int)(puVar10[1] << 6) &&
            (iVar8 = FUN_08032ad0(*puVar10,0x66,&piStack_44), piVar1 = piStack_44, iVar8 == 1)))) {
          iVar8 = 0;
          while ((*(short *)(*piVar1 + iVar8 * 2) != 0 && (uVar6 < 0x20))) {
            for (uVar7 = 0; uVar7 < uVar6; uVar7 = uVar7 + 1) {
              iStack_30 = iVar2 + uVar7 * 0x14;
              uVar5 = *(undefined4 *)(iStack_30 + 4);
              uVar3 = FUN_08031788(uVar5);
              iVar4 = FUN_08031782(uVar5);
              if (((*(short *)(*piVar1 + iVar8 * 2) == *(short *)(iStack_30 + 8)) &&
                  (iVar4 == iStack_40)) && (uStack_3c <= uVar3)) break;
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
        FUN_08018180(param_1,uVar5,uStack_34,uStack_28,iVar8);
      }
      FUN_0802d6c0(iVar2);
      if (iStack_38 == 1) {
        FUN_080302d0(param_1);
        return;
      }
      if (iStack_38 == 3) {
        FUN_08030ca4(param_1,0xf0000,0xffff);
        return;
      }
      if (iStack_38 == 2) {
        FUN_080302d0(param_1);
        FUN_08030c20(param_1);
        return;
      }
    }
  }
  return;
}

