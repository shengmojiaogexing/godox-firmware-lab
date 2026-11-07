// 0x0803a700 FUN_0803a700 size=50

undefined4 FUN_0803a700(int param_1)

{
  bool bVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = DAT_0803a734;
  uVar3 = 0;
  do {
    if (*(int *)(param_1 + 0x10) < 0) break;
    bVar1 = uVar3 <= DAT_0803a734;
    uVar3 = uVar3 + 1;
  } while (bVar1);
  *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) | 1;
  uVar3 = 0;
  do {
    if ((*(uint *)(param_1 + 0x10) & 1) == 0) {
      return 1;
    }
    bVar1 = uVar3 <= uVar2;
    uVar3 = uVar3 + 1;
  } while (bVar1);
  return 1;
}

