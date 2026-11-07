// 0x08036974 FUN_08036974 size=72

void FUN_08036974(int param_1,int param_2,uint param_3,uint param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = *(int *)(param_2 + 0xc);
  iVar2 = *(int *)(param_2 + 8);
  if (iVar3 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  if (iVar2 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  *(int *)(iVar2 + 0xc) = iVar3;
  *(int *)(iVar3 + 8) = iVar2;
  iVar3 = param_1 + param_3 * 0x80 + param_4 * 4;
  if (((*(int *)(iVar3 + 0x40) == param_2) && (*(int *)(iVar3 + 0x40) = iVar2, iVar2 == param_1)) &&
     (iVar2 = param_1 + param_3 * 4, uVar1 = *(uint *)(iVar2 + 0x14) & ~(1 << (param_4 & 0xff)),
     *(uint *)(iVar2 + 0x14) = uVar1, uVar1 == 0)) {
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & ~(1 << (param_3 & 0xff));
  }
  return;
}

