// 0x08036804 FUN_08036804 size=64

void FUN_08036804(int param_1)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  
  uVar1 = FUN_0802ebf4(param_1);
  for (uVar2 = 0; uVar2 < uVar1; uVar2 = uVar2 + 1) {
    uVar3 = *(undefined4 *)(**(int **)(param_1 + 8) + uVar2 * 4);
    FUN_080302d0(uVar3);
    FUN_0802ad74(uVar3,0x2a,0);
    FUN_080302d0(uVar3);
    FUN_08036804(uVar3);
  }
  return;
}

