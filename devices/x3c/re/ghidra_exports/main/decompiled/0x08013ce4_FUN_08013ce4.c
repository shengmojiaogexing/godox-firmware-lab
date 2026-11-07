// 0x08013ce4 FUN_08013ce4 size=130

void FUN_08013ce4(void)

{
  int iVar1;
  int *piVar2;
  undefined1 *puVar3;
  ushort uVar4;
  undefined4 uVar5;
  
  puVar3 = DAT_08013d70;
  piVar2 = DAT_08013d6c;
  iVar1 = DAT_08013d68;
  if (*(int *)(DAT_08013d68 + 0x2c) != *DAT_08013d6c) {
    if (*DAT_08013d6c == 0) {
      FUN_08006064();
      FUN_0801a234(0x24);
    }
    else if (*(int *)(DAT_08013d68 + 0x2c) == 0) {
      uVar5 = FUN_0803c384(*DAT_08013d70);
      FUN_0803c4a4(uVar5,1);
      uVar4 = FUN_0803c344();
      FUN_0801a234((uint)uVar4 % 0xff + 0x37 & 0xff);
      FUN_080061fc();
      *DAT_08013d78 = *DAT_08013d74;
    }
    if (*piVar2 != 0) {
      uVar5 = FUN_0803c384(*puVar3);
      FUN_0803c4a4(uVar5,1);
      uVar4 = FUN_0803c344();
      FUN_0801a234((uint)uVar4 % 0xff + 0x37 & 0xff);
    }
    *(int *)(iVar1 + 0x2c) = *piVar2;
  }
  return;
}

