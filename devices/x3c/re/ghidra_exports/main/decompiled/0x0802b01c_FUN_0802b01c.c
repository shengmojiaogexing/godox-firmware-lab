// 0x0802b01c FUN_0802b01c size=42

undefined4 FUN_0802b01c(uint param_1)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)FUN_08017f30(DAT_0802b048);
  while( true ) {
    if (puVar1 == (undefined4 *)0x0) {
      return 0;
    }
    if (*(byte *)*puVar1 == param_1) break;
    puVar1 = (undefined4 *)FUN_08017f5e(DAT_0802b048);
  }
  return *puVar1;
}

