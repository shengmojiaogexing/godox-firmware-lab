// 0x0802d5c4 FUN_0802d5c4 size=46

int FUN_0802d5c4(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar1 = DAT_0802d5f4;
  if (param_1 == 0) {
    return DAT_0802d5f4;
  }
  iVar2 = FUN_080337dc(*(undefined4 *)(DAT_0802d5f4 + 4),param_1);
  if (iVar2 != 0) {
    uVar3 = *(int *)(iVar1 + 8) + param_1;
    *(uint *)(iVar1 + 8) = uVar3;
    if (uVar3 <= *(uint *)(iVar1 + 0xc)) {
      uVar3 = *(uint *)(iVar1 + 0xc);
    }
    *(uint *)(iVar1 + 0xc) = uVar3;
  }
  return iVar2;
}

