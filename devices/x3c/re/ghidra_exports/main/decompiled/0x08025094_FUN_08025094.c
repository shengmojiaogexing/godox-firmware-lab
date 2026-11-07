// 0x08025094 FUN_08025094 size=96

void FUN_08025094(int param_1,int param_2,uint param_3,uint param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = param_1 + param_3 * 0x80 + param_4 * 4;
  iVar1 = *(int *)(iVar4 + 0x40);
  if (iVar1 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  if (param_2 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  *(int *)(param_2 + 8) = iVar1;
  *(int *)(param_2 + 0xc) = param_1;
  *(int *)(iVar1 + 0xc) = param_2;
  iVar1 = FUN_080198c2(param_2);
  uVar2 = FUN_080198c2(param_2);
  iVar3 = FUN_08018e88(uVar2,4);
  if (iVar1 == iVar3) {
    *(int *)(iVar4 + 0x40) = param_2;
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) | 1 << (param_3 & 0xff);
    param_1 = param_1 + param_3 * 4;
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) | 1 << (param_4 & 0xff);
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

