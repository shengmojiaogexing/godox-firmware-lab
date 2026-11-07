// 0x0802d6e4 FUN_0802d6e4 size=40

void FUN_0802d6e4(int param_1)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = DAT_0802d70c;
  if ((param_1 != DAT_0802d70c) && (param_1 != 0)) {
    uVar2 = FUN_0803379a(*(undefined4 *)(DAT_0802d70c + 4),param_1);
    if (*(uint *)(iVar1 + 8) <= uVar2) {
      *(undefined4 *)(iVar1 + 8) = 0;
      return;
    }
    *(uint *)(iVar1 + 8) = *(uint *)(iVar1 + 8) - uVar2;
  }
  return;
}

