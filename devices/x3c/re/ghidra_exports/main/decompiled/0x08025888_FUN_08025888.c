// 0x08025888 FUN_08025888 size=112

void FUN_08025888(int param_1,int param_2,uint param_3,uint param_4)

{
  int iVar1;
  int iVar2;
  uint local_20;
  uint local_1c;
  
  local_20 = param_3;
  local_1c = param_4;
  if ((param_4 & 1) == 0) {
    iVar2 = 0;
  }
  else {
    iVar2 = 1;
    iVar1 = FUN_080174c4(param_3 + 0x10,param_1 + 0x14,0);
    if (iVar1 != 0) {
      local_20 = local_20 & 0xffffff00;
      local_1c = param_3 + 0x10;
      FUN_0802ad74(param_1,0x12,&local_20);
      if ((char)local_20 == '\0') {
        iVar2 = 0;
      }
    }
    if (iVar2 != 0) {
      *(int *)(param_3 + 0x1c) = *(int *)(param_3 + 0x14) + *(int *)(param_3 + 0x20) + -1;
    }
  }
  if (*(int *)(param_3 + 0xc) < *(int *)(param_3 + 0x1c)) {
    *(int *)(param_3 + 0x1c) = *(int *)(param_3 + 0xc);
  }
  if (*(code **)(param_2 + 0x40) == (code *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x08028d5a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_2 + 0x40))(param_2,param_3,iVar2);
  return;
}

