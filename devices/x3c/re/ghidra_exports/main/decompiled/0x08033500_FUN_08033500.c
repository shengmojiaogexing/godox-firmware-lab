// 0x08033500 FUN_08033500 size=56

undefined4 * FUN_08033500(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  puVar1 = (undefined4 *)FUN_08017f8c(DAT_08033538);
  if (puVar1 != (undefined4 *)0x0) {
    *puVar1 = param_2;
    puVar1[2] = param_1;
    puVar1[4] = 0xffffffff;
    puVar1[5] = 0;
    uVar2 = FUN_080334d8();
    puVar1[1] = uVar2;
    puVar1[3] = param_3;
    *(undefined1 *)(DAT_0803353c + 4) = 1;
    return puVar1;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}

