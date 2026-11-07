// 0x08031a84 FUN_08031a84 size=58

void FUN_08031a84(void)

{
  undefined1 *puVar1;
  int iVar2;
  undefined4 uVar3;
  
  FUN_0802c57e(DAT_08031ac0);
  puVar1 = DAT_08031ac0;
  *DAT_08031ac0 = 1;
  puVar1[0x1b] = 0x3c;
  *(undefined4 *)(puVar1 + 4) = DAT_08031ac4;
  uVar3 = FUN_0802c5ac();
  iVar2 = DAT_08031ac8;
  *(undefined4 *)(DAT_08031ac8 + 0x18) = uVar3;
  FUN_0802c57e(DAT_08031ac0 + 0x20);
  puVar1 = DAT_08031ac0;
  DAT_08031ac0[0x20] = 4;
  *(undefined4 *)(puVar1 + 0x24) = DAT_08031acc;
  uVar3 = FUN_0802c5ac();
  *(undefined4 *)(iVar2 + 0x1c) = uVar3;
  return;
}

