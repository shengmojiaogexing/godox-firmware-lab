// 0x08006064 FUN_08006064 size=62

void FUN_08006064(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = DAT_080060a4;
  iVar2 = FUN_080303b8(*(undefined4 *)(DAT_080060a4 + 0x38));
  if (iVar2 == 0) {
    FUN_0803205a();
    uVar3 = FUN_0802e40c();
    *(undefined4 *)(iVar1 + 0x38) = uVar3;
    FUN_08030e2a(uVar3,0,DAT_080060a8);
    FUN_080313b2(*(undefined4 *)(iVar1 + 0x38),0x170,0x1c0);
    FUN_08031318(*(undefined4 *)(iVar1 + 0x38),0);
    return;
  }
  return;
}

