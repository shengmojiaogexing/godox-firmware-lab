// 0x080089d0 FUN_080089d0 size=2070

void FUN_080089d0(void)

{
  char cVar1;
  byte bVar2;
  char *pcVar3;
  char *pcVar4;
  byte *pbVar5;
  char *pcVar6;
  uint uVar7;
  byte bVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  int iVar13;
  
  pcVar3 = DAT_08008dd4;
  if (*DAT_08008dd0 == '\0') {
    cVar1 = DAT_08008dd4[9];
    DAT_08008dd4[9] = cVar1 + 1U;
    if ((byte)(cVar1 + 1U) < 0x29) {
      return;
    }
    pcVar3[9] = '\0';
  }
  pcVar4 = DAT_08008dd8;
  if (pcVar3[2] != *DAT_08008dd8) {
    pcVar3[2] = *DAT_08008dd8;
    bVar8 = 0;
    do {
      FUN_0801184c(0x50,0xb3,*pcVar4);
      FUN_0801c118(800);
      bVar8 = bVar8 + 1;
    } while (bVar8 < 2);
  }
  pcVar4 = DAT_08008ddc;
  if (pcVar3[3] != *DAT_08008ddc) {
    pcVar3[3] = *DAT_08008ddc;
    bVar8 = 0;
    do {
      FUN_0801184c(0x50,0xb2,*pcVar4);
      FUN_0801c118(800);
      bVar8 = bVar8 + 1;
    } while (bVar8 < 2);
  }
  pcVar4 = DAT_08008de4;
  if (*DAT_08008de0 == '\0') {
    if (pcVar3[6] != *DAT_08008de4) {
      pcVar3[6] = *DAT_08008de4;
      bVar8 = 0;
      do {
        FUN_0801184c(0x50,0xb5,*pcVar4);
        FUN_0801c118(800);
        bVar8 = bVar8 + 1;
      } while (bVar8 < 2);
    }
    if (pcVar3[7] != *DAT_08008de8) {
      pcVar3[7] = *DAT_08008de8;
      FUN_0801184c(0x50,0xb7);
      FUN_0801c118(800);
      FUN_0801184c(0x50,0xb7,pcVar3[7]);
    }
    pcVar4 = DAT_08008dec;
    if (pcVar3[4] != *DAT_08008dec) {
      pcVar3[4] = *DAT_08008dec;
      bVar8 = 0;
      do {
        FUN_0801184c(0x50,0xc0,*pcVar4);
        FUN_0801c118(800);
        bVar8 = bVar8 + 1;
      } while (bVar8 < 2);
    }
    cVar1 = *DAT_08008df0;
    if (cVar1 == pcVar3[1]) {
      if (cVar1 != '\0') {
        uVar9 = 0;
        do {
          if (uVar9 < 6) {
            uVar7 = uVar9 + 10;
          }
          else {
            uVar7 = uVar9 - 6;
          }
          uVar7 = uVar7 & 0xff;
          iVar10 = DAT_08008df4 + uVar9 * 6;
          iVar13 = DAT_08008df8 + uVar9 * 6;
          bVar8 = *(byte *)(iVar10 + 4);
          bVar2 = *(byte *)(iVar13 + 4);
          if ((bVar8 & 0x80) == (bVar2 & 0x80)) {
            if (((int)((uint)bVar8 << 0x18) < 0) &&
               (((*(byte *)(iVar10 + 5) & 1) != (*(byte *)(iVar13 + 5) & 1) ||
                ((byte)(bVar8 * '\x02') != (byte)(bVar2 * '\x02'))))) {
              if ((*(byte *)(iVar10 + 5) & 1) == 0) {
                bVar8 = 0;
                do {
                  FUN_0801184c(uVar7,0xd3,1);
                  FUN_0801c118(800);
                  FUN_0801184c(uVar7,0xd6,0);
                  FUN_0801c118(800);
                  FUN_0801184c(uVar7,0xd1,*(byte *)(iVar10 + 4) & 0x7f);
                  FUN_0801c118(800);
                  bVar8 = bVar8 + 1;
                } while (bVar8 < 2);
              }
              else {
                bVar8 = 0;
                do {
                  FUN_0801184c(uVar7,0xd3,1);
                  FUN_0801c118(800);
                  FUN_0801184c(uVar7,0xd6,1);
                  FUN_0801c118(800);
                  bVar8 = bVar8 + 1;
                } while (bVar8 < 2);
              }
              *(byte *)(iVar13 + 5) = *(byte *)(iVar10 + 5) & 1;
              *(undefined1 *)(iVar13 + 4) = *(undefined1 *)(iVar10 + 4);
            }
          }
          else if ((int)((uint)bVar8 << 0x18) < 0) {
            *(byte *)(iVar13 + 4) = bVar2 | 0x80;
            if ((*(byte *)(iVar10 + 5) & 1) == 0) {
              bVar8 = 0;
              do {
                FUN_0801184c(uVar7,0xd3,1);
                FUN_0801c118(800);
                FUN_0801184c(uVar7,0xd6,0);
                FUN_0801c118(800);
                FUN_0801184c(uVar7,0xd1,*(byte *)(iVar10 + 4) & 0x7f);
                FUN_0801c118(800);
                bVar8 = bVar8 + 1;
              } while (bVar8 < 2);
            }
            else {
              bVar8 = 0;
              do {
                FUN_0801184c(uVar7,0xd3,1);
                FUN_0801c118(800);
                FUN_0801184c(uVar7,0xd6,1);
                FUN_0801c118(800);
                bVar8 = bVar8 + 1;
              } while (bVar8 < 2);
            }
          }
          else {
            *(byte *)(iVar13 + 4) = bVar2 & 0x7f;
            bVar8 = 0;
            do {
              FUN_0801184c(uVar7,0xd3,0);
              FUN_0801c118(800);
              bVar8 = bVar8 + 1;
            } while (bVar8 < 2);
          }
          uVar9 = uVar9 + 1 & 0xff;
        } while (uVar9 < 0x10);
      }
    }
    else if (cVar1 == '\0') {
      FUN_080118b0(0x88);
      pcVar3[1] = '\0';
    }
    else {
      FUN_080118b0(0x89);
      FUN_0801c118(400);
      iVar10 = DAT_08008df4;
      uVar9 = 0;
      do {
        if (uVar9 < 6) {
          uVar7 = uVar9 + 10;
        }
        else {
          uVar7 = uVar9 - 6;
        }
        uVar7 = uVar7 & 0xff;
        iVar13 = iVar10 + uVar9 * 6;
        if ((int)((uint)*(byte *)(iVar13 + 4) << 0x18) < 0) {
          if ((*(byte *)(iVar13 + 5) & 1) == 0) {
            bVar8 = 0;
            do {
              FUN_0801184c(uVar7,0xd3,1);
              FUN_0801c118(800);
              FUN_0801184c(uVar7,0xd6,0);
              FUN_0801c118(800);
              FUN_0801184c(uVar7,0xd1,*(byte *)(iVar13 + 4) & 0x7f);
              FUN_0801c118(800);
              bVar8 = bVar8 + 1;
            } while (bVar8 < 2);
          }
          else {
            bVar8 = 0;
            do {
              FUN_0801184c(uVar7,0xd3,1);
              FUN_0801c118(800);
              FUN_0801184c(uVar7,0xd6,1);
              FUN_0801c118(800);
              bVar8 = bVar8 + 1;
            } while (bVar8 < 2);
          }
        }
        else {
          bVar8 = 0;
          do {
            FUN_0801184c(uVar7,0xd3,0);
            FUN_0801c118(800);
            bVar8 = bVar8 + 1;
          } while (bVar8 < 2);
        }
        uVar9 = uVar9 + 1 & 0xff;
      } while (uVar9 < 0x10);
      pcVar3[1] = '\x01';
    }
    pcVar4 = DAT_08008dfc;
    cVar1 = *DAT_08008dfc;
    if (pcVar3[5] != cVar1) {
      if (cVar1 == '\0') {
        bVar8 = 0;
        do {
          FUN_080118b0(0x40);
          FUN_0801c118(800);
          bVar8 = bVar8 + 1;
        } while (bVar8 < 2);
      }
      else if (cVar1 == '\x01') {
        bVar8 = 0;
        do {
          FUN_080118b0(0x41);
          FUN_0801c118(800);
          bVar8 = bVar8 + 1;
        } while (bVar8 < 2);
      }
      pcVar3[5] = *pcVar4;
    }
    if (*DAT_08008e00 != *pcVar3) {
      if (*DAT_08008e00 == '\0') {
        bVar8 = 0;
        do {
          FUN_080118b0(0x80);
          FUN_0801c118(800);
          bVar8 = bVar8 + 1;
        } while (bVar8 < 2);
        *pcVar3 = '\0';
      }
      else {
        bVar8 = 0;
        do {
          FUN_080118b0(0x81);
          FUN_0801c118(800);
          bVar8 = bVar8 + 1;
        } while (bVar8 < 2);
        *pcVar3 = '\x01';
      }
    }
    iVar10 = DAT_0800921c;
    pcVar6 = DAT_08009214;
    pcVar4 = DAT_08009210;
    pbVar5 = DAT_08008e04;
    if ((pcVar3[10] & 0x80U) == (*DAT_08008e04 & 0x80)) {
      if ((int)((uint)(byte)pcVar3[10] << 0x18) < 0) {
        if (*(char *)(DAT_08009220 + 0xb) != *DAT_08009210) {
          bVar8 = 0;
          do {
            FUN_0801184c(0x50,0xbd,*pcVar4);
            FUN_0801c118(800);
            bVar8 = bVar8 + 1;
          } while (bVar8 < 2);
          uVar9 = 0;
          do {
            bVar8 = 0;
            do {
              if ((1 << uVar9 & (uint)*pbVar5) == 0) {
                FUN_0801184c(uVar9 + 10 & 0xff,0xbd,0xff);
              }
              bVar8 = bVar8 + 1;
            } while (bVar8 < 2);
            uVar9 = uVar9 + 1 & 0xff;
          } while (uVar9 < 5);
        }
        iVar10 = DAT_08009220;
        uVar9 = 0;
        do {
          uVar7 = 1 << uVar9;
          if ((*(byte *)(iVar10 + 10) & uVar7) != (*pbVar5 & uVar7)) {
            bVar8 = 0;
            uVar11 = uVar9 + 10;
            do {
              if ((uVar7 & *pbVar5) == 0) {
                FUN_0801184c(uVar11 & 0xff,0xbd,0xff);
                FUN_0801c118(800);
                *(byte *)(iVar10 + 10) = *(byte *)(iVar10 + 10) & ~(byte)uVar7;
              }
              else {
                FUN_0801184c(uVar11 & 0xff,0xbd,*pcVar4);
                FUN_0801c118(800);
                FUN_0801184c(uVar11 & 0xff,0xbf,*DAT_08009218);
                FUN_0801c118(800);
                FUN_0801184c(uVar11 & 0xff,0xbe,*DAT_08009214);
                FUN_0801c118(800);
                *(byte *)(iVar10 + 10) = *(byte *)(iVar10 + 10) | (byte)uVar7;
              }
              bVar8 = bVar8 + 1;
            } while (bVar8 < 2);
          }
          pcVar3 = DAT_08009218;
          uVar9 = uVar9 + 1 & 0xff;
        } while (uVar9 < 5);
        if (*(char *)(iVar10 + 0xd) != *DAT_08009218) {
          bVar8 = 0;
          do {
            FUN_0801184c(0x50,0xbf,*pcVar3);
            FUN_0801c118(800);
            bVar8 = bVar8 + 1;
          } while (bVar8 < 2);
        }
        pcVar6 = DAT_08009214;
        if (*(char *)(iVar10 + 0xc) != *DAT_08009214) {
          bVar8 = 0;
          do {
            FUN_0801184c(0x50,0xbe,*pcVar6);
            FUN_0801c118(800);
            bVar8 = bVar8 + 1;
          } while (bVar8 < 2);
        }
        *(char *)(iVar10 + 0xd) = *pcVar3;
        *(char *)(iVar10 + 0xc) = *pcVar6;
        *(char *)(iVar10 + 0xb) = *pcVar4;
      }
      else {
        uVar9 = 0;
        do {
          if (uVar9 < 6) {
            uVar7 = uVar9 + 10;
          }
          else {
            uVar7 = uVar9 - 6;
          }
          uVar7 = uVar7 & 0xff;
          iVar12 = DAT_08009224 + uVar9 * 6;
          iVar13 = iVar10 + uVar9 * 6;
          if ((*(byte *)(iVar12 + 3) & 0x40) == (*(byte *)(iVar13 + 3) & 0x40)) {
            if ((int)((uint)*(byte *)(iVar13 + 3) << 0x19) < 0) {
              if (*(char *)(iVar10 + uVar9 * 6) != *(char *)(DAT_08009224 + uVar9 * 6)) {
                bVar8 = 0;
                do {
                  cVar1 = *(char *)(iVar10 + uVar9 * 6);
                  if (cVar1 == '\0') {
                    FUN_0801184c(uVar7,0xb1,1);
                    FUN_0801c118(800);
                    FUN_0801184c(uVar7,0xbc,*(undefined1 *)(iVar13 + 1));
                    FUN_0801c118(800);
                  }
                  else if (cVar1 == '\x01') {
                    FUN_0801184c(uVar7,0xb1,0);
                  }
                  *(undefined1 *)(DAT_08009224 + uVar9 * 6) = *(undefined1 *)(iVar10 + uVar9 * 6);
                  FUN_0801c118(800);
                  bVar8 = bVar8 + 1;
                } while (bVar8 < 2);
              }
              if (*(char *)(iVar13 + 2) != *(char *)(iVar12 + 2)) {
                *(char *)(iVar12 + 2) = *(char *)(iVar13 + 2);
              }
              if (*(char *)(iVar13 + 1) != *(char *)(iVar12 + 1)) {
                if (*(char *)(iVar10 + uVar9 * 6) == '\0') {
                  bVar8 = 0;
                  do {
                    FUN_0801184c(uVar7,0xbc,*(undefined1 *)(iVar13 + 1));
                    FUN_0801c118(800);
                    bVar8 = bVar8 + 1;
                  } while (bVar8 < 2);
                }
                *(undefined1 *)(iVar12 + 1) = *(undefined1 *)(iVar13 + 1);
              }
            }
          }
          else {
            bVar8 = 0;
            do {
              if ((int)((uint)*(byte *)(iVar13 + 3) << 0x19) < 0) {
                cVar1 = *(char *)(iVar10 + uVar9 * 6);
                if (cVar1 == '\0') {
                  FUN_0801184c(uVar7,0xb1,1);
                  FUN_0801c118(800);
                  FUN_0801184c(uVar7,0xbc,*(undefined1 *)(iVar13 + 1));
                  FUN_0801c118(800);
                  *(undefined1 *)(iVar12 + 1) = *(undefined1 *)(iVar13 + 1);
                }
                else if (cVar1 == '\x01') {
                  FUN_0801184c(uVar7,0xb1,0);
                  FUN_0801c118(800);
                  FUN_0801184c(uVar7,0xbc,0xff);
                  FUN_0801c118(800);
                  *(undefined1 *)(iVar12 + 2) = *(undefined1 *)(iVar13 + 2);
                }
                FUN_0801c118(800);
              }
              else {
                FUN_0801184c(uVar7,0xb1,1);
                FUN_0801c118(800);
                FUN_0801184c(uVar7,0xbc,0xff);
                FUN_0801c118(800);
                *(byte *)(iVar12 + 3) = *(byte *)(iVar12 + 3) & 0xbf;
              }
              bVar8 = bVar8 + 1;
            } while (bVar8 < 2);
            *(byte *)(iVar12 + 3) = *(byte *)(iVar13 + 3) & 0x40;
          }
          uVar9 = uVar9 + 1 & 0xff;
        } while (uVar9 < 0x10);
      }
    }
    else if ((int)((uint)*DAT_08008e04 << 0x18) < 0) {
      bVar8 = 0;
      do {
        FUN_0801184c(0x50,0xb1,2);
        FUN_0801c118(800);
        FUN_0801184c(0x50,0xbf,*DAT_08009218);
        FUN_0801c118(800);
        FUN_0801184c(0x50,0xbe,*pcVar6);
        FUN_0801c118(800);
        uVar9 = 0;
        do {
          if ((1 << uVar9 & (uint)*pbVar5) == 0) {
            FUN_0801184c(uVar9 + 10 & 0xff,0xbd,0xff);
          }
          else {
            FUN_0801184c(uVar9 + 10 & 0xff,0xbd,*pcVar4);
          }
          FUN_0801c118(800);
          uVar9 = uVar9 + 1 & 0xff;
        } while (uVar9 < 5);
        bVar8 = bVar8 + 1;
      } while (bVar8 < 2);
      pcVar3[10] = pcVar3[10] | 0x80;
    }
    else {
      bVar8 = 0;
      do {
        uVar9 = 0;
        do {
          if (uVar9 < 6) {
            uVar7 = uVar9 + 10;
          }
          else {
            uVar7 = uVar9 - 6;
          }
          uVar7 = uVar7 & 0xff;
          iVar13 = iVar10 + uVar9 * 6;
          if ((int)((uint)*(byte *)(iVar13 + 3) << 0x19) < 0) {
            cVar1 = *(char *)(iVar10 + uVar9 * 6);
            if (cVar1 == '\0') {
              FUN_0801184c(uVar7,0xb1,1);
              FUN_0801c118(800);
              FUN_0801184c(uVar7,0xbc,*(undefined1 *)(iVar13 + 1));
            }
            else if (cVar1 == '\x01') {
              FUN_0801184c(uVar7,0xb1,0);
            }
          }
          else {
            FUN_0801184c(uVar7,0xb1,1);
            FUN_0801c118(800);
            FUN_0801184c(uVar7,0xbc,0xff);
            FUN_0801c118(800);
          }
          FUN_0801c118(800);
          uVar9 = uVar9 + 1 & 0xff;
        } while (uVar9 < 0x10);
        bVar8 = bVar8 + 1;
      } while (bVar8 < 2);
      pcVar3[10] = pcVar3[10] & 0x7f;
    }
  }
  return;
}

