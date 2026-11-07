// 0x0800aedc FUN_0800aedc size=530

void FUN_0800aedc(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  byte bVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  undefined1 uVar6;
  undefined2 uVar7;
  uint uVar8;
  int iVar9;
  undefined4 uVar10;
  int iVar11;
  uint uVar12;
  undefined4 uVar13;
  
  iVar5 = DAT_0800b0f8;
  iVar4 = DAT_0800b0f4;
  iVar9 = DAT_0800b0f0;
  uVar12 = 0xff;
  uVar13 = *(undefined4 *)(param_1 + 0xc);
  uVar8 = 0;
  do {
    if (*(int *)(DAT_0800b0f0 + uVar8 * 0x74 + 8) == *(int *)(param_1 + 4)) {
      uVar12 = uVar8;
      break;
    }
    uVar8 = uVar8 + 1 & 0xff;
  } while (uVar8 < 0x10);
  if (uVar12 < 0x10) {
    iVar11 = DAT_0800b0f4 + uVar12 * 6;
    if (((int)((uint)*(byte *)(iVar11 + 3) << 0x19) < 0) && (*(byte *)(DAT_0800b0f8 + 8) < 0x11)) {
      cVar1 = *(char *)(param_1 + 8);
      if (cVar1 == '\x1c') {
        *(undefined1 *)(DAT_0800b0f8 + 0x11) = 1;
        uVar10 = FUN_08027c88(0xfd,0x9f,10,1,param_4);
        FUN_0803157c(uVar13,uVar10,0);
        if (*(short *)(iVar5 + 0x32) == 0) {
          if ((*(char *)(iVar5 + 0x1b) != '\0') &&
             (iVar9 = FUN_0802fcb6(*(undefined4 *)(param_1 + 4),0x20), iVar9 == 0)) {
            *(undefined1 *)(iVar5 + 0x1b) = 0;
            uVar8 = thunk_FUN_08027774(*(undefined4 *)(param_1 + 4));
            cVar1 = *(char *)(iVar4 + uVar12 * 6);
            if (cVar1 == '\0') {
              uVar6 = FUN_0800ad00(uVar8 & 0xffff);
              *(undefined1 *)(iVar11 + 1) = uVar6;
              return;
            }
            if (cVar1 != '\x01') {
              return;
            }
            if ((int)uVar8 % 3 == 0) {
              *(char *)(iVar11 + 2) =
                   ((char)((int)uVar8 / 3) + (char)((int)uVar8 / 3 << 2)) * '\x02';
              return;
            }
            if ((int)uVar8 % 3 == 1) {
              *(char *)(iVar11 + 2) = (char)((int)uVar8 / 3) * '\n' + '\x03';
              return;
            }
            if ((int)uVar8 % 3 == 2) {
              *(char *)(iVar11 + 2) = (char)((int)uVar8 / 3) * '\n' + '\a';
              return;
            }
            return;
          }
          uVar7 = thunk_FUN_08027774(*(undefined4 *)(param_1 + 4));
          *(undefined2 *)(iVar5 + 0x32) = uVar7;
        }
        else {
          uVar7 = thunk_FUN_08027774(*(undefined4 *)(param_1 + 4));
          cVar3 = (char)uVar7 - (char)*(undefined2 *)(iVar5 + 0x32);
          *(undefined1 *)(iVar5 + 0x1d) = 1;
          *(undefined2 *)(iVar5 + 0x32) = uVar7;
          cVar1 = *(char *)(iVar4 + uVar12 * 6);
          if (cVar1 == '\0') {
            uVar6 = FUN_0801a38c(*(undefined1 *)(iVar11 + 1),(int)cVar3);
            *(undefined1 *)(iVar11 + 1) = uVar6;
          }
          else if (cVar1 == '\x01') {
            uVar6 = FUN_0801a294(*(undefined1 *)(iVar11 + 2),(int)cVar3);
            *(undefined1 *)(iVar11 + 2) = uVar6;
          }
        }
        *(undefined1 *)(iVar5 + 0x1b) = 0;
      }
      else if (cVar1 == '\x01') {
        *(undefined1 *)(DAT_0800b0f8 + 0x1d) = 0;
        *(undefined1 *)(iVar5 + 0x1b) = 1;
        *(undefined1 *)(iVar5 + 0x1c) = 0;
        *(undefined2 *)(iVar5 + 0x32) = 0;
        *(undefined1 *)(iVar5 + 0x10) = 1;
      }
      else if (cVar1 == '\b') {
        *(undefined1 *)(DAT_0800b0f8 + 0x11) = 0;
        *(undefined1 *)(iVar5 + 0x10) = 0;
        *(undefined1 *)(iVar5 + 10) = 0;
        uVar10 = FUN_08027f68();
        FUN_0803157c(uVar13,uVar10,0);
        cVar1 = *(char *)(iVar4 + uVar12 * 6);
        if (cVar1 == '\0') {
          uVar6 = FUN_0800ae50(*(undefined1 *)(iVar11 + 1));
          thunk_FUN_0802782e(*(undefined4 *)(iVar9 + uVar12 * 0x74 + 8),uVar6,1,uVar10);
          return;
        }
        if (cVar1 == '\x01') {
          uVar6 = FUN_0800acd4(*(undefined1 *)(iVar11 + 2));
          thunk_FUN_0802782e(*(undefined4 *)(iVar9 + uVar12 * 0x74 + 8),uVar6,1,uVar10);
          return;
        }
      }
      else if (cVar1 == '\x06') {
        if (*(char *)(DAT_0800b0f8 + 0x1d) == '\0') {
          bVar2 = *(byte *)(DAT_0800b0f8 + 0x1c);
          if (bVar2 == 8) {
            *(byte *)(iVar11 + 3) = *(byte *)(iVar11 + 3) ^ 0x40;
            uVar10 = FUN_08027f68();
            FUN_0803157c(uVar13,uVar10,0,uVar10);
            return;
          }
          if (bVar2 < 8) {
            *(byte *)(DAT_0800b0f8 + 0x1c) = bVar2 + 1;
          }
        }
      }
      else if (cVar1 == '\a') {
        *(undefined1 *)(DAT_0800b0f8 + 10) = 0;
      }
    }
  }
  return;
}

