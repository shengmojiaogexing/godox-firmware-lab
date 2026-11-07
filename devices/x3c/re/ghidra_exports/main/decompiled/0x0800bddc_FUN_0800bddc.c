// 0x0800bddc FUN_0800bddc size=412

void FUN_0800bddc(void)

{
  char cVar1;
  byte *pbVar2;
  undefined1 *puVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  
  iVar4 = DAT_0800bf8c;
  pbVar2 = DAT_0800bf78;
  if ((int)((uint)*DAT_0800bf78 << 0x18) < 0) {
    FUN_0801184c(0x50,0xb1,2);
    FUN_0801c118(200);
    FUN_0801184c(0x50,0xbf,*DAT_0800bf7c);
    FUN_0801c118(200);
    FUN_0801184c(0x50,0xbe,*DAT_0800bf80);
    FUN_0801c118(200);
    puVar3 = DAT_0800bf84;
    uVar6 = 0;
    do {
      if ((1 << uVar6 & (uint)*pbVar2) == 0) {
        FUN_0801184c(uVar6 + 10 & 0xff,0xbd,0xff);
      }
      else {
        FUN_0801184c(uVar6 + 10 & 0xff,0xbd,*puVar3);
      }
      FUN_0801c118(200);
      uVar6 = uVar6 + 1 & 0xff;
    } while (uVar6 < 5);
  }
  else if (*DAT_0800bf88 == '\x02') {
    uVar6 = 0;
    do {
      if (uVar6 < 6) {
        uVar5 = uVar6 + 10;
      }
      else {
        uVar5 = uVar6 - 6;
      }
      uVar5 = uVar5 & 0xff;
      iVar7 = iVar4 + uVar6 * 6;
      if ((int)((uint)*(byte *)(iVar7 + 3) << 0x19) < 0) {
        if (*(char *)(iVar4 + uVar6 * 6) == '\0') {
          FUN_0801184c(uVar5,0xb1,1);
          FUN_0801c118(200);
          FUN_0801184c(uVar5,0xbc,*(undefined1 *)(iVar7 + 1));
          FUN_0801c118(200);
        }
      }
      else {
        FUN_0801184c(uVar5,0xb1,1);
        FUN_0801c118(200);
        FUN_0801184c(uVar5,0xbc,0xff);
        FUN_0801c118(200);
      }
      uVar6 = uVar6 + 1 & 0xff;
    } while (uVar6 < 5);
  }
  else {
    uVar6 = 0;
    do {
      if (uVar6 < 6) {
        uVar5 = uVar6 + 10;
      }
      else {
        uVar5 = uVar6 - 6;
      }
      uVar5 = uVar5 & 0xff;
      iVar7 = iVar4 + uVar6 * 6;
      if ((int)((uint)*(byte *)(iVar7 + 3) << 0x19) < 0) {
        cVar1 = *(char *)(iVar4 + uVar6 * 6);
        if (cVar1 == '\0') {
          FUN_0801184c(uVar5,0xb1,1);
          FUN_0801c118(200);
          FUN_0801184c(uVar5,0xbc,*(undefined1 *)(iVar7 + 1));
        }
        else if (cVar1 == '\x01') {
          FUN_0801184c(uVar5,0xb1,0);
          FUN_0801c118(200);
          FUN_0801184c(uVar5,0xbc,0xff);
        }
      }
      else {
        FUN_0801184c(uVar5,0xb1,1);
        FUN_0801c118(200);
        FUN_0801184c(uVar5,0xbc,0xff);
      }
      FUN_0801c118(200);
      uVar6 = uVar6 + 1 & 0xff;
    } while (uVar6 < 5);
  }
  FUN_0801184c(0x50,0xb2,*DAT_0800bf90);
  FUN_0801c118(200);
  return;
}

