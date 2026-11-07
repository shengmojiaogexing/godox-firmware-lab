// 0x0802ed54 FUN_0802ed54 size=38

int FUN_0802ed54(int param_1,uint param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 8);
  if (iVar1 == 0) {
    return 0;
  }
  if (param_2 < (*(byte *)(iVar1 + 0x22) & 0x3f)) {
    return *(int *)(iVar1 + 0xc) + param_2 * 0xc;
  }
  return 0;
}

