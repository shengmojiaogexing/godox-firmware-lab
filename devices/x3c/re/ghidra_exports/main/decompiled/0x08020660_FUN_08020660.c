// 0x08020660 FUN_08020660 size=112

undefined4 FUN_08020660(void)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 extraout_r3;
  
  FUN_080207a4();
  puVar1 = DAT_080206d0;
  *DAT_080206d0 = 0;
  iVar2 = FUN_08020570();
  if (iVar2 == 1) {
    DAT_080206d0[-2] = extraout_r3;
    DAT_080206d0[-1] = 0x2101;
    *puVar1 = extraout_r3;
    puVar1 = DAT_080206d0;
    DAT_080206d0[2] = DAT_080206d4;
    puVar1[3] = 0xffff;
    puVar1[4] = extraout_r3;
    puVar1[5] = extraout_r3;
    puVar1[6] = extraout_r3;
    DAT_080206d0[9] = extraout_r3;
    puVar1 = DAT_080206d0;
    DAT_080206d0[0xd] = extraout_r3;
    puVar1[0xe] = extraout_r3;
    puVar1[0xf] = extraout_r3;
    puVar1[0x10] = extraout_r3;
    DAT_080206d0[1] = extraout_r3;
    FUN_08020774();
    FUN_080207b4();
    return 1;
  }
  return 0;
}

