// 0x080083ec FUN_080083ec size=138

void FUN_080083ec(void)

{
  int iVar1;
  undefined4 extraout_r2;
  undefined4 extraout_r3;
  undefined4 unaff_r4;
  undefined4 unaff_lr;
  
  iVar1 = FUN_08020984(0x400);
  if (iVar1 != 0) {
    FUN_08020978(0x400);
    *DAT_08012ac8 = 1;
    FUN_080380c4();
    iVar1 = FUN_08022eb4(DAT_08012acc,0x400,extraout_r2,extraout_r3,unaff_r4,unaff_lr);
    if (iVar1 == 0) {
      FUN_08038400(DAT_08012ad0,0);
      iVar1 = DAT_08012ad4;
      *(undefined1 *)(DAT_08012ad4 + 2) = 0;
      *(undefined1 *)(iVar1 + 4) = 0;
    }
    return;
  }
  iVar1 = FUN_08020984(0x2000);
  if (iVar1 != 0) {
    FUN_08020978(0x2000);
    return;
  }
  iVar1 = FUN_08020984(0x8000);
  if (iVar1 == 0) {
    return;
  }
  *DAT_0800844c = 1;
  if (*DAT_08008450 != '\0') {
    *DAT_08008454 = 100;
  }
  FUN_08020978(0x8000);
  return;
}

