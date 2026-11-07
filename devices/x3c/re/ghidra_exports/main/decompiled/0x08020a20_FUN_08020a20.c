// 0x08020a20 FUN_08020a20 size=78

void FUN_08020a20(void)

{
  if (((*(ushort *)(DAT_08020a54 + 0x10) & 7) == 2) && ((*(ushort *)(DAT_08020a54 + 2) & 0x60) == 0)
     ) {
    return;
  }
  FUN_080218e8();
  FUN_08021908();
  FUN_08021724(2);
  FUN_080218a4(0xf);
  DataSynchronizationBarrier(0xf);
  *DAT_08035d30 = *DAT_08035d30 & 0x700 | DAT_08035d34;
  DataSynchronizationBarrier(0xf);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

