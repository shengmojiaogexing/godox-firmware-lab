// 0x08032dcc FUN_08032dcc size=212

undefined4 FUN_08032dcc(int *param_1,uint param_2)

{
  byte bVar1;
  undefined2 uVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  
  if (*(ushort *)(param_1 + 1) == 0xffff) {
    uVar5 = 0;
  }
  else {
    uVar6 = (uint)*(byte *)((int)param_1 + 7);
    if (uVar6 == 0) {
      uVar5 = 0;
    }
    else if (uVar6 == 1) {
      if ((*(ushort *)(param_1 + 1) & 0xffff3fff) == param_2) {
        *(undefined2 *)(param_1 + 1) = 0;
        *(undefined1 *)((int)param_1 + 7) = 0;
        uVar5 = 1;
      }
      else {
        uVar5 = 0;
      }
    }
    else {
      piVar10 = (int *)*param_1;
      piVar9 = piVar10 + uVar6;
      for (uVar3 = 0; uVar3 < uVar6; uVar3 = uVar3 + 1) {
        if ((*(ushort *)((int)piVar9 + uVar3 * 2) & 0xffff3fff) == param_2) {
          if (uVar6 == 2) {
            *(undefined1 *)((int)param_1 + 7) = 1;
            if (uVar3 == 0) {
              uVar2 = *(undefined2 *)((int)piVar9 + 2);
            }
            else {
              uVar2 = (undefined2)*piVar9;
            }
            *(undefined2 *)(param_1 + 1) = uVar2;
            if (uVar3 == 0) {
              *param_1 = piVar10[1];
            }
            else {
              *param_1 = *piVar10;
            }
          }
          else {
            iVar4 = FUN_0802d5c4((uVar6 - 1) * 6);
            if (iVar4 == 0) {
              return 0;
            }
            *param_1 = iVar4;
            bVar1 = *(char *)((int)param_1 + 7) - 1;
            *(byte *)((int)param_1 + 7) = bVar1;
            iVar8 = 0;
            for (uVar6 = 0; uVar6 <= *(byte *)((int)param_1 + 7); uVar6 = uVar6 + 1) {
              iVar7 = iVar8;
              if (*(ushort *)((int)piVar9 + uVar6 * 2) != param_2) {
                *(int *)(iVar4 + iVar8 * 4) = piVar10[uVar6];
                iVar7 = iVar8 + 1;
                *(undefined2 *)(iVar4 + (uint)bVar1 * 4 + iVar8 * 2) =
                     *(undefined2 *)((int)piVar9 + uVar6 * 2);
              }
              iVar8 = iVar7;
            }
          }
          FUN_0802d6e4(piVar10);
          return 1;
        }
      }
      uVar5 = 0;
    }
  }
  return uVar5;
}

