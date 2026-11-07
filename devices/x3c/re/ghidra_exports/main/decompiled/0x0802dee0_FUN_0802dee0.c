// 0x0802dee0 FUN_0802dee0 size=58

void FUN_0802dee0(int param_1)

{
  byte *pbVar1;
  int iVar2;
  
  if (*(int *)(param_1 + 8) == 0) {
    *(int *)(DAT_0802df1c + 4) = *(int *)(DAT_0802df1c + 4) + 1;
    iVar2 = FUN_0802d5c4(0x24);
    *(int *)(param_1 + 8) = iVar2;
    if (iVar2 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    FUN_0802d8d6(iVar2,0x24);
    *(undefined1 *)(*(int *)(param_1 + 8) + 0x21) = 0xf;
    pbVar1 = (byte *)(*(int *)(param_1 + 8) + 0x20);
    *pbVar1 = *pbVar1 | 3;
  }
  return;
}

