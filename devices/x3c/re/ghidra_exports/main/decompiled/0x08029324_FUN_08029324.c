// 0x08029324 FUN_08029324 size=48

void FUN_08029324(undefined4 *param_1,undefined4 param_2,undefined1 param_3,undefined4 param_4,
                 undefined1 param_5,undefined4 param_6)

{
  FUN_08027430(param_1 + 2);
  *(undefined1 *)(param_1 + 8) = param_3;
  *(undefined1 *)((int)param_1 + 0x21) = param_5;
  param_1[6] = param_4;
  param_1[7] = param_6;
  *param_1 = DAT_08029354;
  *(undefined1 *)(param_1 + 1) = 3;
  return;
}

