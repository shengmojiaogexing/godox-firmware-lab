// 0x08017a4c FUN_08017a4c size=60

void FUN_08017a4c(void)

{
  int iVar1;
  undefined4 extraout_r2;
  undefined4 extraout_r2_00;
  int extraout_r2_01;
  
  FUN_08017f7c(DAT_08017a84,0x14);
  iVar1 = FUN_0802c03c();
  if (iVar1 != 0) {
    FUN_0802c1ae(iVar1,DAT_08017a88);
    FUN_0802c1b2(extraout_r2,DAT_08017a8c);
    FUN_0802c1b6(extraout_r2_00,DAT_08017a90);
    *(undefined4 *)(extraout_r2_01 + 0xc) = DAT_08017a94;
    return;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

