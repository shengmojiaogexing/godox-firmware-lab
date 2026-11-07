// 0x0801708c FUN_0801708c size=68

void FUN_0801708c(void)

{
  uint *puVar1;
  
  puVar1 = DAT_080170d0;
  *DAT_080170d0 = *DAT_080170d0 | 0x1000;
  disableIRQinterrupts();
  FUN_08010b6c();
  FUN_08023536(DAT_080170d4,4);
  FUN_08023520(0x35);
  thunk_FUN_0801c118(600);
  FUN_08023520(0x3b);
  FUN_08023520(0x36);
  FUN_08010b24();
  enableIRQinterrupts();
  *puVar1 = *puVar1 & 0xffffefff;
  return;
}

