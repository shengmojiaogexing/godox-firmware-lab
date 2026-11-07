// 0x08036dbc FUN_08036dbc size=154

void FUN_08036dbc(int param_1)

{
  byte *pbVar1;
  byte bVar2;
  short sVar3;
  char *pcVar4;
  undefined4 uVar5;
  
  uVar5 = *(undefined4 *)(param_1 + 0xc);
  if (*(char *)(param_1 + 8) == '\x04') {
    FUN_0802e34c(*(undefined4 *)(param_1 + 4),8);
    FUN_0802b8b2(*DAT_08036e60,0);
    return;
  }
  if (*(char *)(param_1 + 8) != '\r') {
    return;
  }
  pcVar4 = (char *)FUN_0802ad6c();
  pbVar1 = DAT_08036e5c;
  if (*pcVar4 == '\x14') {
    bVar2 = *DAT_08036e5c;
    if (0x14 < bVar2) {
      if (*DAT_08036e58 == '\0') {
        *DAT_08036e5c = bVar2 - 1;
      }
      else {
        if (bVar2 < 0x1f) {
          bVar2 = 0x14;
        }
        else {
          bVar2 = bVar2 - 10;
        }
        *DAT_08036e5c = bVar2;
      }
    }
  }
  else if ((*pcVar4 == '\x13') && (bVar2 = *DAT_08036e5c, bVar2 < 100)) {
    if (*DAT_08036e58 == '\0') {
      *DAT_08036e5c = bVar2 + 1;
    }
    else {
      if (bVar2 < 0x5a) {
        bVar2 = bVar2 + 10;
      }
      else {
        bVar2 = 100;
      }
      *DAT_08036e5c = bVar2;
    }
  }
  if (*pbVar1 < 0x15) {
    sVar3 = 0;
  }
  else {
    sVar3 = *pbVar1 - 0x14;
  }
  FUN_0802782e(uVar5,sVar3,1);
  FUN_0802ad74(uVar5,0x1c,0);
  return;
}

