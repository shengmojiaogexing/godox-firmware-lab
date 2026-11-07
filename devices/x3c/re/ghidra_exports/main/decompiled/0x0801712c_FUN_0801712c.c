// 0x0801712c FUN_0801712c size=74

void FUN_0801712c(void)

{
  uint *puVar1;
  
  puVar1 = DAT_08017178;
  *DAT_08017178 = *DAT_08017178 | 0x1000;
  disableIRQinterrupts();
  FUN_08010b6c();
  thunk_FUN_0801c118(0x28);
  FUN_08023536(DAT_0801717c,2);
  FUN_08023520(0x35);
  thunk_FUN_0801c118(500);
  FUN_08023520(0x3b);
  FUN_08023520(0x36);
  FUN_08010b24();
  enableIRQinterrupts();
  *puVar1 = *puVar1 & 0xffffefff;
  return;
}

