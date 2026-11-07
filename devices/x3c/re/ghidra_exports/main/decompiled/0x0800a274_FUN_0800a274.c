// 0x0800a274 FUN_0800a274 size=1512

void FUN_0800a274(int param_1)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  char cVar4;
  undefined1 uVar5;
  short sVar6;
  undefined2 uVar7;
  short sVar8;
  uint uVar9;
  uint uVar10;
  undefined4 uVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  
  iVar13 = DAT_0800a694;
  iVar14 = DAT_0800a690;
  iVar3 = DAT_0800a68c;
  bVar2 = true;
  uVar11 = *(undefined4 *)(param_1 + 4);
  cVar4 = '\0';
  if (*(char *)(DAT_0800a68c + 8) == '\0') {
    cVar1 = *(char *)(param_1 + 8);
    if (cVar1 == '\x1c') {
      uVar10 = 0;
      do {
        if (*(byte *)(DAT_0800a690 + uVar10 * 6 + 3) >> 6 == 3) {
          cVar4 = cVar4 + '\x01';
        }
        uVar10 = uVar10 + 1 & 0xff;
      } while (uVar10 < 0x10);
      if (cVar4 != '\0') {
        if (*(short *)(DAT_0800a68c + 0x30) == 0) {
          if ((*(char *)(DAT_0800a68c + 0x19) != '\0') &&
             (iVar12 = FUN_0802fcb6(uVar11,0x20), iVar12 == 0)) {
            *(undefined1 *)(iVar3 + 0x19) = 0;
            sVar8 = thunk_FUN_08027774(*(undefined4 *)(param_1 + 4));
            sVar8 = sVar8 + -0x1e;
            iVar12 = (int)sVar8;
            cVar4 = (char)sVar8;
            if (iVar12 < 0) {
              if ((int)*(short *)(iVar3 + 0x2a) < -iVar12) {
                uVar10 = 0;
                do {
                  iVar14 = DAT_0800a690 + uVar10 * 6;
                  if (*(byte *)(iVar14 + 3) >> 6 == 3) {
                    cVar4 = *(char *)(DAT_0800a690 + uVar10 * 6);
                    if (cVar4 == '\0') {
                      uVar9 = FUN_0801a38c(*(undefined1 *)(iVar14 + 1),(int)-*(char *)(iVar3 + 0x2a)
                                          );
                      *(char *)(iVar14 + 1) = (char)uVar9;
                      if (uVar9 == *(byte *)(iVar3 + 4)) {
                        if (bVar2) {
                          bVar2 = false;
                          FUN_0803118e(*(undefined4 *)(iVar13 + uVar10 * 0x30),1);
                        }
                        FUN_0801b860(*(undefined4 *)(iVar13 + uVar10 * 0x30),0);
                      }
                    }
                    else if (cVar4 == '\x01') {
                      iVar12 = FUN_0801a294(*(undefined1 *)(iVar14 + 2),
                                            (int)-*(char *)(iVar3 + 0x2a));
                      *(char *)(iVar14 + 2) = (char)iVar12;
                      if (iVar12 == 0) {
                        if (bVar2) {
                          bVar2 = false;
                          FUN_0803118e(*(undefined4 *)(iVar13 + uVar10 * 0x30),1);
                        }
                        FUN_0801b860(*(undefined4 *)(iVar13 + uVar10 * 0x30),0);
                      }
                    }
                  }
                  uVar10 = uVar10 + 1 & 0xff;
                } while (uVar10 < 0x10);
                *(short *)(iVar3 + 0x2c) = *(short *)(iVar3 + 0x2c) + *(short *)(iVar3 + 0x2a);
                *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x28) - *(short *)(iVar3 + 0x2a);
                *(undefined2 *)(iVar3 + 0x2a) = 0;
                *(undefined1 *)(iVar3 + 6) = 0x50;
              }
              else {
                uVar10 = 0;
                do {
                  iVar13 = iVar14 + uVar10 * 6;
                  if (*(byte *)(iVar13 + 3) >> 6 == 3) {
                    cVar1 = *(char *)(iVar14 + uVar10 * 6);
                    if (cVar1 == '\0') {
                      uVar5 = FUN_0801a38c(*(undefined1 *)(iVar13 + 1),(int)cVar4);
                      *(undefined1 *)(iVar13 + 1) = uVar5;
                    }
                    else if (cVar1 == '\x01') {
                      uVar5 = FUN_0801a294(*(undefined1 *)(iVar13 + 2),(int)cVar4);
                      *(undefined1 *)(iVar13 + 2) = uVar5;
                    }
                  }
                  uVar10 = uVar10 + 1 & 0xff;
                } while (uVar10 < 0x10);
                *(short *)(iVar3 + 0x2a) = *(short *)(iVar3 + 0x2a) + sVar8;
                *(short *)(iVar3 + 0x2c) = *(short *)(iVar3 + 0x2c) - sVar8;
                *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x28) + sVar8;
                *(undefined1 *)(iVar3 + 6) = 0x50;
              }
            }
            else if (*(short *)(iVar3 + 0x2c) < iVar12) {
              uVar10 = 0;
              do {
                iVar14 = DAT_0800a690 + uVar10 * 6;
                if (*(byte *)(iVar14 + 3) >> 6 == 3) {
                  cVar4 = *(char *)(DAT_0800a690 + uVar10 * 6);
                  if (cVar4 == '\0') {
                    iVar12 = FUN_0801a38c(*(undefined1 *)(iVar14 + 1),(int)*(char *)(iVar3 + 0x2c));
                    *(char *)(iVar14 + 1) = (char)iVar12;
                    if (iVar12 == 0) {
                      if (bVar2) {
                        bVar2 = false;
                        FUN_0803118e(*(undefined4 *)(iVar13 + uVar10 * 0x30),1);
                      }
                      FUN_0801b860(*(undefined4 *)(iVar13 + uVar10 * 0x30),0);
                    }
                  }
                  else if (cVar4 == '\x01') {
                    iVar12 = FUN_0801a294(*(undefined1 *)(iVar14 + 2),(int)*(char *)(iVar3 + 0x2c));
                    *(char *)(iVar14 + 2) = (char)iVar12;
                    if (iVar12 == 0x3c) {
                      if (bVar2) {
                        bVar2 = false;
                        FUN_0803118e(*(undefined4 *)(iVar13 + uVar10 * 0x30),1);
                      }
                      FUN_0801b860(*(undefined4 *)(iVar13 + uVar10 * 0x30),0);
                    }
                  }
                }
                uVar10 = uVar10 + 1 & 0xff;
              } while (uVar10 < 0x10);
              *(short *)(iVar3 + 0x2a) = *(short *)(iVar3 + 0x2a) + *(short *)(iVar3 + 0x2c);
              *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x2c) + *(short *)(iVar3 + 0x28);
              *(undefined2 *)(iVar3 + 0x2c) = 0;
              *(undefined1 *)(iVar3 + 6) = 0x50;
            }
            else {
              *(short *)(iVar3 + 0x2a) = *(short *)(iVar3 + 0x2a) + sVar8;
              *(short *)(iVar3 + 0x2c) = *(short *)(iVar3 + 0x2c) - sVar8;
              uVar10 = 0;
              do {
                iVar13 = iVar14 + uVar10 * 6;
                if (*(byte *)(iVar13 + 3) >> 6 == 3) {
                  cVar1 = *(char *)(iVar14 + uVar10 * 6);
                  if (cVar1 == '\0') {
                    uVar5 = FUN_0801a38c(*(undefined1 *)(iVar13 + 1),(int)cVar4);
                    *(undefined1 *)(iVar13 + 1) = uVar5;
                  }
                  else if (cVar1 == '\x01') {
                    uVar5 = FUN_0801a294(*(undefined1 *)(iVar13 + 2),(int)cVar4);
                    *(undefined1 *)(iVar13 + 2) = uVar5;
                  }
                }
                uVar10 = uVar10 + 1 & 0xff;
              } while (uVar10 < 0x10);
              *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x28) + sVar8;
              *(undefined1 *)(iVar3 + 6) = 0x50;
            }
            thunk_FUN_0802782e(*(undefined4 *)(param_1 + 4),0x1e,1);
            return;
          }
          uVar7 = thunk_FUN_08027774(*(undefined4 *)(param_1 + 4));
          *(undefined2 *)(iVar3 + 0x30) = uVar7;
        }
        else {
          sVar6 = thunk_FUN_08027774(uVar11);
          sVar8 = sVar6 - *(short *)(iVar3 + 0x30);
          iVar12 = (int)sVar8;
          *(short *)(iVar3 + 0x30) = sVar6;
          cVar4 = (char)sVar8;
          if (iVar12 < 0) {
            if ((int)*(short *)(iVar3 + 0x2a) < -iVar12) {
              uVar10 = 0;
              do {
                iVar14 = DAT_0800a690 + uVar10 * 6;
                if (*(byte *)(iVar14 + 3) >> 6 == 3) {
                  cVar4 = *(char *)(DAT_0800a690 + uVar10 * 6);
                  if (cVar4 == '\0') {
                    uVar9 = FUN_0801a38c(*(undefined1 *)(iVar14 + 1),(int)-*(char *)(iVar3 + 0x2a));
                    *(char *)(iVar14 + 1) = (char)uVar9;
                    if (uVar9 == *(byte *)(iVar3 + 4)) {
                      if (bVar2) {
                        bVar2 = false;
                        FUN_0803118e(*(undefined4 *)(iVar13 + uVar10 * 0x30),1);
                      }
                      FUN_0801b860(*(undefined4 *)(iVar13 + uVar10 * 0x30),0);
                    }
                  }
                  else if (cVar4 == '\x01') {
                    iVar12 = FUN_0801a294(*(undefined1 *)(iVar14 + 2),(int)-*(char *)(iVar3 + 0x2a))
                    ;
                    *(char *)(iVar14 + 2) = (char)iVar12;
                    if (iVar12 == 0) {
                      if (bVar2) {
                        bVar2 = false;
                        FUN_0803118e(*(undefined4 *)(iVar13 + uVar10 * 0x30),1);
                      }
                      FUN_0801b860(*(undefined4 *)(iVar13 + uVar10 * 0x30),0);
                    }
                  }
                }
                uVar10 = uVar10 + 1 & 0xff;
              } while (uVar10 < 0x10);
              *(short *)(iVar3 + 0x2c) = *(short *)(iVar3 + 0x2c) + *(short *)(iVar3 + 0x2a);
              *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x28) - *(short *)(iVar3 + 0x2a);
              *(undefined2 *)(iVar3 + 0x2a) = 0;
              *(undefined1 *)(iVar3 + 6) = 0x50;
            }
            else {
              uVar10 = 0;
              do {
                iVar13 = iVar14 + uVar10 * 6;
                if (*(byte *)(iVar13 + 3) >> 6 == 3) {
                  cVar1 = *(char *)(iVar14 + uVar10 * 6);
                  if (cVar1 == '\0') {
                    uVar5 = FUN_0801a38c(*(undefined1 *)(iVar13 + 1),(int)cVar4);
                    *(undefined1 *)(iVar13 + 1) = uVar5;
                  }
                  else if (cVar1 == '\x01') {
                    uVar5 = FUN_0801a294(*(undefined1 *)(iVar13 + 2),(int)cVar4);
                    *(undefined1 *)(iVar13 + 2) = uVar5;
                  }
                }
                uVar10 = uVar10 + 1 & 0xff;
              } while (uVar10 < 0x10);
              *(short *)(iVar3 + 0x2a) = *(short *)(iVar3 + 0x2a) + sVar8;
              *(short *)(iVar3 + 0x2c) = *(short *)(iVar3 + 0x2c) - sVar8;
              *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x28) + sVar8;
              *(undefined1 *)(iVar3 + 6) = 0x50;
            }
          }
          else if (*(short *)(iVar3 + 0x2c) < iVar12) {
            uVar10 = 0;
            do {
              iVar14 = DAT_0800a86c + uVar10 * 6;
              if (*(byte *)(iVar14 + 3) >> 6 == 3) {
                cVar4 = *(char *)(DAT_0800a86c + uVar10 * 6);
                if (cVar4 == '\0') {
                  iVar12 = FUN_0801a38c(*(undefined1 *)(iVar14 + 1),(int)*(char *)(iVar3 + 0x2c));
                  *(char *)(iVar14 + 1) = (char)iVar12;
                  if (iVar12 == 0) {
                    if (bVar2) {
                      bVar2 = false;
                      FUN_0803118e(*(undefined4 *)(iVar13 + uVar10 * 0x30),1);
                    }
                    FUN_0801b860(*(undefined4 *)(iVar13 + uVar10 * 0x30),0);
                  }
                }
                else if (cVar4 == '\x01') {
                  iVar12 = FUN_0801a294(*(undefined1 *)(iVar14 + 2),(int)*(char *)(iVar3 + 0x2c));
                  *(char *)(iVar14 + 2) = (char)iVar12;
                  if (iVar12 == 0x3c) {
                    if (bVar2) {
                      bVar2 = false;
                      FUN_0803118e(*(undefined4 *)(iVar13 + uVar10 * 0x30),1);
                    }
                    FUN_0801b860(*(undefined4 *)(iVar13 + uVar10 * 0x30),0);
                  }
                }
              }
              uVar10 = uVar10 + 1 & 0xff;
            } while (uVar10 < 0x10);
            *(short *)(iVar3 + 0x2a) = *(short *)(iVar3 + 0x2a) + *(short *)(iVar3 + 0x2c);
            *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x2c) + *(short *)(iVar3 + 0x28);
            *(undefined2 *)(iVar3 + 0x2c) = 0;
            *(undefined1 *)(iVar3 + 6) = 0x50;
          }
          else {
            uVar10 = 0;
            do {
              iVar13 = iVar14 + uVar10 * 6;
              if (*(byte *)(iVar13 + 3) >> 6 == 3) {
                cVar1 = *(char *)(iVar14 + uVar10 * 6);
                if (cVar1 == '\0') {
                  uVar5 = FUN_0801a38c(*(undefined1 *)(iVar13 + 1),(int)cVar4);
                  *(undefined1 *)(iVar13 + 1) = uVar5;
                }
                else if (cVar1 == '\x01') {
                  uVar5 = FUN_0801a294(*(undefined1 *)(iVar13 + 2),(int)cVar4);
                  *(undefined1 *)(iVar13 + 2) = uVar5;
                }
              }
              uVar10 = uVar10 + 1 & 0xff;
            } while (uVar10 < 0x10);
            *(short *)(iVar3 + 0x2a) = *(short *)(iVar3 + 0x2a) + sVar8;
            *(short *)(iVar3 + 0x2c) = *(short *)(iVar3 + 0x2c) - sVar8;
            *(short *)(iVar3 + 0x28) = *(short *)(iVar3 + 0x28) + sVar8;
            *(undefined1 *)(iVar3 + 6) = 0x50;
          }
        }
        *(undefined1 *)(iVar3 + 0x19) = 0;
      }
    }
    else if (cVar1 == '\x01') {
      if (*(short *)(DAT_0800a68c + 0x24) == 0 && *(short *)(DAT_0800a68c + 0x26) == 0) {
        FUN_08009580(DAT_0800a870 + -2);
        *(undefined2 *)(iVar3 + 0x24) = *(undefined2 *)(iVar3 + 0x2a);
        *(undefined2 *)(iVar3 + 0x26) = *(undefined2 *)(iVar3 + 0x2c);
      }
      if (*(short *)(iVar3 + 0x2a) != 0 || *(short *)(iVar3 + 0x2c) != 0) {
        *(undefined2 *)(iVar3 + 0x30) = 0;
      }
    }
    else {
      if (cVar1 == '\b') {
        thunk_FUN_0802782e(uVar11,0x1e,1);
        return;
      }
      if (cVar1 == '\x04') {
        *(undefined1 *)(DAT_0800a68c + 0x19) = 1;
      }
      else if (cVar1 == '\x06') {
        uVar10 = 0;
        do {
          if (*(byte *)(DAT_0800a690 + uVar10 * 6 + 3) >> 6 == 3) {
            cVar4 = cVar4 + '\x01';
          }
          uVar10 = uVar10 + 1 & 0xff;
        } while (uVar10 < 0x10);
        if (cVar4 != '\0') {
          *(undefined1 *)(DAT_0800a68c + 6) = 0x50;
        }
      }
    }
  }
  return;
}

