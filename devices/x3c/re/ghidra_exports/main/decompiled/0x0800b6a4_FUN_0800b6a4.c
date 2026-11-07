// 0x0800b6a4 FUN_0800b6a4 size=642

void FUN_0800b6a4(int param_1)

{
  char cVar1;
  int iVar2;
  undefined1 uVar3;
  byte bVar4;
  ushort uVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  char *pcVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  
  uVar6 = *(undefined4 *)(param_1 + 4);
  uVar5 = FUN_0802ed92(uVar6);
  iVar2 = DAT_0800b928;
  uVar12 = (uint)uVar5;
  if (((uVar12 < 0x10) && (*(char *)(DAT_0800b928 + 8) == '\0')) && (*DAT_0800b92c == '\0')) {
    iVar7 = FUN_0802fcb6(*(undefined4 *)(DAT_0800b928 + 0x3c),8);
    if (iVar7 == 0) {
      FUN_080315b6(uVar6,0xff,8);
    }
    else {
      FUN_0802e34c(uVar6,8);
      FUN_080315b6(uVar6,0,8);
    }
    iVar8 = DAT_0800b934;
    iVar13 = DAT_0800b930;
    iVar7 = DAT_0800b928;
    cVar1 = *(char *)(param_1 + 8);
    if (cVar1 == '\x0e') {
      uVar6 = *(undefined4 *)(param_1 + 4);
      if (*(char *)(iVar2 + 0xc) != '\0') {
        FUN_0802da8e(uVar6,8);
        FUN_0802b8b2(*(undefined4 *)(iVar7 + 0x94),1);
      }
      iVar9 = FUN_0802fcb6(uVar6,8);
      if ((iVar9 != 0) && (*(char *)(iVar2 + 8) == '\0')) {
        if (*(char *)(iVar2 + 0xc) == '\0') {
          if (*(char *)(iVar2 + 0x14) == '\0') {
            *(undefined1 *)(iVar2 + 0x14) = 0xe;
            *(undefined1 *)(iVar2 + 0xc) = 1;
          }
          else {
            iVar8 = iVar8 + uVar12 * 6;
            uVar11 = *(byte *)(iVar8 + 3) ^ 0x40;
            *(char *)(iVar8 + 3) = (char)uVar11;
            if (-1 < (int)(uVar11 << 0x19)) {
              FUN_0802e34c(uVar6,8);
              FUN_0802b8b2(*(undefined4 *)(iVar7 + 0x94),0);
            }
          }
        }
        uVar6 = FUN_08027c88(0xfd,0x9f,10);
        iVar13 = iVar13 + uVar12 * 0x30;
        FUN_0803157c(*(undefined4 *)(iVar13 + 0x24),uVar6,0);
        FUN_08031594(*(undefined4 *)(iVar13 + 0x24),0xff,0);
        return;
      }
    }
    else if (cVar1 == '\r') {
      pcVar10 = (char *)FUN_0802ad6c(param_1);
      if (*pcVar10 == '\x14') {
        iVar7 = iVar8 + uVar12 * 6;
        if (*(byte *)(iVar7 + 3) >> 6 == 3) {
          if (*(char *)(iVar8 + uVar12 * 6) == '\0') {
            if ((*(char *)(iVar2 + 5) == '\0') && (*DAT_0800b938 != '\0')) {
              uVar3 = FUN_0801a38c(*(undefined1 *)(iVar7 + 1),0xfffffff6);
              *(undefined1 *)(iVar7 + 1) = uVar3;
            }
            else {
              uVar3 = FUN_0801a38c(*(undefined1 *)(iVar7 + 1),0xffffffff);
              *(undefined1 *)(iVar7 + 1) = uVar3;
            }
          }
          else {
            uVar3 = FUN_0801a294(*(undefined1 *)(iVar7 + 2));
            *(undefined1 *)(iVar7 + 2) = uVar3;
          }
        }
      }
      else if ((*pcVar10 == '\x13') && (iVar7 = iVar8 + uVar12 * 6, *(byte *)(iVar7 + 3) >> 6 == 3))
      {
        if (*(char *)(iVar8 + uVar12 * 6) == '\0') {
          if ((*(char *)(iVar2 + 5) == '\0') && (*DAT_0800b938 != '\0')) {
            uVar3 = FUN_0801a38c(*(undefined1 *)(iVar7 + 1),10);
            *(undefined1 *)(iVar7 + 1) = uVar3;
          }
          else {
            uVar3 = FUN_0801a38c(*(undefined1 *)(iVar7 + 1),1);
            *(undefined1 *)(iVar7 + 1) = uVar3;
          }
        }
        else {
          uVar3 = FUN_0801a294(*(undefined1 *)(iVar7 + 2),1);
          *(undefined1 *)(iVar7 + 2) = uVar3;
        }
      }
    }
    else if (cVar1 == '\x06') {
      bVar4 = *(char *)(iVar2 + 0x1a) + 1;
      *(byte *)(iVar2 + 0x1a) = bVar4;
      if (2 < bVar4) {
        *(undefined1 *)(iVar2 + 0xc) = 0;
        *(undefined1 *)(iVar2 + 10) = 1;
        *(undefined1 *)(iVar2 + 0x1a) = 0;
        FUN_0802e292(*(undefined4 *)(iVar2 + 0x5c));
        uVar6 = FUN_08010acc(*(undefined4 *)(iVar2 + 0x5c),0,0,0,0x170,0x198);
        *(undefined4 *)(iVar2 + 0x68) = uVar6;
        FUN_0800d504(uVar6,(char)uVar5);
        *(char *)(iVar2 + 8) = (char)uVar5 + '\x01';
        FUN_0803118e(*(undefined4 *)(iVar2 + 0x5c),1);
        *(undefined1 *)(iVar2 + 6) = 0;
        uVar6 = FUN_08027c88(0xff,0xdc);
        iVar13 = iVar13 + uVar12 * 0x30;
        FUN_0803157c(*(undefined4 *)(iVar13 + 0x24),uVar6,0);
        FUN_08031594(*(undefined4 *)(iVar13 + 0x24),0x14,0);
        return;
      }
    }
    else if (cVar1 == '\x04') {
      *(undefined1 *)(iVar2 + 0xc) = 0;
      if (*(char *)(iVar2 + 0x14) == '\0') {
        *(undefined1 *)(iVar2 + 0x14) = 0xe;
      }
      else {
        iVar8 = iVar8 + uVar12 * 6;
        *(byte *)(iVar8 + 3) = *(byte *)(iVar8 + 3) ^ 0x40;
        *(undefined1 *)(iVar2 + 6) = 0;
      }
      FUN_0802e34c(uVar6,8);
      FUN_0802b8b2(*(undefined4 *)(iVar7 + 0x94),0);
      uVar6 = FUN_08027c88(0xff,0xdc);
      iVar13 = iVar13 + uVar12 * 0x30;
      FUN_0803157c(*(undefined4 *)(iVar13 + 0x24),uVar6,0);
      FUN_08031594(*(undefined4 *)(iVar13 + 0x24),0x14,0);
      return;
    }
  }
  return;
}

