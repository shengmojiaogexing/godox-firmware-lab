// 0x08002f84 FUN_08002f84 size=50

void FUN_08002f84(int param_1,uint param_2,int param_3)

{
  int iVar1;
  
  if (param_2 < 8) {
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) & ~(0xf << (param_2 << 2 & 0xff));
    *(uint *)(param_1 + 0x20) = *(uint *)(param_1 + 0x20) | param_3 << (param_2 << 2 & 0xff);
    return;
  }
  iVar1 = (param_2 & 7) << 2;
  *(uint *)(param_1 + 0x24) = *(uint *)(param_1 + 0x24) & ~(0xf << iVar1);
  *(uint *)(param_1 + 0x24) = *(uint *)(param_1 + 0x24) | param_3 << iVar1;
  return;
}

