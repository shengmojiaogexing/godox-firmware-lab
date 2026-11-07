// 0x0801184c FUN_0801184c size=46

void FUN_0801184c(undefined1 param_1,undefined1 param_2,undefined1 param_3)

{
  uint *puVar1;
  undefined1 *puVar2;
  
  puVar1 = DAT_0801187c;
  *DAT_0801187c = *DAT_0801187c | 0x1000;
  puVar2 = DAT_08011884;
  if (*DAT_08011880 == '\0') {
    *DAT_08011884 = 0xa9;
    puVar2[1] = param_1;
    puVar2[2] = param_2;
    puVar2[3] = param_3;
    FUN_0801708c();
  }
  *puVar1 = *puVar1 & 0xffffefff;
  return;
}

