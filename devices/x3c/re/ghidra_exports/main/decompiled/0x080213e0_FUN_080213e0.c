// 0x080213e0 FUN_080213e0 size=552

int FUN_080213e0(int param_1,int param_2,int param_3,int param_4,int param_5,int *param_6)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  code *pcVar6;
  undefined4 local_40;
  int local_3c;
  code *local_38;
  int local_34;
  int iStack_30;
  int iStack_2c;
  int local_28;
  
  local_34 = param_1;
  iStack_30 = param_2;
  iStack_2c = param_3;
  local_28 = param_4;
  iVar2 = FUN_0802fae2(param_1,0);
  iVar3 = FUN_0802f8a2(local_34,0);
  bVar1 = *(byte *)(param_2 + 3);
  if (((int)((uint)bVar1 << 0x1e) < 0) &&
     ((((bVar1 & 1) != 0 && (iVar2 == DAT_08021608)) ||
      (((bVar1 & 1) == 0 && (iVar3 == DAT_08021608)))))) {
    *(byte *)(param_2 + 3) = bVar1 & 0xfd;
  }
  if ((*(byte *)(param_2 + 3) & 1) == 0) {
    pcVar6 = DAT_0802160c;
    local_38 = DAT_08021610;
  }
  else {
    pcVar6 = DAT_08021610;
    local_38 = DAT_0802160c;
  }
  param_6[1] = 0;
  param_6[2] = 0;
  param_6[5] = 0;
  *param_6 = 0;
  param_6[3] = 0;
  param_6[4] = 0;
  local_3c = param_3;
  iVar2 = FUN_0802ebc8(local_34,param_3);
  do {
    if ((iVar2 == 0) ||
       ((local_3c != param_3 && (iVar3 = FUN_0802fc9e(iVar2,0x800000), iVar3 != 0))))
    goto LAB_080215b4;
    iVar3 = FUN_0802fcac(iVar2,DAT_08021614);
    if (iVar3 == 0) {
      iVar3 = FUN_0802f888(iVar2,0);
      if (iVar3 == 0) {
        iVar3 = (*local_38)(iVar2);
        if (((int)((uint)*(byte *)(param_2 + 3) << 0x1e) < 0) && (local_28 < param_6[2] + iVar3)) {
LAB_080215b4:
          if (0 < param_6[2]) {
            param_6[2] = param_6[2] - param_5;
          }
          if (param_6[5] == 0) {
            iVar3 = param_6[2];
          }
          else {
            iVar3 = local_28;
          }
          param_6[1] = iVar3;
          if ((iVar2 != 0) && (local_3c == param_3)) {
            iVar2 = *(int *)(**(int **)(local_34 + 8) + local_3c * 4);
            FUN_080224dc(local_34,(*(byte *)(param_2 + 3) & 7) >> 2,&local_3c);
            if (iVar2 != 0) {
              iVar3 = (*pcVar6)(iVar2);
              *param_6 = iVar3;
              iVar2 = (*local_38)(iVar2);
              param_6[1] = iVar2;
              param_6[3] = 1;
            }
          }
          return local_3c;
        }
        param_6[2] = iVar3 + param_5 + param_6[2];
      }
      else {
        iVar4 = param_6[5];
        param_6[5] = iVar4 + 1;
        param_6[2] = param_6[2] + param_5;
        if ((*(byte *)(param_6 + 6) & 1) != 0) {
          iVar4 = FUN_0802d624((iVar4 + 1) * 0x18);
          if (iVar4 == 0) {
            do {
                    /* WARNING: Do nothing block with infinite loop */
            } while( true );
          }
          if (param_6[4] != 0) {
            FUN_0802d764(iVar4,param_6[4],(param_6[5] + -1) * 0x18);
            FUN_0802d6c0(param_6[4]);
          }
          *(int *)(iVar4 + (param_6[5] + -1) * 0x18) = iVar2;
          if ((*(byte *)(param_2 + 3) & 1) == 0) {
            uVar5 = FUN_0802f9bc(iVar2,0,5);
          }
          else {
            uVar5 = FUN_0802f9bc(iVar2,0,2);
          }
          *(undefined4 *)(iVar4 + (param_6[5] + -1) * 0x18 + 4) = uVar5;
          if ((*(byte *)(param_2 + 3) & 1) == 0) {
            local_40 = FUN_0802f9bc(iVar2,0,6);
          }
          else {
            local_40 = FUN_0802f9bc(iVar2,0,3);
          }
          *(undefined4 *)(iVar4 + (param_6[5] + -1) * 0x18 + 8) = local_40;
          *(int *)(iVar4 + (param_6[5] + -1) * 0x18 + 0x10) = iVar3;
          *(undefined4 *)(iVar4 + (param_6[5] + -1) * 0x18 + 0x14) = 0;
          param_6[4] = iVar4;
        }
      }
      iVar3 = (*pcVar6)(iVar2);
      iVar4 = *param_6;
      if (iVar4 < iVar3) {
        iVar4 = (*pcVar6)(iVar2);
      }
      *param_6 = iVar4;
      param_6[3] = param_6[3] + 1;
    }
    if ((int)((uint)*(byte *)(param_2 + 3) << 0x1d) < 0) {
      iVar3 = -1;
    }
    else {
      iVar3 = 1;
    }
    local_3c = local_3c + iVar3;
    if (local_3c < 0) goto LAB_080215b4;
    iVar2 = FUN_0802ebc8(local_34);
  } while( true );
}

