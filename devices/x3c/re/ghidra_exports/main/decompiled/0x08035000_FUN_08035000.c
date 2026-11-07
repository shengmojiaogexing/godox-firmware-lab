// 0x08035000 FUN_08035000 size=46

void FUN_08035000(uint param_1,int *param_2,uint *param_3)

{
  int iVar1;
  uint uVar2;
  
  if (param_1 < 0x80) {
    iVar1 = 0;
    uVar2 = (int)(param_1 + ((uint)((int)param_1 >> 0x1f) >> 0x1e)) >> 2;
  }
  else {
    iVar1 = FUN_080398a6(param_1);
    uVar2 = param_1 >> (iVar1 - 5U & 0xff) ^ 0x20;
    iVar1 = iVar1 + -6;
  }
  *param_2 = iVar1;
  *param_3 = uVar2;
  return;
}

