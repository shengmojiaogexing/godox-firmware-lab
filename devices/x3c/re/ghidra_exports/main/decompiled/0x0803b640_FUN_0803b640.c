// 0x0803b640 FUN_0803b640 size=116

void FUN_0803b640(int *param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  iVar4 = *param_1;
  FUN_0803a6ec(iVar4,0x10,0);
  uVar2 = *(uint *)(iVar4 + 0x20);
  uVar3 = (uVar2 & 0x7fff) >> 4;
  uVar1 = (uVar2 & 0x1fffff) >> 0x11;
  uVar2 = uVar2 & 0xf;
  if (uVar1 == 2) {
    if (uVar3 != 0) {
      FUN_0803a852(iVar4,param_1[uVar2 * 0xb + 0x60],uVar2,uVar3);
      param_1[uVar2 * 0xb + 0x60] = param_1[uVar2 * 0xb + 0x60] + uVar3;
      param_1[uVar2 * 0xb + 0x62] = param_1[uVar2 * 0xb + 0x62] + uVar3;
    }
  }
  else if (uVar1 == 6) {
    FUN_0803a852(iVar4,param_1 + 0xb5,uVar2,uVar3);
    param_1[uVar2 * 0xb + 0x62] = param_1[uVar2 * 0xb + 0x62] + uVar3;
  }
  FUN_0803a6ec(iVar4,0x10,1);
  return;
}

