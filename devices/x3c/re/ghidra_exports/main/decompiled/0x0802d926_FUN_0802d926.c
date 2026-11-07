// 0x0802d926 FUN_0802d926 size=84

void FUN_0802d926(undefined4 *param_1,uint param_2)

{
  int iVar1;
  
  if (((uint)param_1 & 3) != 0) {
    iVar1 = 4 - ((uint)param_1 & 3);
    for (; (iVar1 != 0 && (param_2 != 0)); param_2 = param_2 - 1) {
      *(undefined1 *)param_1 = 0xff;
      iVar1 = iVar1 + -1;
      param_1 = (undefined4 *)((int)param_1 + 1);
    }
  }
  for (; 0x20 < param_2; param_2 = param_2 - 0x20) {
    *param_1 = 0xffffffff;
    param_1[1] = 0xffffffff;
    param_1[2] = 0xffffffff;
    param_1[3] = 0xffffffff;
    param_1[4] = 0xffffffff;
    param_1[5] = 0xffffffff;
    param_1[6] = 0xffffffff;
    param_1[7] = 0xffffffff;
    param_1 = param_1 + 8;
  }
  for (; 4 < param_2; param_2 = param_2 - 4) {
    *param_1 = 0xffffffff;
    param_1 = param_1 + 1;
  }
  for (; param_2 != 0; param_2 = param_2 - 1) {
    *(undefined1 *)param_1 = 0xff;
    param_1 = (undefined4 *)((int)param_1 + 1);
  }
  return;
}

