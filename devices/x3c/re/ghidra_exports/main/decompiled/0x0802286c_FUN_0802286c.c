// 0x0802286c FUN_0802286c size=780

void FUN_0802286c(uint param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  
  iVar7 = DAT_08022b8c;
  iVar5 = DAT_08022b88;
  iVar1 = DAT_08022b84;
  uVar6 = 0x50;
  if (((*(char *)(DAT_08022b84 + 0x15) == '\0') &&
      (iVar2 = FUN_0802b744(*(undefined4 *)(DAT_08022b84 + 0x94)), iVar2 != 0)) ||
     (*(char *)(iVar1 + 0xc) != '\0')) {
    uVar4 = 0;
    do {
      if ((int)((uint)*(byte *)(iVar7 + uVar4 * 6 + 3) << 0x18) < 0) {
        iVar8 = iVar5 + uVar4 * 0x30;
        iVar2 = FUN_080303b8(*(undefined4 *)(iVar8 + 0x24));
        if (iVar2 != 0) {
          uVar3 = FUN_08027c88(0xff,0xdc);
          FUN_0803157c(*(undefined4 *)(iVar8 + 0x24),uVar3,0);
          FUN_08031594(*(undefined4 *)(iVar8 + 0x24),0x14,0);
        }
      }
      uVar4 = uVar4 + 1 & 0xff;
    } while (uVar4 < 0x10);
  }
  if (*(char *)(iVar1 + 0x20) == *(char *)(iVar1 + 1)) {
    if ((*(byte *)(iVar1 + 0x15) == param_1) && (*(char *)(iVar1 + 0x16) == '\0')) {
      if (param_1 == 0) {
        return;
      }
      if (param_1 == 0x50) {
        return;
      }
      if (param_1 == 0x80) {
        return;
      }
    }
  }
  else {
    *(char *)(iVar1 + 0x20) = *(char *)(iVar1 + 1);
  }
  uVar3 = FUN_0802b754(*(undefined4 *)(iVar1 + 0x94));
  FUN_0802b798(*(undefined4 *)(iVar1 + 0x94));
  if (*(char *)(iVar1 + 0xc) != '\0') {
    *(undefined1 *)(iVar1 + 0xc) = 0;
  }
  FUN_0802b8b2(*(undefined4 *)(iVar1 + 0x94),0);
  iVar2 = DAT_08022ba0;
  if (*DAT_08022b90 != '\0') {
    return;
  }
  if (*(char *)(iVar1 + 1) == '\0') goto switchD_08022970_default;
  if (*(char *)(iVar1 + 2) != '\0') {
    FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar1 + 0x84));
    FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar1 + 0x88));
    goto switchD_08022970_default;
  }
  if (param_1 != 9) {
    if ((int)param_1 < 10) {
      switch(param_1) {
      case 0:
        uVar4 = 0;
        do {
          if ((int)((uint)*(byte *)(iVar7 + uVar4 * 6 + 3) << 0x18) < 0) {
            iVar2 = iVar5 + uVar4 * 0x30;
            FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar2 + 0x28));
            FUN_0802e34c(*(undefined4 *)(iVar2 + 0x28),8);
            if (uVar6 == 0x50) {
              uVar6 = uVar4;
            }
          }
          uVar4 = uVar4 + 1 & 0xff;
        } while (uVar4 < 0x10);
        FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar1 + 0x40));
        FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar1 + 0x3c));
        uVar4 = *(byte *)(iVar1 + 0x15) - 1;
        if (uVar4 < 0x10) {
          FUN_0802b670(*(undefined4 *)(iVar5 + uVar4 * 0x30 + 0x28));
        }
        else if (*(byte *)(iVar1 + 0x15) == 0x50) {
          FUN_0802b670(*(undefined4 *)(iVar1 + 0x40));
        }
        else if (uVar6 == 0x50) {
          FUN_0802b670(*(undefined4 *)(iVar1 + 0x3c));
        }
        else {
          FUN_0802b670(*(undefined4 *)(iVar5 + uVar6 * 0x30 + 0x28));
        }
        break;
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
        goto switchD_08022970_caseD_1;
      }
      goto switchD_08022970_default;
    }
    if (param_1 != 0xe) {
      if ((int)param_1 < 0xf) {
        if (((param_1 != 10) && (param_1 != 0xb)) && ((param_1 != 0xc && (param_1 != 0xd))))
        goto switchD_08022970_default;
      }
      else if ((param_1 != 0xf) && (param_1 != 0x10)) {
        if (param_1 == 0x50) {
          uVar6 = 0;
          iVar5 = DAT_08022b94 + -0xa0;
          do {
            iVar7 = FUN_080303b8(*(undefined4 *)(iVar5 + uVar6 * 4));
            if (iVar7 != 0) {
              FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar5 + uVar6 * 4));
            }
            uVar6 = uVar6 + 1 & 0xff;
          } while (uVar6 < 0x10);
          FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar1 + 0xa0));
        }
        else if (param_1 == 0x80) {
          FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(DAT_08022ba0 + 0xc));
          FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar2 + 0x10));
          FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar2 + 0x14));
          if (*DAT_08022ba4 == '\0') {
            FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar2 + 0x18));
          }
          FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar2 + 0x1c));
        }
        goto switchD_08022970_default;
      }
    }
  }
switchD_08022970_caseD_1:
  FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar1 + 0x78));
  iVar7 = iVar7 + (param_1 - 1) * 6;
  iVar5 = DAT_08022b94 + (param_1 - 1) * 0x74;
  if ((int)((uint)*(byte *)(iVar7 + 3) << 0x19) < 0) {
    FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar5 + 4));
    if ((param_1 < 4) && (*DAT_08022b98 == '\0')) {
      FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar5 + 0x44));
    }
    else if ((param_1 < 6) && ((*DAT_08022b9c == '\0' && (*DAT_08022b98 == '\0')))) {
      FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar5 + 0x44));
    }
    FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar5 + 0x4c));
    FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar5 + 0x50));
    if (((*(char *)(iVar1 + 3) != '\0') && ((int)((uint)*(byte *)(iVar7 + 4) << 0x18) < 0)) &&
       ((*(byte *)(iVar7 + 5) & 1) == 0)) {
      FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar5 + 100));
    }
  }
  else {
    FUN_0802b584(*(undefined4 *)(iVar1 + 0x94),*(undefined4 *)(iVar5 + 0x48));
  }
  iVar2 = FUN_0802ed7a(uVar3);
  if (iVar2 == *(int *)(iVar1 + 0x94)) {
    FUN_0802b670(uVar3);
  }
  else if ((int)((uint)*(byte *)(iVar7 + 3) << 0x19) < 0) {
    FUN_0802b670(*(undefined4 *)(iVar5 + 4));
  }
  else {
    FUN_0802b670(uVar3);
  }
switchD_08022970_default:
  *(char *)(iVar1 + 0x15) = (char)param_1;
  return;
}

