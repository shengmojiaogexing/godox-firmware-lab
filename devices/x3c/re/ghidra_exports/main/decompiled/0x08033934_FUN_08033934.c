// 0x08033934 FUN_08033934 size=322

int FUN_08033934(char *param_1,int param_2,int param_3,int param_4,uint param_5,int *param_6,
                undefined4 param_7,int param_8)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  int local_3c;
  int local_38;
  char *pcStack_34;
  int local_30;
  int local_2c;
  int local_28;
  
  if ((param_1 == (char *)0x0) || (*param_1 == '\0')) {
    iVar6 = 0;
  }
  else {
    if (param_2 != 0) {
      local_28 = param_4;
      if ((int)(param_5 << 0x1e) < 0) {
        local_28 = 0x1fffffff;
      }
      local_38 = 0;
      local_3c = 0;
      uVar10 = 0;
      iVar7 = 0;
      iVar6 = -1;
      pcStack_34 = param_1;
      local_30 = param_2;
      local_2c = param_3;
      uVar3 = (**(code **)(DAT_08033a78 + 4))(param_1,&local_38);
      iVar8 = 0;
      iVar9 = 0;
LAB_08033a3c:
      do {
        if (param_1[iVar8] == '\0') goto LAB_08033a44;
        local_3c = local_38;
        uVar10 = (**(code **)(DAT_08033a78 + 4))(param_1,&local_3c);
        if ((param_5 & 1) != 0) {
          iVar4 = FUN_08018a20(param_7,uVar3);
          iVar2 = local_38;
          if (iVar4 != 0) {
            local_38 = local_3c;
            uVar3 = uVar10;
            iVar8 = iVar2;
            iVar9 = iVar9 + 1;
            goto LAB_08033a3c;
          }
        }
        iVar4 = FUN_0802afc2(local_30,uVar3,uVar10);
        iVar2 = local_38;
        iVar7 = iVar7 + iVar4;
        if (0 < iVar4) {
          iVar7 = iVar7 + local_2c;
        }
        if ((iVar6 == -1) && (local_28 < iVar7 - local_2c)) {
          iVar6 = iVar8;
        }
        if ((uVar3 == 10) || (uVar3 == 0xd)) {
LAB_08033a10:
          if (((iVar8 == 0) && (iVar6 == -1)) && (param_6 != (int *)0x0)) {
            *param_6 = iVar7;
          }
          goto LAB_08033a44;
        }
        bVar1 = false;
        if (uVar3 - 0x4e00 < 0x51a6) {
          bVar1 = true;
        }
        else {
          for (uVar5 = 0; (&DAT_08033a7c)[uVar5] != '\0'; uVar5 = uVar5 + 1 & 0xff) {
            if ((byte)(&DAT_08033a7c)[uVar5] == uVar3) {
              bVar1 = true;
              break;
            }
          }
        }
        if (bVar1) goto LAB_08033a10;
        if ((param_6 != (int *)0x0) && (iVar6 == -1)) {
          *param_6 = iVar7;
        }
        local_38 = local_3c;
        uVar3 = uVar10;
        iVar8 = iVar2;
        iVar9 = iVar9 + 1;
      } while( true );
    }
    iVar6 = 0;
  }
LAB_08033980:
  return iVar6;
LAB_08033a44:
  if (iVar6 == -1) {
    if ((iVar9 == 0) || ((iVar6 = iVar8, uVar3 == 0xd && (uVar10 == 10)))) {
      iVar6 = local_38;
    }
  }
  else if (param_8 == 0) {
    if (param_6 != (int *)0x0) {
      *param_6 = 0;
    }
    iVar6 = 0;
  }
  goto LAB_08033980;
}

