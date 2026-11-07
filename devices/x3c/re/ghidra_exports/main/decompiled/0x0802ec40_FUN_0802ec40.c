// 0x0802ec40 FUN_0802ec40 size=92

void FUN_0802ec40(undefined4 param_1,int *param_2)

{
  int iVar1;
  
  iVar1 = FUN_0802f864(param_1,0);
  FUN_0802ed04(param_1,param_2);
  FUN_080274e4(param_2,-iVar1);
  iVar1 = FUN_0802f938(param_1,0);
  *param_2 = iVar1 + *param_2;
  iVar1 = FUN_0802f962(param_1,0);
  param_2[2] = param_2[2] - iVar1;
  iVar1 = FUN_0802f998(param_1,0);
  param_2[1] = iVar1 + param_2[1];
  iVar1 = FUN_0802f902(param_1,0);
  param_2[3] = param_2[3] - iVar1;
  return;
}

