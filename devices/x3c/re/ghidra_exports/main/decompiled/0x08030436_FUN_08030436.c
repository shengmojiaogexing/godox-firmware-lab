// 0x08030436 FUN_08030436 size=42

void FUN_08030436(int param_1)

{
  int iVar1;
  
  *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) | 1;
  iVar1 = FUN_0802ee16();
  *(ushort *)(iVar1 + 0x2a) = *(ushort *)(iVar1 + 0x2a) | 2;
  iVar1 = FUN_0802ed10();
  if (*(int *)(iVar1 + 4) == 0) {
    return;
  }
  FUN_080336c4();
  return;
}

