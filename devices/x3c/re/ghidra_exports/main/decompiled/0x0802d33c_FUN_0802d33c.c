// 0x0802d33c FUN_0802d33c size=130

void FUN_0802d33c(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  
  FUN_080302d0(param_1);
  if (param_2 == 0) {
    param_2 = *(int *)(param_1 + 0x2c);
  }
  if ((*(int *)(param_1 + 0x2c) == param_2) && (-1 < (int)((uint)*(byte *)(param_1 + 0x54) << 0x1c))
     ) {
    uVar1 = FUN_080185a0(param_2);
    iVar2 = FUN_0802d72c(*(undefined4 *)(param_1 + 0x2c),uVar1);
    *(int *)(param_1 + 0x2c) = iVar2;
    if (iVar2 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    FUN_08018614(iVar2,iVar2);
    if (*(int *)(param_1 + 0x2c) == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  }
  else {
    if ((*(int *)(param_1 + 0x2c) != 0) && (-1 < (int)((uint)*(byte *)(param_1 + 0x54) << 0x1c))) {
      FUN_0802d6e4();
      *(undefined4 *)(param_1 + 0x2c) = 0;
    }
    FUN_080185a0(param_2);
    iVar2 = FUN_0802d5c4();
    *(int *)(param_1 + 0x2c) = iVar2;
    if (iVar2 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    FUN_08018614(param_2,iVar2);
    *(byte *)(param_1 + 0x54) = *(byte *)(param_1 + 0x54) & 0xf7;
  }
  FUN_0802cd80(param_1);
  return;
}

