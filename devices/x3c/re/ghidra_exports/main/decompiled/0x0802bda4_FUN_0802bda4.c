// 0x0802bda4 FUN_0802bda4 size=98

undefined4 FUN_0802bda4(int param_1,int param_2,int param_3,int param_4,undefined4 param_5)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  int local_28;
  
  uVar3 = *(undefined4 *)(param_1 + 0x24);
  local_28 = param_4;
  uVar1 = FUN_0802b904(*(byte *)(param_1 + 0x14) & 0x1f);
  iVar2 = FUN_0802b290(uVar3,((int)(uVar1 * (param_3 * ((*(uint *)(param_1 + 0x14) & 0x1fffff) >> 10
                                                       ) + param_2)) >> 3) + 4,0);
  if (iVar2 == 0) {
    param_4 = param_4 * (uVar1 >> 3);
    local_28 = 0;
    iVar2 = FUN_0802b10a(uVar3,param_5,param_4,&local_28);
    if ((iVar2 == 0) && (param_4 == local_28)) {
      uVar3 = 1;
    }
    else {
      uVar3 = 0;
    }
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}

