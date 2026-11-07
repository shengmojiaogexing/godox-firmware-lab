// 0x0800ba4c FUN_0800ba4c size=582

void FUN_0800ba4c(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  byte bVar2;
  undefined1 uVar3;
  undefined4 uVar4;
  int iVar5;
  char *pcVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  undefined4 uVar11;
  int iVar12;
  
  iVar5 = DAT_0800bc98;
  iVar10 = DAT_0800bc94;
  uVar11 = *(undefined4 *)(param_1 + 4);
  uVar9 = 0xff;
  uVar7 = 0;
  do {
    iVar8 = *(int *)(DAT_0800bc94 + uVar7 * 0x74);
    if (iVar8 == *(int *)(param_1 + 0xc)) {
      uVar9 = uVar7;
      break;
    }
    uVar7 = uVar7 + 1 & 0xff;
  } while (uVar7 < 0x10);
  if (uVar9 < 0x10) {
    if ((*DAT_0800bc9c == '\0') &&
       (iVar8 = FUN_0802fcb6(uVar11,8,iVar8,param_4,param_4), iVar8 == 0)) {
      iVar12 = iVar10 + uVar9 * 0x74;
      iVar8 = FUN_080303b8(*(undefined4 *)(iVar12 + 0xc));
      if (iVar8 != 0) {
        uVar4 = FUN_08027f68();
        FUN_0803157c(*(undefined4 *)(iVar12 + 0xc),uVar4,0);
      }
    }
    iVar8 = DAT_0800bca0;
    cVar1 = *(char *)(param_1 + 8);
    if (cVar1 == '\a') {
      if (*(char *)(iVar5 + 0x14) == '\0') {
        *(undefined1 *)(iVar5 + 0x14) = 0xe;
      }
      else {
        iVar5 = DAT_0800bca0 + uVar9 * 6;
        *(byte *)(iVar5 + 3) = *(byte *)(iVar5 + 3) ^ 0x40;
      }
      FUN_0802e34c(uVar11,8);
      FUN_0802b8b2(*(undefined4 *)(DAT_0800bc98 + 0x94),0);
      iVar10 = iVar10 + uVar9 * 0x74;
      iVar5 = FUN_080303b8(*(undefined4 *)(iVar10 + 0xc));
      if (iVar5 != 0) {
        uVar11 = FUN_08027f68();
        FUN_0803157c(*(undefined4 *)(iVar10 + 0xc),uVar11,0,uVar11);
        return;
      }
    }
    else if (cVar1 == '\r') {
      pcVar6 = (char *)FUN_0802ad6c(param_1);
      if (*pcVar6 == '\x13') {
        cVar1 = *(char *)(iVar8 + uVar9 * 6);
        if (cVar1 == '\0') {
          if ((*(char *)(iVar5 + 5) == '\0') && (*DAT_0800bca4 != '\0')) {
            iVar8 = iVar8 + uVar9 * 6;
            uVar3 = FUN_0801a38c(*(undefined1 *)(iVar8 + 1),10);
            *(undefined1 *)(iVar8 + 1) = uVar3;
          }
          else {
            iVar8 = iVar8 + uVar9 * 6;
            uVar3 = FUN_0801a38c(*(undefined1 *)(iVar8 + 1),1);
            *(undefined1 *)(iVar8 + 1) = uVar3;
          }
        }
        else if (cVar1 == '\x01') {
          iVar8 = iVar8 + uVar9 * 6;
          bVar2 = *(byte *)(iVar8 + 2);
          uVar9 = (uint)bVar2;
          if (uVar9 < 0x3c) {
            if (uVar9 % 10 < 3) {
              *(byte *)(iVar8 + 2) = (bVar2 / 10) * '\n' + '\x03';
            }
            else if (uVar9 % 10 < 7) {
              *(byte *)(iVar8 + 2) = (bVar2 / 10) * '\n' + '\a';
            }
            else {
              *(byte *)(iVar8 + 2) = (bVar2 / 10 + 1) * '\n';
            }
          }
        }
      }
      else if (*pcVar6 == '\x14') {
        if (*(char *)(iVar8 + uVar9 * 6) == '\0') {
          if ((*(char *)(iVar5 + 5) == '\0') && (*DAT_0800bca4 != '\0')) {
            *DAT_0800bca4 = '\0';
            iVar8 = iVar8 + uVar9 * 6;
            uVar3 = FUN_0801a38c(*(undefined1 *)(iVar8 + 1),0xfffffff6);
            *(undefined1 *)(iVar8 + 1) = uVar3;
          }
          else {
            iVar8 = iVar8 + uVar9 * 6;
            uVar3 = FUN_0801a38c(*(undefined1 *)(iVar8 + 1),0xffffffff);
            *(undefined1 *)(iVar8 + 1) = uVar3;
          }
        }
        else {
          iVar8 = iVar8 + uVar9 * 6;
          bVar2 = *(byte *)(iVar8 + 2);
          uVar9 = (uint)bVar2;
          if (uVar9 != 0) {
            if (uVar9 % 10 < 4) {
              *(byte *)(iVar8 + 2) = bVar2 - 3;
            }
            else if (uVar9 % 10 == 7) {
              *(byte *)(iVar8 + 2) = bVar2 - 4;
            }
          }
        }
      }
    }
    else if (cVar1 == '\x0e') {
      iVar12 = FUN_0802fcb6(*(undefined4 *)(param_1 + 4),8);
      if (iVar12 != 0) {
        if (*(char *)(iVar5 + 0x14) == '\0') {
          *(undefined1 *)(iVar5 + 0x14) = 0xe;
        }
        else {
          iVar8 = iVar8 + uVar9 * 6;
          *(byte *)(iVar8 + 3) = *(byte *)(iVar8 + 3) ^ 0x40;
        }
        uVar11 = FUN_08027c88(0xfd,0x9f,10);
        FUN_0803157c(*(undefined4 *)(iVar10 + uVar9 * 0x74 + 0xc),uVar11,0);
      }
      *(undefined1 *)(iVar5 + 10) = 0;
    }
  }
  return;
}

