// 0x08028dce FUN_08028dce size=62

void FUN_08028dce(undefined4 *param_1,int param_2)

{
  int *piVar1;
  
  FUN_0802acea(param_1);
  *param_1 = *(undefined4 *)(param_2 + 0x34);
  param_1[1] = *(undefined4 *)(param_2 + 0x30);
  param_1[2] = *(undefined4 *)(param_2 + 0x2c);
  piVar1 = (int *)FUN_08018528();
  *(uint *)(*piVar1 + 0x1c) =
       *(uint *)(*piVar1 + 0x1c) & 0xffffffbf | (*(byte *)(param_2 + 0x38) & 1) << 6;
  if ((code *)param_1[0x12] != (code *)0x0) {
    (*(code *)param_1[0x12])(param_1,param_2);
  }
  FUN_0802d6e4(param_2);
  return;
}

