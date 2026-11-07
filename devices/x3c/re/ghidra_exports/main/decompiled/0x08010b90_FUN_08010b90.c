// 0x08010b90 FUN_08010b90 size=80

void FUN_08010b90(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  
  iVar2 = DAT_08010be4;
  iVar1 = DAT_08010be0;
  uVar3 = 0;
  do {
    iVar4 = iVar1 + uVar3 * 6;
    iVar5 = iVar2 + uVar3 * 6;
    *(undefined1 *)(iVar5 + 3) = *(undefined1 *)(iVar4 + 3);
    *(undefined1 *)(iVar5 + 2) = *(undefined1 *)(iVar4 + 2);
    *(undefined1 *)(iVar5 + 4) = *(undefined1 *)(iVar4 + 4);
    *(undefined1 *)(iVar2 + uVar3 * 6) = *(undefined1 *)(iVar1 + uVar3 * 6);
    *(undefined1 *)(iVar5 + 1) = *(undefined1 *)(iVar4 + 1);
    iVar4 = DAT_08010bec;
    uVar3 = uVar3 + 1 & 0xff;
  } while (uVar3 < 0x10);
  *(undefined1 *)(DAT_08010bec + 10) = *DAT_08010be8;
  *(undefined1 *)(iVar4 + 0xb) = *DAT_08010bf0;
  *(undefined1 *)(iVar4 + 0xc) = *DAT_08010bf4;
  *(undefined1 *)(iVar4 + 0xd) = *DAT_08010bf8;
  return;
}

