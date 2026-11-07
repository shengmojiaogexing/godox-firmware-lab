// 0x0801960c FUN_0801960c size=40

int FUN_0801960c(int param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = FUN_08019678(param_1);
  if (iVar1 == 0) {
    iVar1 = FUN_08019848(param_2);
    *(int *)(param_1 + 4) = iVar1 + 4 + *(int *)(param_1 + 4);
    FUN_08019690(param_1);
    return param_1;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

