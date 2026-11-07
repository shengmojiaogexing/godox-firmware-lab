// 0x08039cb0 FUN_08039cb0 size=372

void FUN_08039cb0(int *param_1,uint param_2)

{
  ushort uVar1;
  bool bVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  undefined2 local_28;
  undefined2 local_24;
  ushort local_20;
  uint local_1c;
  uint local_18;
  
  iVar6 = *param_1;
  uVar5 = 0;
  while( true ) {
    if ((*(ushort *)(iVar6 + 0x2a) & 0x1ff) >> 3 <= uVar5) {
      return;
    }
    uVar3 = *(uint *)(*(int *)(iVar6 + 0xc) + uVar5 * 8 + 4);
    if (((int)(uVar3 << 6) < 0) && ((uVar3 & 0xffffff) == param_1[2])) break;
    uVar5 = uVar5 + 1;
  }
  uVar1 = *(ushort *)(param_1 + 1);
  if (uVar1 == 0x47) goto LAB_08039dbc;
  if (0x47 < uVar1) {
    if (uVar1 == 0x61) {
      local_18 = param_1[3];
      if (local_18 == 0) {
        local_18 = param_1[4];
      }
      else if (param_1[4] != 0) {
        if ((int)param_2 < 0x80) {
        }
        else {
          local_18 = param_1[4];
        }
      }
      goto LAB_08039d38;
    }
    if (uVar1 < 0x62) {
      if (uVar1 == 0x55) goto LAB_08039dbc;
      if (uVar1 == 0x57) goto LAB_08039d8e;
    }
    else {
      if (uVar1 == 0x66) {
LAB_08039d8e:
        if ((int)param_2 < 0xff) {
          local_18 = param_1[3];
        }
        else {
          local_18 = param_1[4];
        }
        goto LAB_08039d38;
      }
      if (uVar1 == 0x67) goto LAB_08039d7c;
    }
    goto LAB_08039d24;
  }
  if (uVar1 == 0x34) {
LAB_08039d7c:
    if ((int)param_2 < 0xff) {
      local_18 = param_1[3];
    }
    else {
      local_18 = param_1[4];
    }
    goto LAB_08039d38;
  }
  if (uVar1 < 0x35) {
    if ((uVar1 != 0x20) && (uVar1 != 0x30)) {
      if (uVar1 == 0x33) goto LAB_08039d7c;
      goto LAB_08039d24;
    }
  }
  else if ((uVar1 != 0x36) && (uVar1 != 0x44)) {
LAB_08039d24:
    if (param_2 == 0) {
      local_18 = param_1[3];
    }
    else if (param_2 == 0xff) {
      local_18 = param_1[4];
    }
    else {
      local_18 = param_1[3] + ((int)(param_2 * (param_1[4] - param_1[3])) >> 8);
    }
    goto LAB_08039d38;
  }
LAB_08039dbc:
  if ((int)param_2 < 1) {
    local_18 = CONCAT22(local_18._2_2_,(short)param_1[3]);
  }
  else if ((int)param_2 < 0xff) {
    local_24 = (undefined2)param_1[3];
    uVar3 = CONCAT22(local_24,local_24) & DAT_08039e28;
    local_28 = (undefined2)param_1[4];
    uVar3 = uVar3 + (((param_2 & 0xff) + 4 >> 3) *
                     ((CONCAT22(local_28,local_28) & DAT_08039e28) - uVar3) >> 5) & DAT_08039e28;
    local_20 = (ushort)uVar3 | (ushort)(uVar3 >> 0x10);
    local_18 = CONCAT22(local_18._2_2_,local_20);
  }
  else {
    local_18 = CONCAT22(local_18._2_2_,(short)param_1[4]);
  }
LAB_08039d38:
  bVar2 = true;
  iVar4 = thunk_FUN_08032b92(*(undefined4 *)(*(int *)(iVar6 + 0xc) + uVar5 * 8),uVar1,&local_1c);
  if (((iVar4 != 0) && (local_18 == local_1c)) && ((local_18 & 0xffff) == (local_1c & 0xffff))) {
    bVar2 = false;
  }
  FUN_08032ff8(*(undefined4 *)(*(int *)(iVar6 + 0xc) + uVar5 * 8),(short)param_1[1],local_18);
  if (!bVar2) {
    return;
  }
  FUN_08030ca4(*param_1,param_1[2],(short)param_1[1]);
  return;
}

