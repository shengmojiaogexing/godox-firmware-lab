// 0x08010344 FUN_08010344 size=114

void FUN_08010344(int param_1)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  int iVar4;
  undefined1 uVar5;
  undefined4 uVar6;
  
  iVar4 = DAT_080103c4;
  puVar3 = DAT_080103c0;
  puVar2 = DAT_080103bc;
  puVar1 = DAT_080103b8;
  if (*(char *)(param_1 + 8) == '\x1c') {
    uVar5 = FUN_08031e48();
    uVar6 = FUN_0800ad92(uVar5);
    *puVar1 = (char)uVar6;
    uVar5 = FUN_0801007c(*puVar3,*puVar2,uVar6);
    *puVar1 = uVar5;
    uVar6 = FUN_0800adf6();
    FUN_08031fba(*(undefined4 *)(iVar4 + 0x14),uVar6,1);
    return;
  }
  if (*(char *)(param_1 + 8) == '\r') {
    uVar6 = FUN_0800ad92(*(undefined1 *)(*(int *)(param_1 + 4) + 0x2e));
    *puVar1 = (char)uVar6;
    uVar5 = FUN_0801007c(*puVar3,*puVar2,uVar6);
    *puVar1 = uVar5;
    uVar6 = FUN_0800adf6();
    FUN_08031fba(*(undefined4 *)(iVar4 + 0x14),uVar6,1);
    return;
  }
  return;
}

