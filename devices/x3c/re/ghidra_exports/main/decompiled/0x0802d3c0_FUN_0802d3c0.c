// 0x0802d3c0 FUN_0802d3c0 size=54

int FUN_0802d3c0(undefined4 param_1,undefined4 param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = DAT_0802d3f8;
  iVar2 = *(int *)(DAT_0802d3f8 + 4) + 1;
  *(int *)(DAT_0802d3f8 + 4) = iVar2;
  piVar1 = DAT_0802d3fc;
  iVar2 = FUN_0802d72c(*DAT_0802d3fc,iVar2 * 8);
  *piVar1 = iVar2;
  if (iVar2 != 0) {
    iVar3 = *(int *)(iVar3 + 4);
    iVar4 = iVar3 + -1;
    *(undefined4 *)(iVar2 + iVar4 * 8) = param_1;
    *(undefined4 *)(iVar2 + iVar4 * 8 + 4) = param_2;
    return iVar3;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

