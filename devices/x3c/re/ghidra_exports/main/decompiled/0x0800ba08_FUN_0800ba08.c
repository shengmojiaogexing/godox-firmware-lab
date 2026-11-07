// 0x0800ba08 FUN_0800ba08 size=100

void FUN_0800ba08(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  iVar2 = DAT_0800ba48;
  iVar1 = DAT_0800ba44;
  uVar4 = 0;
  do {
    iVar5 = iVar1 + uVar4 * 6;
    iVar6 = iVar2 + uVar4 * 6;
    *(undefined1 *)(iVar6 + 3) = *(undefined1 *)(iVar5 + 3);
    *(undefined1 *)(iVar6 + 2) = *(undefined1 *)(iVar5 + 2);
    *(undefined1 *)(iVar6 + 4) = *(undefined1 *)(iVar5 + 4);
    *(undefined1 *)(iVar2 + uVar4 * 6) = *(undefined1 *)(iVar1 + uVar4 * 6);
    *(undefined1 *)(iVar6 + 1) = *(undefined1 *)(iVar5 + 1);
    uVar4 = uVar4 + 1 & 0xff;
  } while (uVar4 < 5);
  uVar3 = FUN_0802b608();
  iVar1 = DAT_0800aeb8;
  *(undefined4 *)(DAT_0800aeb8 + 0x14) = uVar3;
  FUN_0802c912(*DAT_0800aebc,uVar3);
  *(byte *)(*(int *)(iVar1 + 0x14) + 0x1c) = *(byte *)(*(int *)(iVar1 + 0x14) + 0x1c) & 0xf7;
  return;
}

