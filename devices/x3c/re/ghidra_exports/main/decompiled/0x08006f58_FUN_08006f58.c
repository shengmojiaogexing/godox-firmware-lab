// 0x08006f58 FUN_08006f58 size=118

void FUN_08006f58(void)

{
  char *pcVar1;
  char cVar2;
  byte bVar3;
  
  bVar3 = 0;
  FUN_0800cba8();
  disableIRQinterrupts();
  FUN_0801c0fe(0x14);
  pcVar1 = DAT_08006fd0;
  do {
    FUN_08023494();
    FUN_080232bc();
    cVar2 = FUN_0802346c(0xd);
    *pcVar1 = cVar2;
    cVar2 = FUN_0802346c(0xe);
    pcVar1[1] = cVar2;
    thunk_FUN_0801c118(7000);
    bVar3 = bVar3 + 1;
    if (10 < bVar3) break;
  } while (*pcVar1 != '\\');
  FUN_08023540(2,6);
  FUN_08023520(0x36);
  FUN_08023520(0x33);
  thunk_FUN_0801c118(700);
  pcVar1 = DAT_08006fd0;
  DAT_08006fd0[-0x12] = '\0';
  pcVar1[-0x11] = '\0';
  FUN_080118e8(1,0x14,0);
  enableIRQinterrupts();
  return;
}

