// 0x080278e8 FUN_080278e8 size=82

int FUN_080278e8(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  iVar1 = 0x400 - param_1;
  uVar2 = (uint)(iVar1 * iVar1) >> 10;
  uVar3 = (uint)(param_1 * param_1) >> 10;
  return (param_3 * param_1 * uVar2 * 3 >> 0x14) + ((uVar2 * iVar1 >> 10) * param_2 >> 10) +
         (param_4 * uVar3 * iVar1 * 3 >> 0x14) + ((uVar3 * param_1 >> 10) * param_5 >> 10);
}

