// 0x08014b28 FUN_08014b28 size=2042

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_08014b28(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  short sVar1;
  char cVar2;
  char *pcVar3;
  char *pcVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  short *psVar7;
  char *pcVar8;
  char *pcVar9;
  char *pcVar10;
  ushort *puVar11;
  byte *pbVar12;
  char *pcVar13;
  undefined4 *puVar14;
  byte *pbVar15;
  char *pcVar16;
  undefined4 *puVar17;
  undefined4 *puVar18;
  byte *pbVar19;
  char *pcVar20;
  undefined4 *puVar21;
  char *pcVar22;
  char *pcVar23;
  byte bVar24;
  int iVar25;
  int iVar26;
  undefined4 uVar27;
  int iVar28;
  ushort uVar29;
  char cVar30;
  uint uVar31;
  bool bVar32;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  
  puVar6 = DAT_08014f18;
  puVar5 = DAT_08014f14;
  pcVar3 = DAT_08014f10;
  pcVar4 = DAT_08014f0c;
  if (*DAT_08014f0c == '\x02') {
    if (DAT_08014f10[0xb] == '\0') {
      *DAT_08014f18 = *DAT_08014f14;
      *DAT_08014f1c = *puVar5;
      pcVar9 = DAT_08014f2c;
      pcVar8 = DAT_08014f28;
      psVar7 = DAT_08014f20;
      sVar1 = *DAT_08014f20;
      cVar30 = *DAT_08014f24;
      cVar2 = *DAT_08014f28;
      if (sVar1 != 0) {
        if (*DAT_08014f2c == '\0') {
          *DAT_08014f2c = '\x01';
          pcVar3[7] = '\x01';
        }
        *psVar7 = sVar1 + -1;
        if (cVar2 != '\0') {
          *psVar7 = 0;
        }
        if (cVar30 != '\0') {
          *pcVar9 = '\0';
          *psVar7 = 0;
          pcVar3[7] = '\x01';
        }
        if (*psVar7 == 0) {
          *pcVar9 = '\0';
          pcVar3[7] = '\x01';
        }
      }
      pcVar10 = DAT_08014f3c;
      pcVar9 = DAT_08014f34;
      bVar24 = *DAT_08014f38;
      if ((*DAT_08014f30 != '\0') && (*DAT_08014f30 = '\0', cVar30 == '\0')) {
        if (cVar2 == '\0') {
          if (*pcVar10 == '\0') {
            if ((int)((uint)bVar24 << 0x18) < 0) {
              *pcVar4 = '\x01';
            }
            else {
              *pcVar4 = '\0';
            }
          }
          else {
            *pcVar9 = *pcVar10;
            *pcVar10 = '\0';
          }
        }
        else {
          *pcVar8 = '\0';
        }
      }
      puVar11 = DAT_08014f40;
      if (*DAT_08014f40 < 0x65) {
        *DAT_08014f40 = 100;
      }
      if (*DAT_08014f44 != '\0') {
        *DAT_08014f44 = '\0';
      }
      cVar30 = *pcVar8;
      if (cVar30 == '\0') {
        uVar29 = *(short *)(pcVar3 + 0x12) + 1;
        *(ushort *)(pcVar3 + 0x12) = uVar29;
        if (uVar29 < 0xc9) {
          if (uVar29 == 0xc3) {
            *puVar11 = 2000;
          }
          else if (uVar29 == 0xbe) {
            *pcVar10 = '\0';
          }
        }
        else {
          pcVar3[0x12] = '\0';
          pcVar3[0x13] = '\0';
          if ((int)((uint)bVar24 << 0x18) < 0) {
            *pcVar4 = '\x01';
          }
          else {
            *pcVar4 = '\0';
          }
        }
      }
      else {
        *puVar6 = *puVar5;
        *DAT_08014f4c = (uint)*DAT_08014f48;
      }
      cVar2 = *pcVar4;
      uStack_30 = param_2;
      uStack_2c = param_3;
      uStack_28 = param_4;
      if (cVar2 == '\x02') {
        if (pcVar3[0xe] != cVar30) {
          pcVar3[7] = '\x01';
          puVar5 = DAT_08014f58;
          if (cVar30 == '\0') {
            iVar25 = FUN_080303b8(*DAT_08014f58);
            if (iVar25 != 0) {
              FUN_0802da22(*puVar5,1);
            }
            pcVar3[0xe] = *pcVar8;
          }
          else {
            pcVar3[0xe] = cVar30;
          }
        }
        pcVar23 = _LAB_08015400;
        pcVar22 = _LAB_080153e8;
        puVar21 = _LAB_080153e4;
        pcVar20 = _LAB_080153e0;
        pbVar19 = _LAB_080153d0;
        puVar18 = _LAB_080153cc;
        puVar17 = _LAB_080153c0;
        pcVar16 = _LAB_080153bc;
        pbVar15 = _DAT_080153b0;
        puVar14 = DAT_080153ac;
        pcVar13 = DAT_080153a8;
        pcVar8 = DAT_080153a4;
        pcVar4 = DAT_08014f8c;
        puVar6 = DAT_08014f88;
        puVar5 = DAT_08014f68;
        pbVar12 = DAT_08014f64;
        cVar30 = *pcVar3;
        if ((cVar30 == *pcVar10) && (pcVar3[0xc] == '\0')) {
          if (cVar30 == '\x01') {
            if ((pcVar3[6] != *DAT_08014f64) && (iVar25 = FUN_080303b8(*DAT_08014f68), iVar25 != 0))
            {
              FUN_08031fba(*puVar5,*pbVar12 - 1,1);
              pcVar3[6] = *pbVar12;
            }
            iVar25 = DAT_08014f70;
            pcVar4 = DAT_08014f6c;
            if (*DAT_08014f6c == '\0') {
              pcVar3[1] = '\0';
              *DAT_08014f80 = 0;
              iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x30));
              if (iVar26 != 0) {
                FUN_0802e2d2(*(undefined4 *)(iVar25 + 0x30),1);
              }
              iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x34));
              if (iVar26 != 0) {
                FUN_0802da22(*(undefined4 *)(iVar25 + 0x34),1);
              }
            }
            else {
              pcVar3[0x12] = '\0';
              pcVar3[0x13] = '\0';
              pcVar3[1] = '\x01';
              iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x34));
              pcVar8 = DAT_08014f74;
              if (iVar26 != 0) {
                FUN_0802da22(*(undefined4 *)(iVar25 + 0x30),1);
                FUN_0802e2d2(*(undefined4 *)(iVar25 + 0x34),1);
                FUN_080273c0(*(undefined4 *)(iVar25 + 0x34),*pcVar8);
              }
              cVar30 = *pcVar8;
              if (cVar30 == 'd') {
                *pcVar4 = '\0';
                iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x1c));
                if (iVar26 != 0) {
                  FUN_0802e2d2(*(undefined4 *)(iVar25 + 0x1c),1);
                  iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x20));
                  if (iVar26 != 0) {
                    FUN_0802da22(*(undefined4 *)(iVar25 + 0x20),1);
                  }
                  iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x24));
                  if (iVar26 != 0) {
                    FUN_0802da22(*(undefined4 *)(iVar25 + 0x24),1);
                  }
                  iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x2c));
                  if (iVar26 != 0) {
                    FUN_0802da22(*(undefined4 *)(iVar25 + 0x2c),1);
                  }
                  iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x28));
                  if (iVar26 != 0) {
                    FUN_0802da22(*(undefined4 *)(iVar25 + 0x28),1);
                  }
                  iVar26 = DAT_08014f78;
                  uVar31 = 0;
                  do {
                    uVar27 = FUN_0802ebc8(*(undefined4 *)(iVar25 + uVar31 * 4 + 4),0);
                    FUN_08004c48(&uStack_30,10,&DAT_08014f7c,*(undefined1 *)(iVar26 + uVar31));
                    iVar28 = FUN_080303b8(uVar27);
                    if (iVar28 != 0) {
                      FUN_0802d33c(uVar27,&uStack_30);
                    }
                    uVar31 = uVar31 + 1 & 0xff;
                  } while (uVar31 < 6);
                }
                pcVar3[7] = '\x01';
              }
              else if (cVar30 == '\0') {
                pcVar3[7] = '\x01';
                iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x1c));
                if (iVar26 != 0) {
                  FUN_0802da22(*(undefined4 *)(iVar25 + 0x1c),1);
                }
                iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x20));
                if (iVar26 != 0) {
                  FUN_0802e2d2(*(undefined4 *)(iVar25 + 0x20),1);
                }
                iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x24));
                if (iVar26 != 0) {
                  FUN_0802e2d2(*(undefined4 *)(iVar25 + 0x24),1);
                }
                iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x2c));
                if (iVar26 != 0) {
                  FUN_0802e2d2(*(undefined4 *)(iVar25 + 0x2c),1);
                }
                iVar26 = FUN_080303b8(*(undefined4 *)(iVar25 + 0x28));
                if (iVar26 != 0) {
                  FUN_0802e2d2(*(undefined4 *)(iVar25 + 0x28),1);
                }
              }
            }
          }
          else if (cVar30 == '\x02') {
            if ((*DAT_08014f84 == '\x02') && (*DAT_08014f8c == '\x02')) {
              iVar25 = FUN_080303b8(*DAT_08014f88);
              if (((iVar25 != 0) && (iVar25 = FUN_080303b8(puVar6[1]), iVar25 != 0)) &&
                 (iVar25 = FUN_080303b8(puVar6[2]), iVar25 != 0)) {
                if (*DAT_080153a0 == '\0') {
                  FUN_0802e2d2(*puVar6,1);
                  FUN_0802da22(puVar6[1],1);
                  FUN_0802da22(puVar6[2],1);
                }
                else if (*DAT_080153a0 == '\x01') {
                  FUN_0802e2d2(puVar6[1],1);
                  FUN_0802da22(*puVar6,1);
                  FUN_0802da22(puVar6[2],1);
                }
              }
            }
            else if (pcVar3[2] != *DAT_08014f8c) {
              iVar25 = FUN_080303b8(*DAT_08014f88);
              if (((iVar25 != 0) && (iVar25 = FUN_080303b8(puVar6[1]), iVar25 != 0)) &&
                 (iVar25 = FUN_080303b8(puVar6[2]), iVar25 != 0)) {
                cVar30 = *pcVar4;
                if (cVar30 == '\0') {
                  FUN_0802e2d2(*puVar6,1);
                  FUN_0802da22(puVar6[1],1);
                  FUN_0802da22(puVar6[2],1);
                }
                else if (cVar30 == '\x01') {
                  FUN_0802e2d2(puVar6[1],1);
                  FUN_0802da22(*puVar6,1);
                  FUN_0802da22(puVar6[2],1);
                }
                else if (cVar30 == '\x02') {
                  FUN_0802e2d2(puVar6[2],1);
                  FUN_0802da22(*puVar6,1);
                  FUN_0802da22(puVar6[1],1);
                }
              }
              pcVar3[2] = *pcVar4;
            }
          }
          else if (cVar30 == '\x03') {
            if ((pcVar3[4] != *DAT_080153a4) || (pcVar3[3] != *DAT_080153a8)) {
              iVar25 = FUN_080303b8(*DAT_080153ac);
              if ((iVar25 != 0) &&
                 ((iVar25 = FUN_080303b8(puVar14[1]), iVar25 != 0 &&
                  (iVar25 = FUN_080303b8(puVar14[2]), iVar25 != 0)))) {
                if (*pcVar8 == '\0') {
                  cVar30 = *pcVar13;
                  if (cVar30 == '\0') {
                    FUN_0802e2d2(*puVar14,1);
                    FUN_0802da22(puVar14[1],1);
                    FUN_0802da22(puVar14[2],1);
                  }
                  else if (cVar30 == '\x01') {
                    FUN_0802e2d2(puVar14[1],1);
                    FUN_0802da22(*puVar14,1);
                    FUN_0802da22(puVar14[2],1);
                  }
                }
                else {
                  FUN_0802da22(*puVar14,1);
                  FUN_0802da22(puVar14[1],1);
                  FUN_0802e2d2(puVar14[2],1);
                }
              }
              pcVar3[4] = *pcVar8;
              pcVar3[3] = *pcVar13;
            }
          }
          else if (cVar30 != '\x04') {
            if (cVar30 == '\x05') {
              if (pcVar3[8] != *_DAT_080153b0) {
                pcVar3[7] = '\x01';
                puVar5 = _LAB_080153b4;
                iVar25 = FUN_080303b8(*_LAB_080153b4);
                puVar6 = _LAB_080153b8;
                if ((iVar25 != 0) && (iVar25 = FUN_080303b8(*_LAB_080153b8), iVar25 != 0)) {
                  if ((int)((uint)*pbVar15 << 0x18) < 0) {
                    FUN_0802e2d2(*puVar5,1);
                    FUN_0802e2d2(*puVar6,1);
                  }
                  else {
                    FUN_0802da22(*puVar5,1);
                    FUN_0802da22(*puVar6,1);
                  }
                }
                iVar25 = FUN_080303b8(*puVar5);
                if (iVar25 != 0) {
                  FUN_08031fba(*puVar5,*pbVar15 & 3,1);
                }
              }
              pcVar3[8] = *pbVar15;
            }
            else if (cVar30 == '\x06') {
              if (pcVar3[5] != *_LAB_080153bc) {
                iVar25 = FUN_080303b8(_LAB_080153c0[1]);
                if ((iVar25 != 0) && (iVar25 = FUN_080303b8(*puVar17), iVar25 != 0)) {
                  cVar30 = *pcVar16;
                  if (cVar30 == '\0') {
                    FUN_0802e2d2(puVar17[1],1);
                    FUN_0802da22(*puVar17,1);
                  }
                  else if (cVar30 == '\x01') {
                    FUN_0802e2d2(*puVar17,1);
                    FUN_0802da22(puVar17[1],1);
                  }
                }
                pcVar3[5] = *pcVar16;
              }
            }
            else if ((cVar30 != '\a') && (cVar30 != '\b')) {
              if (cVar30 == '\t') {
                bVar24 = *_LAB_080153c4;
                if (bVar24 < 0x19) {
                  cVar30 = '\x01';
                }
                else if (bVar24 < 0x1d) {
                  cVar30 = '\x02';
                }
                else if (bVar24 < 0x24) {
                  cVar30 = '\x03';
                }
                else if (bVar24 < 0x33) {
                  cVar30 = '\x04';
                }
                else if (bVar24 < 0x47) {
                  cVar30 = '\x05';
                }
                else if (bVar24 < 0x51) {
                  cVar30 = '\x06';
                }
                else if (bVar24 < 0x6a) {
                  cVar30 = '\a';
                }
                else if (bVar24 < 0x88) {
                  cVar30 = '\b';
                }
                else {
                  cVar30 = '\t';
                }
                if (*_LAB_080153c8 != '\0') {
                  cVar30 = '\0';
                }
                if (cVar30 != pcVar3[0xd]) {
                  iVar25 = FUN_080303b8(*_LAB_080153cc);
                  if (iVar25 != 0) {
                    FUN_08031fba(*puVar18,cVar30,1);
                  }
                  if ((cVar30 == '\0') && (iVar25 = FUN_080303b8(puVar18[1]), iVar25 != 0)) {
                    FUN_0802da22(puVar18[1],1);
                  }
                  pcVar3[0xd] = cVar30;
                }
              }
              else if (cVar30 == '\n') {
                if (pcVar3[9] != *_LAB_080153d0) {
                  pcVar3[7] = '\x01';
                  puVar5 = _LAB_080153d4;
                  iVar25 = FUN_080303b8(*_LAB_080153d4);
                  puVar6 = _LAB_080153d8;
                  if ((iVar25 != 0) && (iVar25 = FUN_080303b8(*_LAB_080153d8), iVar25 != 0)) {
                    if ((int)((uint)*pbVar19 << 0x18) < 0) {
                      FUN_0802e2d2(*puVar5,1);
                      FUN_0802e2d2(*puVar6,1);
                    }
                    else {
                      FUN_0802da22(*puVar5,1);
                      FUN_0802da22(*puVar6,1);
                    }
                  }
                  iVar25 = FUN_0802b744(*_LAB_080153dc);
                  if ((iVar25 == 0) && (iVar25 = FUN_080303b8(*puVar5), iVar25 != 0)) {
                    FUN_08031fba(*puVar5,*pbVar19 & 7,1);
                  }
                }
                pcVar3[9] = *pbVar19;
              }
              else if (cVar30 == '\v') {
                if (pcVar3[10] != *_LAB_080153e0) {
                  iVar25 = FUN_080303b8(_LAB_080153e4[1]);
                  if ((iVar25 != 0) && (iVar25 = FUN_080303b8(*puVar21), iVar25 != 0)) {
                    if (*pcVar20 == '\0') {
                      FUN_0802da22(puVar21[1],1);
                      FUN_0802e2d2(*puVar21,1);
                    }
                    else {
                      FUN_0802da22(*puVar21,1);
                      FUN_0802e2d2(puVar21[1],1);
                    }
                  }
                  pcVar3[10] = *pcVar20;
                  FUN_08015418();
                }
              }
              else if (cVar30 == '\f') {
                if (*_LAB_080153e8 == '\0') {
                  pcVar3[0x10] = '\0';
                }
                else {
                  bVar24 = pcVar3[0x10] + 1;
                  pcVar3[0x10] = bVar24;
                  puVar6 = _LAB_080153fc;
                  puVar5 = _LAB_080153ec;
                  if (bVar24 == 2) {
                    iVar25 = FUN_080303b8(*_LAB_080153fc);
                    if (iVar25 != 0) {
                      FUN_0802da22(*puVar6,1);
                    }
                    *pcVar9 = *pcVar10;
                    *pcVar10 = '\0';
                  }
                  else if (4 < bVar24) {
                    iVar25 = FUN_080303b8(*_LAB_080153ec);
                    if (iVar25 != 0) {
                      FUN_0802e2d2(*puVar5,1);
                    }
                    iVar25 = FUN_080303b8(*puVar5);
                    if (iVar25 != 0) {
                      FUN_0802e2d2(*puVar5,1);
                    }
                    puVar5 = _LAB_080153f0;
                    iVar25 = FUN_080303b8(*_LAB_080153f0);
                    if (iVar25 != 0) {
                      FUN_0802e2d2(*puVar5,1);
                    }
                    puVar5 = _LAB_080153f4;
                    iVar25 = FUN_080303b8(*_LAB_080153f4);
                    if (iVar25 != 0) {
                      FUN_0802e2d2(*puVar5,1);
                    }
                    *pcVar22 = '\0';
                    pcVar3[0x10] = '\0';
                    *_LAB_080153f8 = 0;
                    pcVar3[7] = '\x01';
                  }
                }
              }
              else if (cVar30 == '\r') {
                if (*_LAB_08015400 == '\x01') {
                  FUN_0802e2d2(*_LAB_08015404,1);
                }
                else {
                  FUN_0802da22(*_LAB_08015404,1);
                }
                if (pcVar3[0xf] != *pcVar23) {
                  pcVar3[0xf] = *pcVar23;
                  pcVar3[7] = '\x01';
                }
              }
            }
          }
        }
        else {
          pcVar3[0xc] = '\0';
          pcVar3[0xb] = '\x02';
          puVar6 = DAT_08014f5c;
          puVar5 = DAT_08014f54;
          if (*pcVar10 == '\0') {
            iVar25 = FUN_080303b8(*DAT_08014f5c);
            if ((iVar25 != 0) && (*DAT_08014f60 != '\0')) {
              FUN_0803118e(*puVar6,1);
            }
          }
          else {
            FUN_0802e292(*DAT_08014f54);
            FUN_08006f0c(*puVar5,*pcVar10);
            FUN_0803118e(*puVar5,1);
          }
          *pcVar3 = *pcVar10;
          pcVar3[7] = '\x01';
        }
        bVar32 = pcVar3[7] == '\0';
        do {
          if (bVar32) {
            return;
          }
          bVar32 = true;
        } while (*_LAB_08015408 == '\0');
        pcVar3[7] = '\0';
        FUN_08037ae8(*pcVar3);
      }
      else {
        *DAT_08014f50 = 0;
        if (cVar2 == '\0') {
          FUN_080345b4(0);
        }
        else if (cVar2 == '\x01') {
          FUN_0800fe78();
        }
      }
    }
    else {
      DAT_08014f10[0xb] = DAT_08014f10[0xb] + -1;
    }
  }
  return;
}

