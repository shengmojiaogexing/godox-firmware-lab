// 0x0802eb88 FUN_0802eb88 size=64

bool FUN_0802eb88(undefined4 *param_1,int param_2)

{
  if (param_1 == (undefined4 *)0x0) {
    param_1 = (undefined4 *)**(undefined4 **)(param_2 + 4);
  }
  else {
    param_1 = (undefined4 *)*param_1;
  }
  for (; (param_1 != (undefined4 *)0x0 && (param_1[4] == 0)); param_1 = (undefined4 *)*param_1) {
  }
  if (param_1 == (undefined4 *)0x0) {
    return true;
  }
  if (param_1[4] == 0) {
    return true;
  }
  *(undefined4 *)(param_2 + 0xc) = 0;
  (*(code *)param_1[4])(param_1,param_2);
  return (*(byte *)(param_2 + 0x18) & 1) == 0;
}

