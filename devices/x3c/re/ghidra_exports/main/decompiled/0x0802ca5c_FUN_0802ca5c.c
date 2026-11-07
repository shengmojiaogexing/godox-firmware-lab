// 0x0802ca5c FUN_0802ca5c size=846

void FUN_0802ca5c(undefined4 param_1,undefined4 param_2)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int *piVar8;
  uint uVar9;
  byte bVar10;
  int local_74;
  int local_70;
  undefined1 auStack_6c [16];
  int local_5c;
  int local_58;
  int local_54;
  undefined2 local_4e;
  undefined2 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  int local_40;
  int local_3c;
  char local_36;
  byte local_35;
  int local_30;
  int local_2c;
  uint local_28;
  int local_24;
  int local_20;
  
  iVar5 = FUN_0802eb88(DAT_0802cb70,param_2);
  if (iVar5 == 1) {
    iVar5 = FUN_0802acf4(param_2);
    iVar6 = FUN_0802ad70(param_2);
    if (iVar5 == 0x2a) {
      FUN_0802d20c(iVar6);
      FUN_0802cd80(iVar6);
      return;
    }
    if (iVar5 == 0x13) {
      iVar5 = FUN_0802fa9a(iVar6,0);
      FUN_0802add2(param_2,(int)(*(int *)(iVar5 + 8) + ((uint)(*(int *)(iVar5 + 8) >> 0x1f) >> 0x1e)
                                ) >> 2);
      return;
    }
    if (iVar5 == 0x29) {
      FUN_0802d20c(iVar6);
      FUN_0802cd80(iVar6);
      return;
    }
    if (iVar5 == 0x2c) {
      uVar3 = FUN_0802fa9a(iVar6,0);
      uVar7 = FUN_0802faa6(iVar6,0);
      iVar5 = FUN_0802faac(iVar6,0);
      uVar9 = (uint)((int)((uint)*(byte *)(iVar6 + 0x54) << 0x1b) < 0);
      if ((int)((uint)*(byte *)(iVar6 + 0x54) << 0x1a) < 0) {
        uVar9 = uVar9 | 2;
      }
      FUN_0802ecd0(iVar6);
      iVar2 = FUN_0802faee(iVar6,0);
      if ((iVar2 == DAT_0802cb74) && (-1 < (int)((uint)*(ushort *)(iVar6 + 0x2a) << 0x15))) {
        local_2c = 0x1fffffff;
      }
      else {
        local_2c = FUN_0802ecd0(iVar6);
      }
      local_30 = iVar5;
      local_28 = uVar9;
      FUN_08033a84(&local_24,*(undefined4 *)(iVar6 + 0x2c),uVar3,uVar7);
      piVar8 = (int *)FUN_0802ad6c(param_2);
      if (local_24 < *piVar8) {
        local_24 = *piVar8;
      }
      *piVar8 = local_24;
      iVar5 = piVar8[1];
      if (piVar8[1] <= local_20) {
        iVar5 = local_20;
      }
      piVar8[1] = iVar5;
    }
    else if (iVar5 == 0x15) {
      iVar5 = FUN_0802ad70(param_2);
      iVar6 = FUN_0802ad00(param_2);
      FUN_0802ec40(iVar5,&local_30);
      bVar10 = (int)((uint)*(byte *)(iVar5 + 0x54) << 0x1b) < 0;
      if ((int)((uint)*(byte *)(iVar5 + 0x54) << 0x1a) < 0) {
        bVar10 = bVar10 | 2;
      }
      iVar2 = FUN_0802faee(iVar5,0);
      if ((iVar2 == DAT_0801e7a4) && (-1 < (int)((uint)*(ushort *)(iVar5 + 0x2a) << 0x15))) {
        bVar10 = bVar10 | 4;
      }
      FUN_08028d04(&local_5c);
      local_40 = *(undefined4 *)(iVar5 + 0x4c);
      local_3c = *(undefined4 *)(iVar5 + 0x50);
      local_35 = bVar10;
      FUN_0802fe78(iVar5,0,&local_5c);
      if (local_36 == '\0') {
        local_36 = '\x01';
      }
      local_58 = FUN_0802cd7a(iVar5);
      local_54 = FUN_0802cd76(iVar5);
      if ((local_58 != 0xffff) && (local_54 != 0xffff)) {
        uVar3 = FUN_0802f9bc(iVar5,0x40000,0x55);
        local_74 = FUN_0801812e(iVar5,0x40000,uVar3);
        local_70._0_2_ = (undefined2)local_74;
        local_4e = (undefined2)local_70;
        local_70 = local_74;
        local_74 = FUN_0802f9bc(iVar5,0x40000,0x20);
        local_70._0_2_ = (undefined2)local_74;
        local_4c = (undefined2)local_70;
        local_70 = local_74;
      }
      bVar1 = *(byte *)(iVar5 + 0x54) & 7;
      if ((((bVar1 == 2) || (bVar1 == 3)) && ((local_36 == '\x02' || (local_36 == '\x03')))) &&
         (FUN_08033a84(&local_74,*(undefined4 *)(iVar5 + 0x2c),local_5c,local_44,local_48,0x1fffffff
                       ,bVar10), (int)((local_28 - local_30) + 1) < local_74)) {
        local_36 = '\x01';
      }
      iVar2 = iVar5 + 0x38;
      if (((*(byte *)(iVar5 + 0x54) & 7) == 3) || ((local_24 - local_2c) + 1 < 0x400)) {
        iVar2 = 0;
      }
      iVar4 = FUN_08017484(auStack_6c,&local_30,*(undefined4 *)(iVar6 + 8));
      if (iVar4 != 0) {
        if ((*(byte *)(iVar5 + 0x54) & 7) == 0) {
          iVar4 = FUN_0802f0bc(iVar5);
          FUN_080274fe(&local_30,0,-iVar4);
          local_24 = *(int *)(iVar5 + 0x20);
        }
        bVar1 = *(byte *)(iVar5 + 0x54) & 7;
        if ((bVar1 == 2) || (bVar1 == 3)) {
          uVar3 = *(undefined4 *)(iVar6 + 8);
          *(undefined1 **)(iVar6 + 8) = auStack_6c;
          FUN_08028800(iVar6,&local_5c,&local_30,*(undefined4 *)(iVar5 + 0x2c),iVar2);
          *(undefined4 *)(iVar6 + 8) = uVar3;
        }
        else {
          FUN_08028800(iVar6,&local_5c,&local_30,*(undefined4 *)(iVar5 + 0x2c),iVar2);
        }
        uVar3 = *(undefined4 *)(iVar6 + 8);
        *(undefined1 **)(iVar6 + 8) = auStack_6c;
        if ((*(byte *)(iVar5 + 0x54) & 7) == 3) {
          FUN_08033a84(&local_74,*(undefined4 *)(iVar5 + 0x2c),local_5c,local_44,local_48,0x1fffffff
                       ,bVar10);
          if ((int)((local_28 - local_30) + 1) < local_74) {
            iVar4 = FUN_0802afc2(local_5c,0x20);
            local_40 = iVar4 * 3 + *(int *)(iVar5 + 0x4c) + local_74;
            local_3c = *(undefined4 *)(iVar5 + 0x50);
            FUN_08028800(iVar6,&local_5c,&local_30,*(undefined4 *)(iVar5 + 0x2c),iVar2);
          }
          if ((local_24 - local_2c) + 1 < local_70) {
            local_40 = *(undefined4 *)(iVar5 + 0x4c);
            local_3c = *(int *)(local_5c + 8) + *(int *)(iVar5 + 0x50) + local_70;
            FUN_08028800(iVar6,&local_5c,&local_30,*(undefined4 *)(iVar5 + 0x2c),iVar2);
          }
        }
        *(undefined4 *)(iVar6 + 8) = uVar3;
      }
      return;
    }
  }
  return;
}

