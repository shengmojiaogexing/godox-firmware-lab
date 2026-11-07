// 0x0800510c FUN_0800510c size=76

void FUN_0800510c(void)

{
  undefined4 uVar1;
  undefined2 *puVar2;
  undefined2 uVar3;
  int iVar4;
  
  uVar1 = DAT_08005158;
  iVar4 = FUN_08018c16(DAT_08005158,0x20);
  if (iVar4 != 0) {
    FUN_08018c10(uVar1,0x20);
    *DAT_0800515c = *DAT_0800515c + 1;
  }
  iVar4 = FUN_08018c16(uVar1,4);
  if (iVar4 != 0) {
    FUN_08018c10(uVar1,4);
    uVar3 = FUN_08018dda(uVar1,0);
    puVar2 = DAT_08005160;
    *DAT_08005160 = uVar3;
    uVar3 = FUN_08018dda(uVar1,1);
    puVar2[1] = uVar3;
  }
  return;
}

