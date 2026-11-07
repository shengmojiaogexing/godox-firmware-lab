// 0x08031a08 FUN_08031a08 size=48

undefined4 FUN_08031a08(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = (param_1[2] - *param_1) / 2;
  iVar3 = *param_2 - (*param_1 + iVar2);
  iVar1 = param_2[1] - (param_1[1] + iVar2);
  if ((uint)(iVar2 * iVar2) < (uint)(iVar1 * iVar1 + iVar3 * iVar3)) {
    return 0;
  }
  return 1;
}

