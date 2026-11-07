// 0x0800c000 FUN_0800c000 size=186

void FUN_0800c000(int param_1)

{
  byte bVar1;
  char cVar2;
  char *pcVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  
  uVar6 = 0xff;
  if (*(char *)(DAT_0800c0bc + 0xe) == '\0') {
    uVar4 = 0;
    do {
      if (*(int *)(DAT_0800c0c0 + uVar4 * 0x74) == *(int *)(param_1 + 0xc)) {
        uVar6 = uVar4;
        break;
      }
      uVar4 = uVar4 + 1 & 0xff;
    } while (uVar4 < 0x10);
    if (uVar6 < 0x10) {
      if (*(char *)(param_1 + 8) == '\a') {
        FUN_0802e34c(*(undefined4 *)(param_1 + 4),8);
        FUN_0802b8b2(*(undefined4 *)(DAT_0800c0bc + 0x94),0);
        return;
      }
      if (*(char *)(param_1 + 8) == '\r') {
        pcVar3 = (char *)FUN_0802ad6c();
        if (*pcVar3 == '\x14') {
          iVar5 = DAT_0800c0c8 + uVar6 * 6;
          bVar1 = *(byte *)(iVar5 + 4);
          if (10 < (bVar1 & 0x7f)) {
            if (*DAT_0800c0c4 == '\0') {
              *(byte *)(iVar5 + 4) = bVar1 - 1;
              return;
            }
            if ((bVar1 & 0x7f) < 0x15) {
              cVar2 = -0x76;
            }
            else {
              cVar2 = bVar1 - 10;
            }
            *(char *)(iVar5 + 4) = cVar2;
          }
        }
        else if (*pcVar3 == '\x13') {
          iVar5 = DAT_0800c0c8 + uVar6 * 6;
          bVar1 = *(byte *)(iVar5 + 4);
          if ((bVar1 & 0x7f) < 100) {
            if (*DAT_0800c0c4 == '\0') {
              *(byte *)(iVar5 + 4) = bVar1 + 1;
              return;
            }
            if ((bVar1 & 0x7f) < 0x5a) {
              cVar2 = bVar1 + 10;
            }
            else {
              cVar2 = -0x1c;
            }
            *(char *)(iVar5 + 4) = cVar2;
            return;
          }
        }
      }
    }
  }
  return;
}

