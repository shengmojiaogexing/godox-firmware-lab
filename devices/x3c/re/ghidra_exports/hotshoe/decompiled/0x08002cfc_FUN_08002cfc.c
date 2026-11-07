// 0x08002cfc FUN_08002cfc size=24

undefined4 FUN_08002cfc(uint param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (((DAT_08002d14[5] & param_1) != 0) && ((*DAT_08002d14 & param_1) != 0)) {
    uVar1 = 1;
  }
  return uVar1;
}

