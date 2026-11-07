// 0x0802b608 FUN_0802b608 size=56

int FUN_0802b608(void)

{
  int iVar1;
  
  iVar1 = FUN_08017f8c(DAT_0802b640);
  if (iVar1 != 0) {
    FUN_08017f7c(iVar1,4);
    *(undefined4 *)(iVar1 + 0xc) = 0;
    *(undefined4 *)(iVar1 + 0x10) = 0;
    *(undefined4 *)(iVar1 + 0x14) = 0;
    *(byte *)(iVar1 + 0x1c) = *(byte *)(iVar1 + 0x1c) & 0xfc | 0xc;
    *(undefined4 *)(iVar1 + 0x18) = 0;
    return iVar1;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

