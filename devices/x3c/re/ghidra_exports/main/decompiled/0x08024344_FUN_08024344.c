// 0x08024344 FUN_08024344 size=946

void FUN_08024344(int *param_1,int param_2)

{
  byte bVar1;
  char cVar2;
  char cVar3;
  bool bVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  
  bVar1 = *(byte *)((int)param_1 + 5);
  if ((*(char *)(param_2 + 0x12) == '\x01') && ((int)((uint)bVar1 << 0x1c) < 0)) {
    return;
  }
  if ((int)((uint)bVar1 << 0x1c) < 0) {
    param_1[0x1d] = 0;
    *(byte *)((int)param_1 + 5) = bVar1 & 0xf6;
    *(undefined1 *)(param_1 + 2) = 0;
  }
  cVar3 = (char)param_1[2];
  *(undefined1 *)(param_1 + 2) = *(undefined1 *)(param_2 + 0x12);
  param_1[3] = *(int *)(param_2 + 8);
  iVar9 = param_1[0x20];
  if (iVar9 == 0) {
    return;
  }
  iVar5 = FUN_0802b754(iVar9);
  iVar8 = DAT_080246f8;
  *(int *)(DAT_080246f8 + 4) = iVar5;
  if (iVar5 == 0) {
    return;
  }
  if (*(char *)(param_2 + 0x12) != '\0') {
    *(undefined2 *)(param_2 + 0x10) = 0;
  }
  iVar5 = FUN_0802b754(iVar9);
  *(int *)(iVar8 + 4) = iVar5;
  if (iVar5 == 0) {
    return;
  }
  cVar2 = *(char *)(param_2 + 0x12);
  if ((cVar2 == '\x01') && (cVar3 == '\0')) {
    iVar5 = FUN_080334d8();
    param_1[0x1d] = iVar5;
    iVar5 = *(int *)(param_2 + 8);
    if (iVar5 == 10) {
      iVar5 = FUN_0803033e(*(undefined4 *)(iVar8 + 4));
      if ((iVar5 == 0) && (iVar5 = FUN_0802fc9e(*(undefined4 *)(iVar8 + 4),0x10), iVar5 == 0)) {
        uVar7 = 0;
      }
      else {
        uVar7 = 1;
      }
      uVar6 = FUN_0802b744(iVar9);
      if ((uVar7 & ~uVar6) != 0) goto LAB_08024642;
      FUN_0802ad74(*(undefined4 *)(iVar8 + 4),1,*(undefined4 *)(iVar8 + 8));
      iVar5 = FUN_08024ea0(param_1 + 1);
    }
    else {
      if (iVar5 == 0x14) {
        *(short *)(param_2 + 0x10) = *(short *)(param_2 + 0x10) + -1;
        goto LAB_08024642;
      }
      if (iVar5 == 0x13) {
        *(short *)(param_2 + 0x10) = *(short *)(param_2 + 0x10) + 1;
        goto LAB_08024642;
      }
      if (iVar5 == 0x1b) {
        FUN_0802b88a(iVar9,0x1b);
        iVar5 = FUN_08024ea0(param_1 + 1);
        if (iVar5 != 0) {
          return;
        }
        FUN_0802ad74(*(undefined4 *)(iVar8 + 4),0x20,*(undefined4 *)(iVar8 + 8));
        iVar5 = FUN_08024ea0(param_1 + 1);
      }
      else {
        FUN_0802b88a(iVar9);
        iVar5 = FUN_08024ea0(param_1 + 1);
      }
    }
joined_r0x08024520:
    if (iVar5 != 0) {
      return;
    }
    goto LAB_08024642;
  }
  if ((cVar2 != '\x01') || (cVar3 != '\x01')) {
    if ((cVar2 != '\0') || (cVar3 != '\x01')) goto LAB_08024642;
    if (*(int *)(param_2 + 8) == 10) {
      iVar5 = FUN_0803033e();
      if (iVar5 == 0) {
        iVar5 = FUN_0802fc9e(*(undefined4 *)(iVar8 + 4),0x10);
        bVar4 = false;
        if (iVar5 != 0) goto LAB_0802456a;
      }
      else {
LAB_0802456a:
        bVar4 = true;
      }
      if (bVar4) {
        iVar5 = FUN_0802b744(iVar9);
        if (iVar5 == 0) {
          if ((*(byte *)((int)param_1 + 5) & 1) == 0) {
            FUN_0802b8b2(iVar9,1);
          }
        }
        else {
          if (((*(byte *)((int)param_1 + 5) & 1) == 0) ||
             (uVar7 = thunk_FUN_08017f3c(iVar9), uVar7 < 2)) {
            FUN_0802ad74(*(undefined4 *)(iVar8 + 4),8,*(undefined4 *)(iVar8 + 8));
            iVar5 = FUN_08024ea0(param_1 + 1);
            if (iVar5 != 0) {
              return;
            }
            FUN_0802ad74(*(undefined4 *)(iVar8 + 4),4,*(undefined4 *)(iVar8 + 8));
            iVar5 = FUN_08024ea0(param_1 + 1);
            if (iVar5 != 0) {
              return;
            }
            FUN_0802ad74(*(undefined4 *)(iVar8 + 4),7,*(undefined4 *)(iVar8 + 8));
            iVar5 = FUN_08024ea0(param_1 + 1);
            if (iVar5 != 0) {
              return;
            }
            FUN_0802b88a(iVar9,10);
            iVar5 = FUN_08024ea0(param_1 + 1);
            goto joined_r0x0802461a;
          }
          FUN_0802e34c(*(undefined4 *)(iVar8 + 4),0x20);
        }
      }
      else {
        FUN_0802ad74(*(undefined4 *)(iVar8 + 4),8,*(undefined4 *)(iVar8 + 8));
        iVar5 = FUN_08024ea0(param_1 + 1);
        if (iVar5 != 0) {
          return;
        }
        if ((*(byte *)((int)param_1 + 5) & 1) == 0) {
          FUN_0802ad74(*(undefined4 *)(iVar8 + 4),4,*(undefined4 *)(iVar8 + 8));
        }
        iVar5 = FUN_08024ea0(param_1 + 1);
        if (iVar5 != 0) {
          return;
        }
        FUN_0802ad74(*(undefined4 *)(iVar8 + 4),7,*(undefined4 *)(iVar8 + 8));
        iVar5 = FUN_08024ea0(param_1 + 1);
joined_r0x0802461a:
        if (iVar5 != 0) {
          return;
        }
      }
    }
    param_1[0x1d] = 0;
    *(byte *)((int)param_1 + 5) = *(byte *)((int)param_1 + 5) & 0xfe;
    goto LAB_08024642;
  }
  if (((*(byte *)((int)param_1 + 5) & 1) != 0) ||
     (uVar7 = FUN_080334c0(param_1[0x1d]), uVar7 <= *(ushort *)(*param_1 + 0x1c))) {
    if (((*(byte *)((int)param_1 + 5) & 1) == 0) ||
       (uVar7 = FUN_080334c0(param_1[0x1e]), uVar7 <= *(ushort *)(*param_1 + 0x1e)))
    goto LAB_08024642;
    iVar5 = FUN_080334d8();
    param_1[0x1e] = iVar5;
    iVar5 = *(int *)(param_2 + 8);
    if (iVar5 == 10) {
      FUN_0802ad74(*(undefined4 *)(iVar8 + 4),6,*(undefined4 *)(iVar8 + 8));
      iVar5 = FUN_08024ea0(param_1 + 1);
    }
    else {
      if (iVar5 == 0x14) {
        *(short *)(param_2 + 0x10) = *(short *)(param_2 + 0x10) + -1;
        goto LAB_08024642;
      }
      if (iVar5 == 0x13) {
        *(short *)(param_2 + 0x10) = *(short *)(param_2 + 0x10) + 1;
        goto LAB_08024642;
      }
      FUN_0802b88a(iVar9);
      iVar5 = FUN_08024ea0(param_1 + 1);
    }
    goto joined_r0x08024520;
  }
  *(byte *)((int)param_1 + 5) = *(byte *)((int)param_1 + 5) | 1;
  iVar5 = FUN_080334d8();
  param_1[0x1e] = iVar5;
  if (*(int *)(param_2 + 8) == 10) {
    iVar5 = FUN_0803033e(*(undefined4 *)(iVar8 + 4));
    if (iVar5 == 0) {
      iVar5 = FUN_0802fc9e(*(undefined4 *)(iVar8 + 4),0x10);
      bVar4 = false;
      if (iVar5 != 0) goto LAB_080244a4;
    }
    else {
LAB_080244a4:
      bVar4 = true;
    }
    if (bVar4) {
      uVar7 = thunk_FUN_08017f3c(iVar9);
      if (1 < uVar7) {
        uVar7 = FUN_0802b744(iVar9);
        FUN_0802b8b2(iVar9,uVar7 ^ 1);
        FUN_0802e34c(*(undefined4 *)(iVar8 + 4),0x20);
      }
    }
    else {
      FUN_0802ad74(*(undefined4 *)(iVar8 + 4),5,*(undefined4 *)(iVar8 + 8));
      iVar5 = FUN_08024ea0(param_1 + 1);
      if (iVar5 != 0) {
        return;
      }
    }
  }
  *(byte *)((int)param_1 + 5) = *(byte *)((int)param_1 + 5) | 1;
LAB_08024642:
  *(undefined4 *)(iVar8 + 4) = 0;
  if (*(short *)(param_2 + 0x10) != 0) {
    iVar8 = FUN_0802b744(iVar9);
    if (iVar8 == 0) {
      if (*(short *)(param_2 + 0x10) < 0) {
        for (iVar8 = 0;
            -iVar8 != (int)*(short *)(param_2 + 0x10) && iVar8 <= -(int)*(short *)(param_2 + 0x10);
            iVar8 = iVar8 + 1) {
          FUN_0802b70c(iVar9);
          iVar5 = FUN_08024ea0(param_1 + 1);
          if (iVar5 != 0) {
            return;
          }
        }
      }
      else if (0 < *(short *)(param_2 + 0x10)) {
        for (iVar8 = 0; iVar8 < *(short *)(param_2 + 0x10); iVar8 = iVar8 + 1) {
          FUN_0802b644(iVar9);
          iVar5 = FUN_08024ea0(param_1 + 1);
          if (iVar5 != 0) {
            return;
          }
        }
      }
    }
    else if (*(short *)(param_2 + 0x10) < 0) {
      for (iVar8 = 0;
          -iVar8 != (int)*(short *)(param_2 + 0x10) && iVar8 <= -(int)*(short *)(param_2 + 0x10);
          iVar8 = iVar8 + 1) {
        FUN_0802b88a(iVar9,0x14);
        iVar5 = FUN_08024ea0(param_1 + 1);
        if (iVar5 != 0) {
          return;
        }
      }
    }
    else if (0 < *(short *)(param_2 + 0x10)) {
      for (iVar8 = 0; iVar8 < *(short *)(param_2 + 0x10); iVar8 = iVar8 + 1) {
        FUN_0802b88a(iVar9,0x13);
        iVar5 = FUN_08024ea0(param_1 + 1);
        if (iVar5 != 0) {
          return;
        }
      }
    }
  }
  return;
}

