// 0x0801d0b0 FUN_0801d0b0 size=20

void FUN_0801d0b0(undefined2 *param_1,undefined2 *param_2,undefined2 *param_3,undefined2 *param_4)

{
  undefined2 uVar1;
  
  uVar1 = *param_1;
  *param_1 = *param_2;
  *param_2 = *param_3;
  *param_3 = *param_4;
  *param_4 = uVar1;
  return;
}

