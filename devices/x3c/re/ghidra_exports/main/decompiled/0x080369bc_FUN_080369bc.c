// 0x080369bc FUN_080369bc size=88

void FUN_080369bc(int param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = 0;
  do {
    if ((*(ushort *)(param_2 + 0x2a) & 0x1ff) >> 3 <= uVar1) {
LAB_080369f0:
      uVar1 = FUN_0802ebf4(param_2);
      for (uVar2 = 0; uVar2 < uVar1; uVar2 = uVar2 + 1) {
        FUN_080369bc(param_1,*(undefined4 *)(**(int **)(param_2 + 8) + uVar2 * 4));
      }
      return;
    }
    if ((param_1 == 0) || (*(int *)(*(int *)(param_2 + 0xc) + uVar1 * 8) == param_1)) {
      FUN_08030ca4(param_2,0xf0000,0xffff);
      goto LAB_080369f0;
    }
    uVar1 = uVar1 + 1;
  } while( true );
}

