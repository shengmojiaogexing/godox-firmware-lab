// 0x080118b0 FUN_080118b0 size=42

void FUN_080118b0(undefined1 param_1)

{
  uint *puVar1;
  undefined1 *puVar2;
  
  puVar1 = DAT_080118dc;
  *DAT_080118dc = *DAT_080118dc | 0x1000;
  puVar2 = DAT_080118e4;
  if (*DAT_080118e0 == '\0') {
    *DAT_080118e4 = 0xd5;
    puVar2[1] = param_1;
    FUN_0801712c();
  }
  *puVar1 = *puVar1 & 0xffffefff;
  return;
}

