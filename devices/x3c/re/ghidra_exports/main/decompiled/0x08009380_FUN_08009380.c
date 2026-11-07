// 0x08009380 FUN_08009380 size=494

void FUN_08009380(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  byte bVar2;
  char cVar3;
  int iVar4;
  undefined1 uVar5;
  undefined2 uVar6;
  undefined4 uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  undefined4 uVar12;
  
  iVar4 = DAT_08009574;
  uVar11 = 0xff;
  uVar12 = *(undefined4 *)(param_1 + 0xc);
  if (*DAT_08009570 != '\x01') {
    if (*(char *)(DAT_08009574 + 8) == '\0') {
      uVar9 = 0;
      do {
        iVar8 = *(int *)(DAT_08009578 + uVar9 * 0x30 + 0x20);
        if (iVar8 == *(int *)(param_1 + 4)) {
          uVar11 = uVar9;
          break;
        }
        uVar9 = uVar9 + 1 & 0xff;
      } while (uVar9 < 0x10);
      iVar10 = DAT_0800957c + uVar11 * 6;
      if ((int)((uint)*(byte *)(iVar10 + 3) << 0x19) < 0) {
        cVar1 = *(char *)(param_1 + 8);
        if (cVar1 == '\x1c') {
          *(undefined1 *)(DAT_08009574 + 0x11) = 1;
          *(undefined1 *)(iVar4 + 0x18) = 1;
          uVar7 = FUN_08027c88(0xfd,0x9f,10,iVar8,param_3,param_4);
          FUN_0803157c(uVar12,uVar7,0);
          FUN_08031594(uVar12,0xff,0);
          if (*(short *)(iVar4 + 0x2e) == 0) {
            uVar6 = thunk_FUN_08027774(*(undefined4 *)(param_1 + 4));
            *(undefined2 *)(iVar4 + 0x2e) = uVar6;
          }
          else {
            *(undefined1 *)(iVar4 + 0x13) = 1;
            uVar6 = thunk_FUN_08027774(*(undefined4 *)(param_1 + 4));
            cVar1 = (char)uVar6 - (char)*(undefined2 *)(iVar4 + 0x2e);
            *(undefined2 *)(iVar4 + 0x2e) = uVar6;
            if (uVar11 != 0xff) {
              cVar3 = *(char *)(DAT_0800957c + uVar11 * 6);
              if (cVar3 == '\0') {
                uVar5 = FUN_0801a38c(*(undefined1 *)(iVar10 + 1),(int)cVar1);
                *(undefined1 *)(iVar10 + 1) = uVar5;
              }
              else if (cVar3 == '\x01') {
                uVar5 = FUN_0801a294(*(undefined1 *)(iVar10 + 2),(int)cVar1);
                *(undefined1 *)(iVar10 + 2) = uVar5;
              }
            }
          }
          *(undefined2 *)(iVar4 + 0x24) = 0;
          *(undefined2 *)(iVar4 + 0x26) = 0;
        }
        else if (cVar1 == '\x01') {
          *(undefined1 *)(DAT_08009574 + 0x18) = 0;
          *(undefined2 *)(iVar4 + 0x2e) = 0;
          *(undefined1 *)(iVar4 + 0x13) = 0;
        }
        else {
          if (cVar1 == '\b') {
            *(undefined1 *)(DAT_08009574 + 0x12) = 0;
            *(undefined1 *)(iVar4 + 0x11) = 0;
            uVar7 = FUN_08027c88(0xff,0xdc,0xff,iVar8,param_3,param_4);
            FUN_0803157c(uVar12,uVar7,0);
            FUN_08031594(uVar12,0x14,0);
            return;
          }
          if (cVar1 == '\x04') {
            if ((((*(char *)(DAT_08009574 + 0xd) != '\x02') &&
                 (*(char *)(DAT_08009574 + 0x18) != '\x01')) && (*DAT_08009570 != '\x01')) &&
               (uVar11 < 0x10)) {
              FUN_0802e292(*(undefined4 *)(DAT_08009574 + 0x5c));
              uVar12 = FUN_08010acc(*(undefined4 *)(iVar4 + 0x5c),0,0,0,0x170,0x198);
              *(undefined4 *)(iVar4 + 0x68) = uVar12;
              FUN_0800d504(uVar12,uVar11);
              *(char *)(iVar4 + 8) = (char)uVar11 + '\x01';
              FUN_0803118e(*(undefined4 *)(iVar4 + 0x5c),1);
              *(undefined1 *)(iVar4 + 6) = 0;
            }
          }
          else if ((cVar1 == '\x06') && (*(char *)(DAT_08009574 + 0x13) != '\x01')) {
            bVar2 = *(byte *)(DAT_08009574 + 0x12);
            if (bVar2 == 8) {
              *(byte *)(iVar10 + 3) = *(byte *)(iVar10 + 3) ^ 0x40;
              *(undefined1 *)(iVar4 + 6) = 0;
              uVar7 = FUN_08027c88(0xff,0xdc,0xff,iVar8,param_3,param_4);
              FUN_0803157c(uVar12,uVar7,0);
              FUN_08031594(uVar12,0x14,0);
              *(char *)(iVar4 + 0x12) = *(char *)(iVar4 + 0x12) + '\x01';
            }
            else if (bVar2 < 8) {
              *(byte *)(DAT_08009574 + 0x12) = bVar2 + 1;
            }
          }
        }
      }
    }
    else if (*(char *)(DAT_08009574 + 0x11) != '\0') {
      *(undefined1 *)(DAT_08009574 + 0x11) = 0;
      uVar7 = FUN_08027c88(0xff,0xdc,0xff,param_4,param_3,param_4);
      FUN_0803157c(uVar12,uVar7,0);
      FUN_08031594(uVar12,0x14,0);
      return;
    }
  }
  return;
}

