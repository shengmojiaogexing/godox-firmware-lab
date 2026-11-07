// 0x0800cba8 FUN_0800cba8 size=128

void FUN_0800cba8(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = DAT_0800cc28;
  FUN_08022e38(DAT_0800cc28,0x4000);
  FUN_080147f4(uVar1,0x4000,1,0);
  FUN_08022e34(uVar1,1);
  FUN_080147f4(uVar1,1,1,0);
  FUN_08022e34(uVar1,2);
  FUN_080147f4(uVar1,2,1,0);
  FUN_080147f4(uVar1,0x8000,0);
  uVar2 = DAT_0800cc2c;
  FUN_080147f4(DAT_0800cc2c,0x2000,0);
  FUN_08010b24();
  FUN_080147f4(uVar1,0x2000,1,0);
  FUN_080147f4(uVar2,0x100,1,0);
  return;
}

