// 0x0800b558 FUN_0800b558 size=320

void FUN_0800b558(int param_1)

{
  byte bVar1;
  int iVar2;
  byte bVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  undefined4 uVar10;
  
  uVar8 = 0xff;
  iVar9 = *(int *)(param_1 + 0xc);
  uVar10 = *(undefined4 *)(param_1 + 4);
  thunk_FUN_08027774(uVar10);
  iVar5 = DAT_0800b6a0;
  iVar2 = DAT_0800b698;
  if ((*(char *)(DAT_0800b698 + 0xd) != '\x02') && (*(char *)(DAT_0800b698 + 0xe) == '\0')) {
    uVar7 = 0;
    do {
      if (*(int *)(DAT_0800b69c + uVar7 * 0x74) == iVar9) {
        uVar8 = uVar7;
        break;
      }
      uVar7 = uVar7 + 1 & 0xff;
    } while (uVar7 < 0x10);
    if (uVar8 < 0x10) {
      cVar4 = *(char *)(param_1 + 8);
      if (cVar4 == '\x1c') {
        *(undefined1 *)(DAT_0800b698 + 0x11) = 1;
        bVar3 = thunk_FUN_08027774(*(undefined4 *)(param_1 + 4));
        if (*(byte *)(iVar2 + 0x1e) == 0) {
          if ((*(char *)(iVar2 + 0x1f) == '\0') ||
             (iVar9 = FUN_0802fcb6(*(undefined4 *)(param_1 + 4),0x20), iVar9 != 0)) {
            *(undefined1 *)(iVar2 + 0x1f) = 0;
            *(byte *)(iVar2 + 0x1e) = bVar3;
          }
          else {
            *(undefined1 *)(iVar2 + 0x1f) = 0;
            cVar4 = thunk_FUN_08027774(*(undefined4 *)(param_1 + 4));
            *(byte *)(iVar5 + uVar8 * 6 + 4) = cVar4 + 10U | 0x80;
          }
        }
        else {
          iVar9 = (uint)bVar3 - (uint)*(byte *)(iVar2 + 0x1e);
          if (iVar9 < 0) {
            iVar6 = iVar5 + uVar8 * 6;
            bVar1 = *(byte *)(iVar6 + 4);
            if ((short)(ushort)(byte)((bVar1 & 0x7f) - 10) < (short)-(short)iVar9) {
              *(byte *)(iVar6 + 4) = bVar1 & 0x80 | 10;
            }
            else {
              *(byte *)(iVar6 + 4) = (char)iVar9 + bVar1;
            }
          }
          else {
            iVar6 = iVar5 + uVar8 * 6;
            if ((int)(100 - (*(byte *)(iVar6 + 4) & 0x7f)) < iVar9) {
              *(undefined1 *)(iVar6 + 4) = 100;
            }
            else {
              uVar7 = iVar9 + (uint)*(byte *)(iVar6 + 4);
              *(char *)(iVar6 + 4) = (char)uVar7;
              if (100 < (uVar7 & 0x7f)) {
                *(undefined1 *)(iVar6 + 4) = 100;
              }
            }
          }
          iVar5 = iVar5 + uVar8 * 6;
          *(byte *)(iVar5 + 4) = *(byte *)(iVar5 + 4) | 0x80;
          *(byte *)(iVar2 + 0x1e) = bVar3;
        }
      }
      else if (cVar4 == '\x01') {
        *(undefined1 *)(DAT_0800b698 + 0x1e) = 0;
        *(undefined1 *)(iVar2 + 0x10) = 1;
        *(undefined1 *)(iVar2 + 0x1f) = 1;
      }
      else if (cVar4 == '\b') {
        *(undefined1 *)(DAT_0800b698 + 0x11) = 0;
        *(undefined1 *)(iVar2 + 0x10) = 0;
        thunk_FUN_0802782e(uVar10,(*(byte *)(iVar5 + uVar8 * 6 + 4) & 0x7f) - 10,1);
        return;
      }
    }
  }
  return;
}

