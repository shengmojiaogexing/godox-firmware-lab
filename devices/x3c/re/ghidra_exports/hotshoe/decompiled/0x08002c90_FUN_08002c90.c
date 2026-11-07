// 0x08002c90 FUN_08002c90 size=104

void FUN_08002c90(uint *param_1)

{
  char cVar1;
  uint *puVar2;
  
  puVar2 = DAT_08002cf8;
  cVar1 = (char)param_1[1];
  if (*(char *)((int)param_1 + 6) == '\0') {
    if (cVar1 == '\0') {
      *DAT_08002cf8 = *DAT_08002cf8 & ~*param_1;
      return;
    }
    if (cVar1 == '\x01') {
      DAT_08002cf8[1] = DAT_08002cf8[1] & ~*param_1;
    }
    return;
  }
  if (cVar1 == '\0') {
    *DAT_08002cf8 = *DAT_08002cf8 | *param_1;
  }
  else if (cVar1 == '\x01') {
    DAT_08002cf8[1] = DAT_08002cf8[1] | *param_1;
  }
  if (*(char *)((int)param_1 + 5) == '\0') {
    puVar2[2] = puVar2[2] | *param_1;
    return;
  }
  if (*(char *)((int)param_1 + 5) != '\x01') {
    puVar2[2] = puVar2[2] | *param_1;
  }
  puVar2[3] = puVar2[3] | *param_1;
  return;
}

