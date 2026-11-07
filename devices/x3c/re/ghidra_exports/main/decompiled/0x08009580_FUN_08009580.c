// 0x08009580 FUN_08009580 size=500

void FUN_08009580(undefined2 *param_1,undefined2 *param_2)

{
  char cVar1;
  char cVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  
  bVar3 = true;
  iVar6 = 0;
  iVar5 = 0;
  uVar8 = 0;
  cVar1 = *DAT_08009778;
  do {
    iVar7 = DAT_0800977c + uVar8 * 6;
    if (*(byte *)(iVar7 + 3) >> 6 == 3) {
      cVar2 = *(char *)(DAT_0800977c + uVar8 * 6);
      if (cVar2 == '\0') {
        iVar4 = (int)(char)(*DAT_08009780 - *(char *)(iVar7 + 1));
        if ((cVar1 != '\0') && (cVar1 == '\x01')) {
          if (iVar4 % 10 == 0) {
            iVar4 = (int)(char)((char)(iVar4 / 10) + (char)(iVar4 / 10 << 1));
          }
          else if (iVar4 % 10 == 3) {
            iVar4 = (int)(char)((char)(iVar4 / 10) + (char)(iVar4 / 10 << 1) + '\x01');
          }
          else if (iVar4 % 10 == 7) {
            iVar4 = (int)(char)((char)(iVar4 / 10) + (char)(iVar4 / 10 << 1) + '\x02');
          }
        }
        if (bVar3) {
          iVar6 = iVar4;
        }
        else if (iVar4 < iVar6) {
          iVar6 = iVar4;
        }
        iVar7 = (int)*(char *)(iVar7 + 1);
        if ((cVar1 != '\0') && (cVar1 == '\x01')) {
          if (iVar7 % 10 == 0) {
            iVar7 = (int)(char)((char)(iVar7 / 10) + (char)(iVar7 / 10 << 1));
          }
          else if (iVar7 % 10 == 3) {
            iVar7 = (int)(char)((char)(iVar7 / 10) + (char)(iVar7 / 10 << 1) + '\x01');
          }
          else if (iVar7 % 10 == 7) {
            iVar7 = (int)(char)((char)(iVar7 / 10) + (char)(iVar7 / 10 << 1) + '\x02');
          }
        }
        if (bVar3) {
          iVar5 = iVar7;
        }
        else if (iVar7 < iVar5) {
          iVar5 = iVar7;
        }
      }
      else if (cVar2 == '\x01') {
        iVar4 = (int)*(char *)(iVar7 + 2);
        if (iVar4 % 10 == 0) {
          iVar4 = (int)(char)((char)(iVar4 / 10) + (char)(iVar4 / 10 << 1));
        }
        else if (iVar4 % 10 == 3) {
          iVar4 = (int)(char)((char)(iVar4 / 10) + (char)(iVar4 / 10 << 1) + '\x01');
        }
        else if (iVar4 % 10 == 7) {
          iVar4 = (int)(char)((char)(iVar4 / 10) + (char)(iVar4 / 10 << 1) + '\x02');
        }
        if (bVar3) {
          iVar6 = iVar4;
        }
        else if (iVar4 < iVar6) {
          iVar6 = iVar4;
        }
        iVar7 = (int)(char)('<' - *(char *)(iVar7 + 2));
        if (iVar7 % 10 == 0) {
          iVar7 = (int)(char)((char)(iVar7 / 10) + (char)(iVar7 / 10 << 1));
        }
        else if (iVar7 % 10 == 3) {
          iVar7 = (int)(char)((char)(iVar7 / 10) + (char)(iVar7 / 10 << 1) + '\x01');
        }
        else if (iVar7 % 10 == 7) {
          iVar7 = (int)(char)((char)(iVar7 / 10) + (char)(iVar7 / 10 << 1) + '\x02');
        }
        if (bVar3) {
          iVar5 = iVar7;
        }
        else if (iVar7 < iVar5) {
          iVar5 = iVar7;
        }
      }
      bVar3 = false;
    }
    uVar8 = uVar8 + 1 & 0xff;
  } while (uVar8 < 0x10);
  *param_1 = (short)iVar6;
  *param_2 = (short)iVar5;
  return;
}

