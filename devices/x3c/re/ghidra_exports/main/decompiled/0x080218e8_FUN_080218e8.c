// 0x080218e8 FUN_080218e8 size=20

void FUN_080218e8(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  
  puVar3 = DAT_08021900;
  uVar1 = DAT_080218fc;
  *DAT_08021900 = DAT_080218fc;
  uVar2 = DAT_08021904;
  *puVar3 = DAT_08021904;
  puVar3 = DAT_08021900 + 0x10;
  *puVar3 = uVar1;
  *puVar3 = uVar2;
  return;
}

