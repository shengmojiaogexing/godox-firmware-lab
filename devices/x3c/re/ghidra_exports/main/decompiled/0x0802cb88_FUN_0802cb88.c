// 0x0802cb88 FUN_0802cb88 size=468

int FUN_0802cb88(int param_1,int *param_2)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  uint local_5c;
  uint local_58;
  int local_54 [3];
  int local_48;
  int local_44;
  int local_40 [2];
  int local_38;
  int local_30;
  int local_2c;
  int local_28;
  
  if (param_2 == (int *)0x0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  local_58 = FUN_0802f9bc(param_1,0,0x12);
  local_30 = *param_2 - local_58;
  local_58 = FUN_0802f9bc(param_1,0,0x10);
  local_2c = param_2[1] - local_58;
  FUN_0802ec40(param_1,local_40);
  iVar2 = FUN_0802cd72(param_1);
  local_44 = (local_38 - local_40[0]) + 1;
  local_48 = FUN_0802fa9a(param_1,0);
  local_54[2] = FUN_0802faac(param_1,0);
  iVar3 = FUN_0802faa6(param_1,0);
  iVar10 = *(int *)(local_48 + 8);
  iVar9 = 0;
  uVar8 = (uint)((int)((uint)*(byte *)(param_1 + 0x54) << 0x1b) < 0);
  if ((int)((uint)*(byte *)(param_1 + 0x54) << 0x1a) < 0) {
    uVar8 = uVar8 | 2;
  }
  iVar4 = FUN_0802faee(param_1,0);
  if ((iVar4 == DAT_0802cd5c) && (-1 < (int)((uint)*(ushort *)(param_1 + 0x2a) << 0x15))) {
    uVar8 = uVar8 | 4;
  }
  iVar4 = FUN_0802e0f8(param_1,0,*(undefined4 *)(param_1 + 0x2c));
  local_28 = local_54[2] + iVar10;
  uVar7 = 0;
  while (uVar6 = uVar7, *(char *)(iVar2 + uVar7) != '\0') {
    local_5c = uVar8;
    iVar5 = FUN_08018918(iVar2 + uVar7,local_48,iVar3,local_44,0);
    uVar6 = uVar7 + iVar5;
    if (local_2c <= iVar9 + iVar10) {
      local_58 = uVar6;
      iVar9 = (*(code *)*DAT_0802cd60)(iVar2,&local_58);
      if ((iVar9 != 10) && (*(char *)(iVar2 + uVar6) == '\0')) {
        uVar6 = uVar6 + 1;
      }
      break;
    }
    iVar9 = iVar9 + local_28;
    uVar7 = uVar6;
  }
  iVar10 = iVar2 + uVar7;
  iVar9 = 0;
  if (iVar4 == 2) {
    iVar9 = FUN_08033b40(iVar10,uVar6 - uVar7,local_48,iVar3,uVar8);
    iVar9 = ((local_38 - local_40[0]) + 1) / 2 - iVar9 / 2;
  }
  else if (iVar4 == 3) {
    iVar9 = FUN_08033b40(iVar10,uVar6 - uVar7,local_48,iVar3,uVar8);
    iVar9 = ((local_38 - local_40[0]) + 1) - iVar9;
  }
  local_54[1] = 0;
  local_54[0] = 0;
  iVar4 = 0;
  if (uVar6 != 0) {
    while (local_54[0] + uVar7 < uVar6) {
      FUN_080188e4(iVar10,&local_58,&local_5c,local_54);
      if (((uVar8 & 1) == 0) ||
         (iVar5 = FUN_08018a20(local_54 + 1,*(undefined1 *)(iVar10 + local_54[0])), iVar5 == 0)) {
        iVar5 = FUN_0802afc2(local_48,local_58,local_5c);
        if ((local_30 < iVar5 + iVar9) ||
           ((local_54[0] + uVar7 == uVar6 || (*(char *)(iVar2 + iVar4 + uVar7) == '\0')))) {
          local_54[0] = iVar4;
          break;
        }
        iVar9 = iVar5 + iVar9 + iVar3;
        iVar4 = local_54[0];
      }
    }
  }
  puVar1 = DAT_0802cd64;
  iVar3 = (*(code *)*DAT_0802cd64)(iVar10,local_54[0]);
  iVar2 = (*(code *)*puVar1)(iVar2,uVar7);
  return iVar2 + iVar3;
}

