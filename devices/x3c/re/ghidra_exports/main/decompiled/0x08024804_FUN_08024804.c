// 0x08024804 FUN_08024804 size=618

/* WARNING: Type propagation algorithm not settling */

void FUN_08024804(int *param_1,int param_2)

{
  byte bVar1;
  char cVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  
  bVar1 = *(byte *)((int)param_1 + 5);
  if ((*(char *)(param_2 + 0x12) == '\x01') && ((int)((uint)bVar1 << 0x1c) < 0)) {
    return;
  }
  if ((int)((uint)bVar1 << 0x1c) < 0) {
    param_1[0x1d] = 0;
    *(byte *)((int)param_1 + 5) = bVar1 & 0xf6;
    *(undefined1 *)(param_1 + 2) = 0;
  }
  iVar8 = param_1[0x20];
  if (iVar8 == 0) {
    return;
  }
  iVar5 = FUN_0802b754(iVar8);
  iVar4 = DAT_08024a70;
  *(int *)(DAT_08024a70 + 4) = iVar5;
  if (iVar5 == 0) {
    return;
  }
  iVar5 = FUN_0802fcb6(iVar5,0x80);
  iVar7 = param_1[3];
  param_1[3] = *(int *)(param_2 + 8);
  cVar2 = (char)param_1[2];
  *(undefined1 *)(param_1 + 2) = *(undefined1 *)(param_2 + 0x12);
  cVar3 = *(char *)(param_2 + 0x12);
  if ((cVar3 == '\x01') && (cVar2 == '\0')) {
    iVar7 = FUN_080334d8();
    param_1[0x1d] = iVar7;
    iVar7 = *(int *)(param_2 + 8);
    if (iVar7 == 9) {
      FUN_0802b8b2(iVar8,0);
      FUN_0802b644(iVar8);
      iVar8 = FUN_08024ea0(param_1 + 1);
    }
    else if (iVar7 == 0xb) {
      FUN_0802b8b2(iVar8,0);
      FUN_0802b70c(iVar8);
      iVar8 = FUN_08024ea0(param_1 + 1);
    }
    else {
      if (iVar5 != 0) goto LAB_08024a68;
      if (iVar7 == 10) {
        FUN_0802b88a(iVar8,10);
        iVar8 = FUN_08024ea0(param_1 + 1);
        if (iVar8 != 0) {
          return;
        }
        FUN_0802ad74(*(undefined4 *)(iVar4 + 4),1,*(undefined4 *)(iVar4 + 8));
        iVar8 = FUN_08024ea0(param_1 + 1);
      }
      else if (iVar7 == 0x1b) {
        FUN_0802b88a(iVar8,0x1b);
        iVar8 = FUN_08024ea0(param_1 + 1);
        if (iVar8 != 0) {
          return;
        }
        FUN_0802ad74(*(undefined4 *)(iVar4 + 4),0x20,*(undefined4 *)(iVar4 + 8));
        iVar8 = FUN_08024ea0(param_1 + 1);
      }
      else {
        FUN_0802b88a(iVar8);
        iVar8 = FUN_08024ea0(param_1 + 1);
      }
    }
  }
  else {
    if (((iVar5 != 0) || (cVar3 != '\x01')) || (cVar2 != '\x01')) {
      if (((iVar5 == 0) && (cVar3 == '\0')) && (cVar2 == '\x01')) {
        *(int *)(param_2 + 8) = iVar7;
        if (iVar7 == 10) {
          FUN_0802ad74(*(undefined4 *)(iVar4 + 4),8,*(undefined4 *)(iVar4 + 8));
          iVar8 = FUN_08024ea0(param_1 + 1);
          if (iVar8 != 0) {
            return;
          }
          if ((*(byte *)((int)param_1 + 5) & 1) == 0) {
            FUN_0802ad74(*(undefined4 *)(iVar4 + 4),4,*(undefined4 *)(iVar4 + 8));
            iVar8 = FUN_08024ea0(param_1 + 1);
            if (iVar8 != 0) {
              return;
            }
          }
          FUN_0802ad74(*(undefined4 *)(iVar4 + 4),7,*(undefined4 *)(iVar4 + 8));
          iVar8 = FUN_08024ea0(param_1 + 1);
          if (iVar8 != 0) {
            return;
          }
        }
        param_1[0x1d] = 0;
        *(byte *)((int)param_1 + 5) = *(byte *)((int)param_1 + 5) & 0xfe;
      }
      goto LAB_08024a68;
    }
    if (*(int *)(param_2 + 8) == 10) {
      FUN_0802ad74(*(undefined4 *)(iVar4 + 4),2,*(undefined4 *)(iVar4 + 8));
      iVar5 = FUN_08024ea0(param_1 + 1);
      if (iVar5 != 0) {
        return;
      }
    }
    if (((*(byte *)((int)param_1 + 5) & 1) == 0) &&
       (uVar6 = FUN_080334c0(param_1[0x1d]), *(ushort *)(*param_1 + 0x1c) < uVar6)) {
      *(byte *)((int)param_1 + 5) = *(byte *)((int)param_1 + 5) | 1;
      if (*(int *)(param_2 + 8) != 10) goto LAB_08024a68;
      iVar8 = FUN_080334d8();
      param_1[0x1e] = iVar8;
      FUN_0802ad74(*(undefined4 *)(iVar4 + 4),5,*(undefined4 *)(iVar4 + 8));
      iVar8 = FUN_08024ea0(param_1 + 1);
    }
    else {
      if (((*(byte *)((int)param_1 + 5) & 1) == 0) ||
         (uVar6 = FUN_080334c0(param_1[0x1e]), uVar6 <= *(ushort *)(*param_1 + 0x1e)))
      goto LAB_08024a68;
      iVar5 = FUN_080334d8();
      param_1[0x1e] = iVar5;
      iVar5 = *(int *)(param_2 + 8);
      if (iVar5 == 10) {
        FUN_0802ad74(*(undefined4 *)(iVar4 + 4),6,*(undefined4 *)(iVar4 + 8));
        iVar8 = FUN_08024ea0(param_1 + 1);
      }
      else if (iVar5 == 9) {
        FUN_0802b8b2(iVar8,0);
        FUN_0802b644(iVar8);
        iVar8 = FUN_08024ea0(param_1 + 1);
      }
      else if (iVar5 == 0xb) {
        FUN_0802b8b2(iVar8,0);
        FUN_0802b70c(iVar8);
        iVar8 = FUN_08024ea0(param_1 + 1);
      }
      else {
        FUN_0802b88a(iVar8);
        iVar8 = FUN_08024ea0(param_1 + 1);
      }
    }
  }
  if (iVar8 != 0) {
    return;
  }
LAB_08024a68:
  *(undefined4 *)(iVar4 + 4) = 0;
  return;
}

