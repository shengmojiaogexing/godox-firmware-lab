// 0x08006e5c FUN_08006e5c size=162

void FUN_08006e5c(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  byte bVar4;
  
  uVar1 = DAT_08006f00;
  iVar2 = FUN_08022eb4(DAT_08006f00,0x100);
  bVar4 = iVar2 == 1;
  iVar3 = FUN_08022eb4(uVar1,0x200);
  iVar2 = DAT_08006f04;
  if (iVar3 == 1) {
    bVar4 = bVar4 | 2;
  }
  if (bVar4 == 0) {
    if (*(char *)(DAT_08006f04 + 0x11) == '\r') {
      *(char *)(DAT_08006f04 + 0xb) = *(char *)(DAT_08006f04 + 0xb) + '\x01';
    }
    else if (*(char *)(DAT_08006f04 + 0x11) == '\x0e') {
      *(char *)(DAT_08006f04 + 0xb) = *(char *)(DAT_08006f04 + 0xb) + -1;
    }
    *(undefined1 *)(iVar2 + 0x11) = 0;
    *(undefined1 *)(iVar2 + 0x10) = 0;
  }
  else if (bVar4 == 1) {
    if ((*(byte *)(DAT_08006f04 + 0x10) & 3) != 1) {
      bVar4 = *(byte *)(DAT_08006f04 + 0x10) & 0xfc | 1;
      *(byte *)(DAT_08006f04 + 0x10) = bVar4;
      *(byte *)(iVar2 + 0x11) = bVar4;
    }
  }
  else if (bVar4 == 2) {
    if ((*(byte *)(DAT_08006f04 + 0x10) & 3) != 2) {
      bVar4 = *(byte *)(DAT_08006f04 + 0x10) & 0xfc | 2;
      *(byte *)(DAT_08006f04 + 0x10) = bVar4;
      *(byte *)(iVar2 + 0x11) = bVar4;
    }
  }
  else if (bVar4 == 3) {
    *(undefined1 *)(DAT_08006f04 + 0x11) = 0;
    *(undefined1 *)(iVar2 + 0x10) = 0xc;
  }
  if (*(char *)(iVar2 + 0xb) != '\0') {
    *DAT_08006f08 = 2000;
  }
  return;
}

