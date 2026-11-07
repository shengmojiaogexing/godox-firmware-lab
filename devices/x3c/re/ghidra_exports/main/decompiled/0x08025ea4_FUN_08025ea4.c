// 0x08025ea4 FUN_08025ea4 size=112

void FUN_08025ea4(int param_1)

{
  int iVar1;
  byte *pbVar2;
  byte bVar3;
  uint uVar4;
  
  iVar1 = DAT_08025f14;
  if (*(char *)(param_1 + 8) == '\x1c') {
    if (*(char *)(DAT_08025f14 + 0xb) == '\0') {
      bVar3 = FUN_08031e48(*(undefined4 *)(param_1 + 4));
      pbVar2 = DAT_08025f18;
      if (bVar3 < 2) {
        if ((((*DAT_08025f18 == 0) && (bVar3 == 1)) && (*DAT_08025f1c == '\x02')) &&
           (*DAT_08025f20 != '\0')) {
          *(undefined1 *)(iVar1 + 0xc) = 1;
        }
        *pbVar2 = bVar3;
        if (bVar3 == 1) {
          *DAT_08025f24 = 0;
          *DAT_08025f28 = *DAT_08025f28 & 0x7f;
          iVar1 = DAT_08025f2c;
          uVar4 = 0;
          do {
            if (*(char *)(iVar1 + uVar4 * 6) == '\x01') {
              *(undefined1 *)(iVar1 + uVar4 * 6) = 0;
            }
            uVar4 = uVar4 + 1 & 0xff;
          } while (uVar4 < 5);
          return;
        }
        *(undefined1 *)(iVar1 + 0xc) = 0;
      }
    }
  }
  return;
}

