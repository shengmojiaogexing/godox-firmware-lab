// 0x0802a4c2 FUN_0802a4c2 size=252

undefined4 * FUN_0802a4c2(int *param_1,undefined4 *param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  int *piVar4;
  
  if ((param_3 & 1) == 0) {
    if ((int)(param_3 << 0x1e) < 0) {
      param_2[0xf] = (param_2[0xf] & 0x80000000) + 0x6000;
      iVar1 = FUN_080274b0(param_2);
      if ((uint)(iVar1 * 2) < (param_2[0xf] & 0x7fffffff)) {
        param_2[0xf] = param_2[0xf] & 0x80000000 | iVar1 * 2 & 0x7fffffffU;
      }
      iVar1 = FUN_0802d5c4(param_2[0xf] & 0x7fffffff);
      param_2[10] = iVar1;
      if (iVar1 == 0) {
        uVar2 = (param_2[0xf] & 0x80000000) + 0xc00;
        param_2[0xf] = uVar2;
        iVar1 = FUN_0802d5c4(uVar2 & 0x7fffffff);
        param_2[10] = iVar1;
        if (iVar1 == 0) {
          return (undefined4 *)0x0;
        }
      }
      param_2[4] = *param_2;
      param_2[5] = param_2[1];
      param_2[6] = param_2[2];
      param_2[7] = param_2[3];
      param_2[7] = param_2[1];
      iVar1 = (param_2[6] - param_2[4]) + 1;
      param_2[8] = ((int)(param_2[0xf] & 0x7fffffff) / iVar1) / 3;
      param_2[9] = (uint)((int)(param_2[0xf] & 0x7fffffff) / iVar1) >> 1;
    }
    else {
      param_2[4] = *param_2;
      param_2[5] = param_2[1];
      param_2[6] = param_2[2];
      param_2[7] = param_2[3];
      uVar2 = FUN_080274b0(param_2);
      uVar2 = (uVar2 & 0x3fffffff) << 1;
      param_2[0xf] = param_2[0xf] & 0x80000000 | uVar2;
      uVar3 = FUN_0802d5c4(uVar2);
      param_2[10] = uVar3;
      FUN_0802d8d6(uVar3,param_2[0xf] & 0x7fffffff);
      param_2[0xf] = param_2[0xf] & 0x7fffffff | param_3 << 0x1f;
      if (param_2[10] == 0) {
        return (undefined4 *)0x0;
      }
      *param_1 = param_2[10];
      param_1[1] = (int)(param_2 + 4);
      param_1[2] = (int)(param_2 + 4);
      piVar4 = (int *)FUN_08018528();
      *(uint *)(*piVar4 + 0x1c) = *(uint *)(*piVar4 + 0x1c) & 0xffffffbf | (param_3 & 1) << 6;
    }
  }
  else {
    param_2 = (undefined4 *)0x0;
  }
  return param_2;
}

