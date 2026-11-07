// 0x08019190 FUN_08019190 size=792

void FUN_08019190(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
                 int param_7,int param_8,int param_9,int param_10,int param_11,int param_12)

{
  short sVar1;
  short sVar2;
  short sVar3;
  bool bVar4;
  undefined2 uVar5;
  int *piVar6;
  int iVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  short *psVar13;
  uint uVar14;
  int iVar15;
  short *psVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  int iVar20;
  int iVar21;
  uint uVar22;
  int iVar23;
  short *psVar24;
  short local_60;
  
  local_60 = 0;
  if (param_12 == 4) {
    bVar4 = false;
    iVar18 = 2;
  }
  else if (param_12 == 5) {
    bVar4 = true;
    iVar18 = 3;
  }
  else if (param_12 == 6) {
    bVar4 = true;
    iVar18 = 2;
    piVar6 = (int *)FUN_08018528();
    local_60 = (short)*(undefined4 *)(*piVar6 + 0x44);
  }
  else {
    if (param_12 != 0x14) {
      return;
    }
    bVar4 = true;
    iVar18 = 2;
  }
  iVar17 = 0;
  do {
    if (param_9 <= iVar17) {
      return;
    }
    uVar14 = param_5 + (iVar17 * param_7 >> 8);
    uVar11 = param_6 + (iVar17 * param_8 >> 8);
    iVar10 = (int)uVar14 >> 8;
    iVar7 = (int)uVar11 >> 8;
    if ((((iVar10 < 0) || (param_2 <= iVar10)) || (iVar7 < 0)) || (param_3 <= iVar7)) {
      *(undefined1 *)(param_11 + iVar17) = 0;
    }
    else {
      uVar14 = uVar14 & 0xff;
      uVar11 = uVar11 & 0xff;
      if (uVar14 < 0x80) {
        iVar15 = -1;
        iVar19 = 0x7f - uVar14;
      }
      else {
        iVar15 = 1;
        iVar19 = uVar14 - 0x80;
      }
      if (uVar11 < 0x80) {
        iVar23 = -1;
        iVar12 = 0x7f - uVar11;
      }
      else {
        iVar23 = 1;
        iVar12 = uVar11 - 0x80;
      }
      psVar13 = (short *)(iVar10 * iVar18 + iVar18 * iVar7 * param_4 + param_1);
      iVar20 = iVar10 + iVar15;
      if (((iVar20 < 0) || (param_2 + -1 < iVar20)) ||
         ((iVar21 = iVar7 + iVar23, iVar21 < 0 || (param_3 + -1 < iVar21)))) {
        sVar1 = *psVar13;
        *(short *)(param_10 + iVar17 * 2) = sVar1;
        if (param_12 == 5) {
          uVar11 = (uint)*(byte *)(psVar13 + 1);
        }
        else if (param_12 == 6) {
          if (sVar1 == local_60) {
            uVar11 = 0;
          }
          else {
            uVar11 = 0xff;
          }
        }
        else if (param_12 == 0x14) {
          uVar11 = (uint)*(byte *)(iVar7 * param_4 + param_1 + iVar10 + param_4 * param_3 * 2);
        }
        else {
          uVar11 = 0xff;
        }
        if (((iVar10 == 0) && (iVar15 < 0)) || ((iVar10 == param_2 + -1 && (0 < iVar15)))) {
          *(char *)(param_11 + iVar17) = (char)(uVar11 * (iVar19 * -2 + 0xff) >> 8);
        }
        else if (((iVar7 == 0) && (iVar23 < 0)) || ((iVar7 == param_3 + -1 && (0 < iVar23)))) {
          *(char *)(param_11 + iVar17) = (char)(uVar11 * (iVar12 * -2 + 0xff) >> 8);
        }
        else {
          *(undefined1 *)(param_11 + iVar17) = 0;
        }
      }
      else {
        psVar16 = (short *)(iVar15 * iVar18 + (int)psVar13);
        psVar24 = (short *)(iVar23 * param_4 * iVar18 + (int)psVar13);
        if (bVar4) {
          if (param_12 == 5) {
            uVar14 = (uint)*(byte *)(psVar13 + 1);
            uVar22 = (uint)*(byte *)(psVar24 + 1);
            uVar11 = (uint)*(byte *)(psVar16 + 1);
          }
          else if (param_12 == 0x14) {
            iVar15 = param_1 + param_4 * param_3 * 2;
            iVar10 = iVar15 + iVar10;
            uVar14 = (uint)*(byte *)(iVar7 * param_4 + iVar10);
            uVar11 = (uint)*(byte *)(iVar7 * param_4 + iVar15 + iVar20);
            uVar22 = (uint)*(byte *)(iVar21 * param_4 + iVar10);
          }
          else if (param_12 == 6) {
            if (((*psVar13 == local_60) || (*psVar24 == local_60)) || (*psVar16 == local_60)) {
              *(undefined1 *)(param_11 + iVar17) = 0;
              goto LAB_0801949a;
            }
            uVar14 = 0xff;
            uVar22 = 0xff;
            uVar11 = 0xff;
          }
          else {
            uVar14 = 0xff;
            uVar22 = 0xff;
            uVar11 = 0xff;
          }
          if (uVar22 != uVar14) {
            uVar22 = (uVar14 * (iVar12 * -2 + 0x100) + uVar22 * iVar12 * 2 & 0xffff) >> 8;
          }
          if (uVar11 != uVar14) {
            uVar11 = (uVar14 * (iVar19 * -2 + 0x100) + iVar19 * 2 * uVar11 & 0xffff) >> 8;
          }
          uVar11 = uVar11 + uVar22 >> 1;
          *(char *)(param_11 + iVar17) = (char)uVar11;
          if (uVar11 == 0) goto LAB_0801949a;
          sVar1 = *psVar13;
          sVar3 = *psVar24;
          sVar2 = *psVar16;
        }
        else {
          sVar1 = *psVar13;
          sVar2 = *psVar16;
          sVar3 = *psVar24;
          *(undefined1 *)(param_11 + iVar17) = 0xff;
        }
        if ((sVar1 == sVar3) && (sVar1 == sVar2)) {
          *(short *)(param_10 + iVar17 * 2) = sVar1;
        }
        else {
          uVar8 = FUN_08027e9c(sVar3,sVar1,iVar12 * 2 & 0xff);
          uVar9 = FUN_08027e9c(sVar2,sVar1,iVar19 * 2 & 0xff);
          uVar5 = FUN_08027e9c(uVar9,uVar8,0x7f);
          *(undefined2 *)(param_10 + iVar17 * 2) = uVar5;
        }
      }
    }
LAB_0801949a:
    iVar17 = iVar17 + 1;
  } while( true );
}

