// 0x0802ba14 FUN_0802ba14 size=180

undefined4 FUN_0802ba14(int param_1,uint *param_2,uint *param_3,undefined4 param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  uint uVar5;
  int local_20;
  uint *puStack_1c;
  uint *puStack_18;
  undefined4 uStack_14;
  
  local_20 = param_1;
  puStack_1c = param_2;
  puStack_18 = param_3;
  uStack_14 = param_4;
  iVar2 = FUN_0802c55a(param_2);
  if (iVar2 == 0) {
    if (0x10 < (*param_2 & 0x1f) - 4) {
      return 0;
    }
    uVar3 = *param_3;
    uVar1 = (*param_2 >> 10 & 0x7ff) << 10;
    *param_3 = uVar3 & 0xffe003ff | uVar1;
    uVar5 = *param_2;
    *param_3 = uVar3 & 0x3ff | uVar1 | uVar5 & 0xffe00000;
    *param_3 = uVar3 & 0x3e0 | uVar1 | uVar5 & 0xffe00000 | (byte)*param_2 & 0x1f;
  }
  else if (iVar2 == 1) {
    uVar4 = FUN_0802b04c(param_2);
    iVar2 = FUN_08004ee8(uVar4,&LAB_0802bac8);
    if (iVar2 != 0) {
      return 0;
    }
    iVar2 = FUN_0802b08c(&puStack_1c,param_2,2);
    if (iVar2 == 0) {
      iVar2 = FUN_0802b10a(&puStack_1c,param_3,4,&local_20);
      FUN_0802afdc(&puStack_1c);
      if ((iVar2 != 0) || (local_20 != 4)) {
        return 0;
      }
    }
    if (0x10 < ((byte)*param_3 & 0x1f) - 4) {
      return 0;
    }
  }
  else {
    if (iVar2 != 2) {
      return 0;
    }
    *param_3 = ((*param_3 & 0x3ff | 0x400) + 0x200000 & 0xffffffe0) + 0xb;
  }
  return 1;
}

