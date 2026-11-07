// 0x0801b5c4 FUN_0801b5c4 size=654

void FUN_0801b5c4(uint param_1)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  undefined1 uVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  int iVar8;
  undefined4 uVar9;
  uint uVar10;
  char cVar11;
  undefined4 *puVar12;
  bool bVar13;
  bool bVar14;
  
  iVar6 = DAT_0801b854;
  uVar9 = *(undefined4 *)(param_1 + 4);
  cVar11 = *(char *)(param_1 + 8);
  if (cVar11 == '\a') {
    iVar6 = FUN_0802fcb6(uVar9,8);
    if (iVar6 != 0) {
      FUN_0802e34c(*(undefined4 *)(param_1 + 4),8);
      FUN_0802b8b2(*(undefined4 *)(DAT_0801b854 + 0x94),0);
      return;
    }
  }
  else if (cVar11 == '\r') {
    iVar5 = FUN_0802fcb6(uVar9,8);
    bVar13 = iVar5 == 0;
    do {
      if (bVar13) {
        return;
      }
      bVar2 = true;
      if (*(short *)(iVar6 + 0x24) == 0 && *(short *)(iVar6 + 0x26) == 0) {
        FUN_08009580(DAT_0801b854 + 0x2a);
        *(undefined2 *)(iVar6 + 0x24) = *(undefined2 *)(iVar6 + 0x2a);
        *(undefined2 *)(iVar6 + 0x26) = *(undefined2 *)(iVar6 + 0x2c);
      }
      pcVar7 = (char *)FUN_0802ad6c(param_1);
      iVar3 = DAT_0801b85c;
      iVar5 = DAT_0801b858;
      if (*pcVar7 != '\x14') {
        bVar14 = *pcVar7 == '\x13';
        goto LAB_0801b682;
      }
      bVar14 = false;
      cVar11 = '\0';
      param_1 = 0;
      do {
        iVar8 = iVar3 + param_1 * 6;
        if (*(byte *)(iVar8 + 3) >> 6 == 3) {
          cVar1 = *(char *)(iVar3 + param_1 * 6);
          if (cVar1 == '\0') {
            if (*(byte *)(iVar6 + 4) <= *(byte *)(iVar8 + 1)) {
              bVar14 = true;
              puVar12 = (undefined4 *)(iVar5 + param_1 * 0x30);
              FUN_0801b860(*puVar12,0);
              if (bVar2) {
                bVar2 = false;
                FUN_0803118e(*puVar12,1);
              }
            }
          }
          else if ((cVar1 == '\x01') && (*(char *)(iVar8 + 2) == '\0')) {
            bVar14 = true;
            puVar12 = (undefined4 *)(iVar5 + param_1 * 0x30);
            FUN_0801b860(*puVar12,0);
            if (bVar2) {
              bVar2 = false;
              FUN_0803118e(*puVar12,1);
            }
          }
        }
        else {
          cVar11 = cVar11 + '\x01';
        }
        param_1 = param_1 + 1 & 0xff;
      } while (param_1 < 0x10);
      bVar13 = true;
    } while (cVar11 == '\x10');
    bVar14 = !bVar14;
    if (bVar14) {
      uVar10 = 0;
      do {
        iVar5 = iVar3 + uVar10 * 6;
        if (*(byte *)(iVar5 + 3) >> 6 == 3) {
          if (*(char *)(iVar3 + uVar10 * 6) == '\0') {
            uVar4 = FUN_0801a38c(*(undefined1 *)(iVar5 + 1),0xffffffff);
            *(undefined1 *)(iVar5 + 1) = uVar4;
          }
          else {
            uVar4 = FUN_0801a294(*(undefined1 *)(iVar5 + 2),0xffffffff);
            *(undefined1 *)(iVar5 + 2) = uVar4;
          }
        }
        uVar10 = uVar10 + 1 & 0xff;
      } while (uVar10 < 0x10);
      *(short *)(iVar6 + 0x2a) = *(short *)(iVar6 + 0x2a) + -1;
      *(short *)(iVar6 + 0x2c) = *(short *)(iVar6 + 0x2c) + 1;
      *(short *)(iVar6 + 0x28) = *(short *)(iVar6 + 0x28) + -1;
      *(undefined1 *)(iVar6 + 6) = 0x50;
    }
    else {
LAB_0801b682:
      if (bVar14) {
        bVar13 = false;
        cVar11 = '\0';
        uVar10 = 0;
        do {
          iVar8 = iVar3 + uVar10 * 6;
          if (*(byte *)(iVar8 + 3) >> 6 == 3) {
            cVar1 = *(char *)(iVar3 + uVar10 * 6);
            if (cVar1 == '\0') {
              if (*(char *)(iVar8 + 1) == '\0') {
                bVar13 = true;
                puVar12 = (undefined4 *)(iVar5 + uVar10 * 0x30);
                FUN_0801b860(*puVar12,0);
                if (bVar2) {
                  bVar2 = false;
                  FUN_0803118e(*puVar12,1);
                }
              }
            }
            else if ((cVar1 == '\x01') && (*(char *)(iVar8 + 2) == '<')) {
              bVar13 = true;
              puVar12 = (undefined4 *)(iVar5 + uVar10 * 0x30);
              FUN_0801b860(*puVar12,0);
              if (bVar2) {
                bVar2 = false;
                FUN_0803118e(*puVar12,1);
              }
            }
          }
          else {
            cVar11 = cVar11 + '\x01';
          }
          uVar10 = uVar10 + 1 & 0xff;
        } while (uVar10 < 0x10);
        if ((cVar11 != '\x10') && (!bVar13)) {
          uVar10 = 0;
          do {
            iVar5 = iVar3 + uVar10 * 6;
            if (*(byte *)(iVar5 + 3) >> 6 == 3) {
              if (*(char *)(iVar3 + uVar10 * 6) == '\0') {
                uVar4 = FUN_0801a38c(*(undefined1 *)(iVar5 + 1),1);
                *(undefined1 *)(iVar5 + 1) = uVar4;
              }
              else {
                uVar4 = FUN_0801a294(*(undefined1 *)(iVar5 + 2),1);
                *(undefined1 *)(iVar5 + 2) = uVar4;
              }
            }
            uVar10 = uVar10 + 1 & 0xff;
          } while (uVar10 < 0x10);
          *(short *)(iVar6 + 0x2a) = *(short *)(iVar6 + 0x2a) + 1;
          *(short *)(iVar6 + 0x2c) = *(short *)(iVar6 + 0x2c) + -1;
          *(short *)(iVar6 + 0x28) = *(short *)(iVar6 + 0x28) + 1;
          *(undefined1 *)(iVar6 + 6) = 0x50;
        }
      }
    }
  }
  else if (((cVar11 == '\x0e') && (iVar5 = FUN_0802fcb6(uVar9,8), iVar5 != 0)) &&
          (*(short *)(iVar6 + 0x24) == 0 && *(short *)(iVar6 + 0x26) == 0)) {
    FUN_08009580(iVar6 + 0x2a);
    *(undefined2 *)(iVar6 + 0x24) = *(undefined2 *)(iVar6 + 0x2a);
    *(undefined2 *)(iVar6 + 0x26) = *(undefined2 *)(iVar6 + 0x2c);
  }
  return;
}

