// 0x08022480 FUN_08022480 size=88

uint FUN_08022480(int *param_1,uint param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 local_20;
  int local_1c;
  undefined4 local_18;
  int local_14;
  
  iVar1 = DAT_080224d8;
  param_2 = *(uint *)(*(int *)(*param_1 + 0x18) + 0xc) / param_2;
  if ((int)param_3 < (int)param_2) {
    param_2 = param_3;
  }
  if (*(int *)(**(int **)(DAT_080224d8 + 4) + 0x24) != 0) {
    local_20 = 0;
    local_18 = 0;
    local_1c = 0;
    uVar2 = param_2;
    do {
      local_14 = uVar2 - 1;
      (**(code **)(**(int **)(iVar1 + 4) + 0x24))(**(int **)(iVar1 + 4),&local_20);
      if ((local_14 - local_1c) + 1 <= (int)param_2) break;
      uVar2 = uVar2 - 1;
    } while (0 < (int)uVar2);
    if ((int)uVar2 < 1) {
      return 0;
    }
    param_2 = local_14 + 1;
  }
  return param_2;
}

