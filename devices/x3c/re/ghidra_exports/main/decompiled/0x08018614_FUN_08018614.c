// 0x08018614 FUN_08018614 size=706

void FUN_08018614(undefined4 param_1,byte *param_2)

{
  undefined4 *puVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  byte *pbVar10;
  uint uVar11;
  char cVar12;
  char cVar13;
  uint local_34;
  int local_30;
  undefined4 uStack_2c;
  byte *local_28;
  
  uStack_2c = param_1;
  local_28 = param_2;
  uVar3 = (*(code *)*DAT_080188d8)(param_1);
  iVar4 = (uVar3 + 1) * 4;
  iVar5 = FUN_0802d5c4(iVar4);
  iVar4 = FUN_0802d5c4(iVar4);
  puVar1 = DAT_080188dc;
  local_34 = 0;
  for (uVar11 = 0; uVar11 < uVar3; uVar11 = uVar11 + 1) {
    uVar6 = (*(code *)*puVar1)(param_1,&local_34);
    *(undefined4 *)(iVar5 + uVar11 * 4) = uVar6;
  }
  *(undefined4 *)(iVar5 + uVar11 * 4) = 0;
  iVar2 = DAT_080188e0;
  local_34 = 0;
  uVar11 = 0;
  iVar9 = -1;
  while (local_34 < uVar3) {
    iVar7 = FUN_08026b2c(*(undefined2 *)(iVar5 + local_34 * 4));
    local_30 = FUN_08026b2c(*(undefined2 *)(iVar5 + (local_34 + 1) * 4));
    iVar8 = FUN_08033bb8(*(undefined2 *)(iVar5 + local_34 * 4));
    if (iVar8 == 0) {
      iVar8 = FUN_08033bb8(*(undefined2 *)(iVar5 + (local_34 + 1) * 4));
      if (iVar8 != 0) {
        local_30 = FUN_08026b2c(*(undefined2 *)(iVar5 + (local_34 + 2) * 4));
      }
      if (iVar7 == -1) {
        *(undefined4 *)(iVar4 + uVar11 * 4) = *(undefined4 *)(iVar5 + local_34 * 4);
        uVar11 = uVar11 + 1;
        local_34 = local_34 + 1;
        iVar9 = -1;
      }
      else {
        if ((local_34 == 0) || (iVar9 == -1)) {
          cVar12 = '\0';
        }
        else {
          cVar12 = *(char *)(iVar2 + iVar9 * 10 + 8);
        }
        if ((local_34 == uVar3 - 1) || (local_30 == -1)) {
          cVar13 = '\0';
        }
        else {
          cVar13 = *(char *)(iVar2 + local_30 * 10 + 7);
        }
        iVar9 = FUN_08033bc8(iVar7,local_30);
        if (iVar9 == 0) {
          if ((cVar12 == '\0') || (cVar13 == '\0')) {
            if ((cVar12 == '\0') && (cVar13 != '\0')) {
              iVar9 = iVar2 + iVar7 * 10;
              *(uint *)(iVar4 + uVar11 * 4) =
                   (int)*(char *)(iVar9 + 4) + (uint)*(ushort *)(iVar9 + 2);
            }
            else if ((cVar12 == '\0') || (cVar13 != '\0')) {
              iVar9 = iVar2 + iVar7 * 10;
              *(uint *)(iVar4 + uVar11 * 4) =
                   (int)*(char *)(iVar9 + 6) + (uint)*(ushort *)(iVar9 + 2);
            }
            else {
              *(uint *)(iVar4 + uVar11 * 4) = (uint)*(ushort *)(iVar2 + iVar7 * 10 + 2);
            }
          }
          else {
            iVar9 = iVar2 + iVar7 * 10;
            *(uint *)(iVar4 + uVar11 * 4) = (int)*(char *)(iVar9 + 5) + (uint)*(ushort *)(iVar9 + 2)
            ;
          }
          local_34 = local_34 + 1;
          uVar11 = uVar11 + 1;
          iVar9 = iVar7;
        }
        else {
          if (cVar12 != '\0') {
            iVar9 = iVar9 + 1;
          }
          *(int *)(iVar4 + uVar11 * 4) = iVar9;
          local_34 = local_34 + 2;
          uVar11 = uVar11 + 1;
          iVar9 = -1;
        }
      }
    }
    else {
      *(undefined4 *)(iVar4 + uVar11 * 4) = *(undefined4 *)(iVar5 + local_34 * 4);
      local_34 = local_34 + 1;
      uVar11 = uVar11 + 1;
    }
  }
  *(undefined4 *)(iVar4 + uVar11 * 4) = 0;
  for (local_34 = 0; local_34 < uVar3; local_34 = local_34 + 1) {
    *(undefined4 *)(iVar5 + local_34 * 4) = 0;
  }
  for (local_34 = 0; local_34 < uVar11; local_34 = local_34 + 1) {
    *(undefined4 *)(iVar5 + local_34 * 4) = *(undefined4 *)(iVar4 + local_34 * 4);
  }
  FUN_0802d6e4(iVar4);
  pbVar10 = local_28;
  for (local_34 = 0; local_34 < uVar3; local_34 = local_34 + 1) {
    uVar11 = *(uint *)(iVar5 + local_34 * 4);
    if (uVar11 < 0x80) {
      *pbVar10 = (byte)uVar11;
      pbVar10 = pbVar10 + 1;
    }
    else if (uVar11 < 0x800) {
      *pbVar10 = (byte)((uVar11 & 0x7ff) >> 6) | 0xc0;
      pbVar10[1] = *(byte *)(iVar5 + local_34 * 4) & 0x3f | 0x80;
      pbVar10 = pbVar10 + 2;
    }
    else if (uVar11 < 0x10000) {
      *pbVar10 = (byte)((uVar11 & 0xffff) >> 0xc) | 0xe0;
      pbVar10[1] = (byte)((*(ushort *)(iVar5 + local_34 * 4) & 0xfff) >> 6) | 0x80;
      pbVar10[2] = *(byte *)(iVar5 + local_34 * 4) & 0x3f | 0x80;
      pbVar10 = pbVar10 + 3;
    }
    else if (uVar11 < 0x110000) {
      *pbVar10 = (byte)((uVar11 & 0x1fffff) >> 0x12) | 0xf0;
      pbVar10[1] = (byte)((*(uint *)(iVar5 + local_34 * 4) & 0x3ffff) >> 0xc) | 0x80;
      pbVar10[2] = (byte)((*(ushort *)(iVar5 + local_34 * 4) & 0xfff) >> 6) | 0x80;
      pbVar10[3] = *(byte *)(iVar5 + local_34 * 4) & 0x3f | 0x80;
      pbVar10 = pbVar10 + 4;
    }
  }
  *pbVar10 = 0;
  FUN_0802d6e4(iVar5);
  return;
}

