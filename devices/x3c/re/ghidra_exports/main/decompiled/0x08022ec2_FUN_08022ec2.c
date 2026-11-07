// 0x08022ec2 FUN_08022ec2 size=48

void FUN_08022ec2(int param_1,uint param_2,int param_3)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = (param_2 & 7) << 2;
  uVar2 = param_3 << iVar3;
  uVar1 = 0xf << iVar3;
  if (param_2 >> 3 == 0) {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & ~uVar1;
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | uVar2;
    return;
  }
  *(uint *)(param_1 + 0x24) = *(uint *)(param_1 + 0x24) & ~uVar1;
  *(uint *)(param_1 + 0x24) = *(uint *)(param_1 + 0x24) | uVar2;
  return;
}

