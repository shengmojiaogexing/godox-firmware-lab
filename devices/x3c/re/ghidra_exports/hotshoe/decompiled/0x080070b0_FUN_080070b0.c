// 0x080070b0 FUN_080070b0 size=20

void FUN_080070b0(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = DAT_080070c8;
  iVar1 = DAT_080070c4;
  uVar3 = 0;
  do {
    *(undefined1 *)(iVar2 + uVar3) = *(undefined1 *)(iVar1 + uVar3);
    uVar3 = uVar3 + 1 & 0xff;
  } while (uVar3 < 0x19);
  return;
}

