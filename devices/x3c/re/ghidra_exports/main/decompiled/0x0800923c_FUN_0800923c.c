// 0x0800923c FUN_0800923c size=272

void FUN_0800923c(void)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  undefined4 uVar6;
  
  *DAT_0800934c = 0;
  if ((*DAT_08009350 == '\0') && (*DAT_08009354 == '\0')) {
    *DAT_0800935c = *DAT_08009358;
    *DAT_08009364 = (uint)*DAT_08009360;
    iVar4 = DAT_08009370;
    iVar3 = DAT_0800936c;
    if (*DAT_08009368 == '\0') {
      cVar1 = *(char *)(DAT_08009370 + 8);
      if (cVar1 == '\0') {
        *(uint *)(DAT_0800936c + 0x14) = *(uint *)(DAT_0800936c + 0x14) | 0x4000;
        *(undefined2 *)(iVar4 + 0x16) = 100;
        pcVar5 = DAT_08009374;
        if (*DAT_08009374 == '\0') {
          *DAT_08009378 = 1;
          FUN_080118b0(0x19);
          if (*(char *)(iVar4 + 0xf) == '\0') {
            FUN_0801c118(100);
            FUN_080118b0(0x19);
            return;
          }
        }
        else {
          *DAT_08009378 = 1;
          FUN_08010b6c();
          uVar6 = DAT_0800937c;
          FUN_08022e34(DAT_0800937c,0x4000);
          FUN_0801c118(500);
          FUN_08010b24();
          FUN_08022e38(uVar6,0x4000);
          FUN_08022e34(uVar6,1);
          FUN_08022e38(uVar6,2);
          FUN_08023540(0x18,8);
          FUN_08023520(0x36);
          FUN_08023520(0x3b);
          FUN_08023540(6,4);
          *pcVar5 = '\0';
        }
      }
      else {
        cVar2 = *(char *)(DAT_08009370 + 10);
        if (cVar1 == '\x01') {
          if (cVar2 != '\0') {
            FUN_0801184c(cVar2,0xb4,1);
            *(undefined1 *)(iVar4 + 10) = 0;
          }
        }
        else if ((cVar1 == '\x02') && (cVar2 != '\0')) {
          FUN_0801184c(cVar2,0xb4,4);
          *(undefined1 *)(iVar4 + 10) = 0;
        }
      }
    }
    else {
      FUN_080118b0(9);
      FUN_0801c118(100);
      FUN_080118b0(9);
      *(uint *)(iVar3 + 0x14) = *(uint *)(iVar3 + 0x14) | 0x4000;
      *(undefined2 *)(iVar4 + 0x16) = 100;
    }
  }
  return;
}

