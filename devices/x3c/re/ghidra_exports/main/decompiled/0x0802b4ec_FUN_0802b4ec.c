// 0x0802b4ec FUN_0802b4ec size=38

void FUN_0802b4ec(undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  piVar1 = DAT_0802b514;
  FUN_0802d6e4(*DAT_0802b514);
  iVar3 = FUN_0802d5c4(param_1);
  *piVar1 = iVar3;
  iVar2 = DAT_0802b518;
  *(int *)(DAT_0802b518 + 8) = iVar3;
  if (iVar3 != 0) {
    FUN_0802d8d6(iVar3,param_1);
    *(undefined4 *)(iVar2 + 4) = param_1;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

