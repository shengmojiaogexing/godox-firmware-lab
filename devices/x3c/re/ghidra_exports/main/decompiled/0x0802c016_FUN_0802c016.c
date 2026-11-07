// 0x0802c016 FUN_0802c016 size=36

void FUN_0802c016(int *param_1)

{
  int iVar1;
  
  iVar1 = *param_1;
  if (iVar1 != 0) {
    if (*(code **)(iVar1 + 0xc) != (code *)0x0) {
      (**(code **)(iVar1 + 0xc))(iVar1,param_1);
    }
    if ((char)param_1[4] == '\x01') {
      FUN_0802d6e4(param_1[1]);
      param_1[1] = 0;
    }
  }
  return;
}

