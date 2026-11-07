// 0x08017a98 FUN_08017a98 size=70

void FUN_08017a98(int *param_1,int *param_2)

{
  char cVar1;
  code *UNRECOVERED_JUMPTABLE;
  
  FUN_0802d8d6(param_2,0x14);
  cVar1 = *(char *)*param_1;
  if (cVar1 == '\x01') {
    *param_2 = param_1[8];
    param_2[1] = param_1[9];
  }
  else if (cVar1 == '\x02') {
    param_2[2] = param_1[3];
  }
  else if (cVar1 == '\x04') {
    param_2[2] = 10;
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*param_1 + 4);
  if (UNRECOVERED_JUMPTABLE == (code *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x08017ada. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(*param_1,param_2);
  return;
}

