// 0x0802ecd0 FUN_0802ecd0 size=52

int FUN_0802ecd0(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int extraout_r2;
  undefined8 uVar4;
  
  iVar1 = FUN_0802f938(param_1,0);
  iVar2 = FUN_0802f962(param_1,0);
  uVar4 = FUN_0802f864(param_1,0);
  iVar3 = FUN_0802fc5e(param_1,(int)((ulonglong)uVar4 >> 0x20),(int)uVar4);
  return ((iVar3 - iVar1) - iVar2) + extraout_r2 * -2;
}

