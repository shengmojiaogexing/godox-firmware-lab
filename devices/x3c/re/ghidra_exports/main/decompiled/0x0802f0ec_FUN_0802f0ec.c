// 0x0802f0ec FUN_0802f0ec size=1662

void FUN_0802f0ec(int param_1,int *param_2,int *param_3)

{
  bool bVar1;
  bool bVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  int iVar20;
  bool bVar21;
  
  FUN_08027518(param_2,0,0,0xffffffff,0xffffffff);
  FUN_08027518(param_3,0,0,0xffffffff,0xffffffff);
  iVar4 = FUN_0802fc9e(param_1,0x10);
  if ((iVar4 != 0) && (iVar4 = FUN_0802f76a(param_1), iVar4 != 0)) {
    iVar5 = FUN_0802c610(0);
    if (iVar4 == 2) {
      while ((iVar5 != 0 && (iVar6 = FUN_0802c682(iVar5), iVar6 != param_1))) {
        iVar5 = FUN_0802c610(iVar5);
      }
      if (iVar5 == 0) {
        return;
      }
    }
    iVar6 = FUN_0802f0bc(param_1);
    iVar7 = FUN_0802ee28(param_1);
    iVar8 = FUN_0802ef1c(param_1);
    iVar9 = FUN_0802efdc(param_1);
    uVar10 = FUN_0802eec8(param_1);
    bVar1 = false;
    if (((uVar10 & 0xc) != 0) &&
       (((iVar4 == 1 || ((iVar4 == 3 && ((0 < iVar6 || (0 < iVar7)))))) ||
        ((iVar4 == 2 && (iVar11 = FUN_0802c660(iVar5), iVar11 == 0xc)))))) {
      bVar1 = true;
    }
    bVar2 = false;
    if (((uVar10 & 3) != 0) &&
       (((iVar4 == 1 || ((iVar4 == 3 && ((0 < iVar8 || (0 < iVar9)))))) ||
        ((iVar4 == 2 && (iVar4 = FUN_0802c660(iVar5), iVar4 == 3)))))) {
      bVar2 = true;
    }
    if (bVar2 || bVar1) {
      iVar4 = FUN_0802f7ec(param_1,0x10000);
      bVar21 = iVar4 != 1;
      iVar12 = FUN_0802f99e(param_1,0x10000);
      iVar13 = FUN_0802f908(param_1,0x10000);
      iVar14 = FUN_0802f93e(param_1,0x10000);
      iVar15 = FUN_0802f968(param_1,0x10000);
      iVar4 = FUN_0802f9bc(param_1,0x10000,1);
      iVar5 = FUN_0802ed88(param_1);
      iVar11 = FUN_0802fc5e(param_1);
      if (bVar1) {
        iVar20 = iVar4;
      }
      else {
        iVar20 = 0;
      }
      if (bVar2) {
        iVar16 = iVar4;
      }
      else {
        iVar16 = 0;
      }
      bVar3 = FUN_0802f9bc(param_1,0x10000,0x21);
      if ((1 < bVar3) || (bVar3 = FUN_0802f9bc(param_1,0x10000,0x31), 1 < bVar3)) {
        iVar6 = iVar7 + iVar6 + iVar5;
        bVar1 = !bVar1;
        do {
          if (bVar1) goto LAB_0802f494;
          bVar1 = true;
        } while (iVar6 == 0);
        param_3[1] = *(int *)(param_1 + 0x18);
        param_3[3] = *(int *)(param_1 + 0x20);
        if (bVar21) {
          iVar17 = *(int *)(param_1 + 0x1c) - iVar15;
          param_3[2] = iVar17;
          *param_3 = (iVar17 - iVar4) + 1;
        }
        else {
          iVar17 = *(int *)(param_1 + 0x14) + iVar14;
          *param_3 = iVar17;
          param_3[2] = iVar17 + iVar4 + -1;
        }
        iVar17 = ((iVar5 - iVar12) - iVar13) - iVar16;
        iVar18 = (iVar5 * iVar17) / iVar6;
        iVar19 = FUN_08028228(0);
        if (iVar19 * 10 + 0x50 < 0x140) {
          iVar19 = 1;
        }
        else {
          iVar19 = FUN_08028228(0);
          iVar19 = (iVar19 * 10 + 0x50) / 0xa0;
        }
        if (iVar18 <= iVar19) {
          iVar19 = FUN_08028228(0);
          if (iVar19 * 10 + 0x50 < 0x140) {
            iVar18 = 1;
          }
          else {
            iVar19 = FUN_08028228(0);
            iVar18 = (iVar19 * 10 + 0x50) / 0xa0;
          }
        }
        iVar17 = iVar17 - iVar18;
        if (iVar6 - iVar5 < 1) {
          param_3[1] = iVar12 + *(int *)(param_1 + 0x18);
          param_3[3] = ((*(int *)(param_1 + 0x20) - iVar13) - iVar16) + -1;
        }
        else {
          iVar5 = (iVar17 - (iVar17 * iVar7) / (iVar6 - iVar5)) + iVar12 + *(int *)(param_1 + 0x18);
          param_3[1] = iVar5;
          param_3[3] = iVar18 + -1 + iVar5;
          iVar12 = iVar12 + *(int *)(param_1 + 0x18);
          if (iVar5 < iVar12) {
            param_3[1] = iVar12;
            iVar5 = FUN_08028228(0);
            if (iVar5 * 10 + 0x50 < 0x140) {
              iVar5 = 1;
            }
            else {
              iVar5 = FUN_08028228(0);
              iVar5 = (iVar5 * 10 + 0x50) / 0xa0;
            }
            if (param_3[3] < iVar5 + param_3[1]) {
              iVar5 = FUN_08028228(0);
              if (iVar5 * 10 + 0x50 < 0x140) {
                iVar5 = 1;
              }
              else {
                iVar5 = FUN_08028228(0);
                iVar5 = (iVar5 * 10 + 0x50) / 0xa0;
              }
              param_3[3] = iVar5 + param_3[1];
            }
          }
          iVar5 = (*(int *)(param_1 + 0x20) - iVar16) - iVar13;
          if (iVar5 < param_3[3]) {
            param_3[3] = iVar5;
            iVar5 = FUN_08028228(0);
            if (iVar5 * 10 + 0x50 < 0x140) {
              iVar5 = 1;
            }
            else {
              iVar5 = FUN_08028228(0);
              iVar5 = (iVar5 * 10 + 0x50) / 0xa0;
            }
            if (param_3[3] - iVar5 < param_3[1]) {
              iVar5 = FUN_08028228(0);
              if (iVar5 * 10 + 0x50 < 0x140) {
                iVar5 = 1;
              }
              else {
                iVar5 = FUN_08028228(0);
                iVar5 = (iVar5 * 10 + 0x50) / 0xa0;
              }
              param_3[1] = param_3[3] - iVar5;
            }
          }
        }
LAB_0802f494:
        iVar5 = iVar9 + iVar11 + iVar8;
        if ((bVar2) && (iVar5 != 0)) {
          iVar13 = *(int *)(param_1 + 0x20) - iVar13;
          param_2[3] = iVar13;
          param_2[1] = (iVar13 - iVar4) + 1;
          *param_2 = *(int *)(param_1 + 0x14);
          param_2[2] = *(int *)(param_1 + 0x1c);
          iVar4 = ((iVar11 - iVar14) - iVar15) - iVar20;
          iVar7 = (iVar11 * iVar4) / iVar5;
          iVar6 = FUN_08028228(0);
          if (iVar6 * 10 + 0x50 < 0x140) {
            iVar6 = 1;
          }
          else {
            iVar6 = FUN_08028228(0);
            iVar6 = (iVar6 * 10 + 0x50) / 0xa0;
          }
          if (iVar7 <= iVar6) {
            iVar6 = FUN_08028228(0);
            if (iVar6 * 10 + 0x50 < 0x140) {
              iVar7 = 1;
            }
            else {
              iVar6 = FUN_08028228(0);
              iVar7 = (iVar6 * 10 + 0x50) / 0xa0;
            }
          }
          iVar4 = iVar4 - iVar7;
          if (iVar5 - iVar11 < 1) {
            if (bVar21) {
              *param_2 = *(int *)(param_1 + 0x14) + iVar14;
              param_2[2] = ((*(int *)(param_1 + 0x1c) - iVar15) - iVar20) + -1;
            }
            else {
              *param_2 = *(int *)(param_1 + 0x14) + iVar14 + iVar20 + -1;
              param_2[2] = *(int *)(param_1 + 0x1c) - iVar15;
            }
          }
          else {
            iVar4 = iVar4 - (iVar4 * iVar9) / (iVar5 - iVar11);
            if (bVar21) {
              iVar4 = iVar4 + iVar14 + *(int *)(param_1 + 0x14);
              *param_2 = iVar4;
              param_2[2] = iVar7 + -1 + iVar4;
              iVar14 = *(int *)(param_1 + 0x14) + iVar14;
              if (iVar4 < iVar14) {
                *param_2 = iVar14;
                iVar4 = FUN_08028228(0);
                if (iVar4 * 10 + 0x50 < 0x140) {
                  iVar4 = 1;
                }
                else {
                  iVar4 = FUN_08028228(0);
                  iVar4 = (iVar4 * 10 + 0x50) / 0xa0;
                }
                if (param_2[2] < iVar4 + *param_2) {
                  iVar4 = FUN_08028228(0);
                  if (iVar4 * 10 + 0x50 < 0x140) {
                    iVar4 = 1;
                  }
                  else {
                    iVar4 = FUN_08028228(0);
                    iVar4 = (iVar4 * 10 + 0x50) / 0xa0;
                  }
                  param_2[2] = iVar4 + *param_2;
                }
              }
              iVar15 = (*(int *)(param_1 + 0x1c) - iVar20) - iVar15;
              if (iVar15 < param_2[2]) {
                param_2[2] = iVar15;
                iVar4 = FUN_08028228(0);
                if (iVar4 * 10 + 0x50 < 0x140) {
                  iVar4 = 1;
                }
                else {
                  iVar4 = FUN_08028228(0);
                  iVar4 = (iVar4 * 10 + 0x50) / 0xa0;
                }
                if (param_2[2] - iVar4 < *param_2) {
                  iVar4 = FUN_08028228(0);
                  if (iVar4 * 10 + 0x50 < 0x140) {
                    iVar4 = 1;
                  }
                  else {
                    iVar4 = FUN_08028228(0);
                    iVar4 = (iVar4 * 10 + 0x50) / 0xa0;
                  }
                  *param_2 = param_2[2] - iVar4;
                }
              }
            }
            else {
              iVar5 = iVar4 + *(int *)(param_1 + 0x14) + iVar14 + iVar20;
              *param_2 = iVar5;
              param_2[2] = iVar7 + -1 + iVar5;
              iVar4 = *(int *)(param_1 + 0x14) + iVar14 + iVar20;
              if (iVar5 < iVar4) {
                *param_2 = iVar4;
                iVar4 = FUN_08028228(0);
                if (iVar4 * 10 + 0x50 < 0x140) {
                  iVar4 = 1;
                }
                else {
                  iVar4 = FUN_08028228(0);
                  iVar4 = (iVar4 * 10 + 0x50) / 0xa0;
                }
                if (param_2[2] < iVar4 + *param_2) {
                  iVar4 = FUN_08028228(0);
                  if (iVar4 * 10 + 0x50 < 0x140) {
                    iVar4 = 1;
                  }
                  else {
                    iVar4 = FUN_08028228(0);
                    iVar4 = (iVar4 * 10 + 0x50) / 0xa0;
                  }
                  param_2[2] = iVar4 + *param_2;
                }
              }
              iVar15 = *(int *)(param_1 + 0x1c) - iVar15;
              if (iVar15 < param_2[2]) {
                param_2[2] = iVar15;
                iVar4 = FUN_08028228(0);
                if (iVar4 * 10 + 0x50 < 0x140) {
                  iVar4 = 1;
                }
                else {
                  iVar4 = FUN_08028228(0);
                  iVar4 = (iVar4 * 10 + 0x50) / 0xa0;
                }
                if (param_2[2] - iVar4 < *param_2) {
                  iVar4 = FUN_08028228(0);
                  if (iVar4 * 10 + 0x50 < 0x140) {
                    iVar4 = 1;
                  }
                  else {
                    iVar4 = FUN_08028228(0);
                    iVar4 = (iVar4 * 10 + 0x50) / 0xa0;
                  }
                  *param_2 = param_2[2] - iVar4;
                }
              }
            }
          }
        }
      }
    }
  }
  return;
}

