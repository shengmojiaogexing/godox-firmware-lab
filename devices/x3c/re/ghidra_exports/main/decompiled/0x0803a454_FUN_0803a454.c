// 0x0803a454 FUN_0803a454 size=80

void FUN_0803a454(int param_1)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = 0;
  do {
    iVar2 = param_1 + uVar1 * 0x2c;
    *(char *)(iVar2 + 0xc) = (char)uVar1;
    *(char *)(iVar2 + 0xd) = (char)uVar1;
    *(undefined1 *)(iVar2 + 0xe) = 0;
    *(undefined4 *)(iVar2 + 0x14) = 0;
    *(undefined4 *)(iVar2 + 0x20) = 0;
    *(undefined4 *)(iVar2 + 0x24) = 0;
    uVar1 = uVar1 + 1 & 0xff;
  } while (uVar1 < 8);
  uVar1 = 0;
  do {
    iVar2 = param_1 + uVar1 * 0x2c;
    *(char *)(iVar2 + 0x16c) = (char)uVar1;
    *(char *)(iVar2 + 0x16d) = (char)uVar1;
    *(undefined1 *)(iVar2 + 0x16e) = 1;
    *(undefined4 *)(iVar2 + 0x174) = 0;
    *(undefined4 *)(iVar2 + 0x180) = 0;
    *(undefined4 *)(iVar2 + 0x184) = 0;
    uVar1 = uVar1 + 1 & 0xff;
  } while (uVar1 < 8);
  return;
}

