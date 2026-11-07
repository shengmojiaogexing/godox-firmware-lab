// 0x08002ebc FUN_08002ebc size=64

void FUN_08002ebc(void)

{
  byte bVar1;
  char *pcVar2;
  byte *pbVar3;
  uint uVar4;
  
  pbVar3 = DAT_08002f04;
  pcVar2 = DAT_08002f00;
  uVar4 = 0;
  while ((*(byte *)(DAT_08002efc + uVar4) & 3) != 0) {
    uVar4 = uVar4 + 1 & 0xff;
    if (4 < uVar4) {
      return;
    }
  }
  if (*DAT_08002f00 == '\0') {
    return;
  }
  bVar1 = *DAT_08002f04;
  *DAT_08002f04 = bVar1 + 1;
  if ((byte)(bVar1 + 1) < 4) {
    return;
  }
  if (*(char *)(DAT_08002f08 + 3) == '\0') {
    *pbVar3 = 0;
    *pcVar2 = '\0';
    return;
  }
  *pbVar3 = 1;
  return;
}

