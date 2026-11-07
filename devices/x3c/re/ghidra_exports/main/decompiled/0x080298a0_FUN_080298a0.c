// 0x080298a0 FUN_080298a0 size=956

void FUN_080298a0(undefined4 *param_1,int *param_2,int param_3,undefined1 param_4)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  undefined1 *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  int *piVar11;
  int iVar12;
  uint uVar13;
  int *piVar14;
  int iVar15;
  uint local_50 [4];
  int local_40 [4];
  undefined1 auStack_30 [4];
  uint local_2c [5];
  
  iVar8 = (param_2[2] - *param_2) + 1;
  iVar12 = (param_2[3] - param_2[1]) + 1;
  if (iVar12 <= iVar8) {
    iVar8 = iVar12;
  }
  if (iVar8 >> 1 < param_3) {
    param_3 = iVar8 >> 1;
  }
  if (param_3 < 0) {
    param_3 = 0;
  }
  FUN_08027430(param_1 + 2);
  param_1[6] = param_3;
  *(undefined1 *)(param_1 + 7) = param_4;
  *param_1 = DAT_080299bc;
  *(undefined1 *)(param_1 + 1) = 2;
  if (param_3 == 0) {
    param_1[8] = 0;
    return;
  }
  uVar9 = 0;
  iVar8 = 1000;
  do {
    iVar12 = param_3 >> 4;
    if (*(int *)(DAT_080299c0 + uVar9 * 0x1c + 0x18) == param_3) {
      iVar10 = DAT_080299c0 + uVar9 * 0x1c;
      *(int *)(iVar10 + 0x14) = *(int *)(iVar10 + 0x14) + 1;
      iVar15 = iVar12;
      if (param_3 < 0x10) {
        iVar15 = 1;
      }
      if (iVar15 + *(int *)(iVar10 + 0x10) < 1000) {
        if (param_3 < 0x10) {
          iVar12 = 1;
        }
        iVar8 = *(int *)(iVar10 + 0x10) + iVar12;
      }
      *(int *)(iVar10 + 0x10) = iVar8;
      param_1[8] = iVar10;
      return;
    }
    uVar9 = uVar9 + 1;
  } while (uVar9 < 4);
  piVar14 = (int *)0x0;
  uVar9 = 0;
  do {
    piVar11 = (int *)(DAT_080299c0 + uVar9 * 0x1c);
    if (piVar11[5] == 0) {
      if (piVar14 == (int *)0x0) {
        piVar14 = piVar11;
      }
      else if (piVar11[4] < piVar14[4]) {
        piVar14 = piVar11;
      }
    }
    uVar9 = uVar9 + 1;
  } while (uVar9 < 4);
  if (piVar14 == (int *)0x0) {
    piVar14 = (int *)FUN_0802d5c4(0x1c);
    if (piVar14 == (int *)0x0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    FUN_0802d8d6(piVar14,0x1c);
    piVar14[4] = -1;
  }
  else {
    piVar14[5] = piVar14[5] + 1;
    piVar14[4] = 0;
    iVar15 = iVar12;
    if (param_3 < 0x10) {
      iVar15 = 1;
    }
    if (iVar15 < 1000) {
      if (param_3 < 0x10) {
        iVar12 = 1;
      }
      iVar8 = iVar12;
    }
    piVar14[4] = iVar8;
  }
  param_1[8] = piVar14;
  if (param_3 != 0) {
    piVar14[6] = param_3;
    if (*piVar14 != 0) {
      FUN_0802d6e4();
    }
    puVar5 = (undefined1 *)FUN_0802d5c4(param_3 * 6 + 6);
    *piVar14 = (int)puVar5;
    if (puVar5 == (undefined1 *)0x0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    piVar14[1] = (int)puVar5;
    piVar14[3] = (int)(puVar5 + param_3 * 2 + 2);
    piVar14[2] = (int)(puVar5 + param_3 * 4 + 4);
    if (param_3 == 1) {
      *puVar5 = 0xb4;
      *(undefined2 *)piVar14[3] = 0;
      *(undefined2 *)(piVar14[3] + 2) = 1;
      *(undefined2 *)piVar14[2] = 0;
    }
    else {
      iVar8 = FUN_0802d624((param_3 + 1) * 0x10);
      iVar15 = iVar8 + (param_3 + 1) * 8;
      iVar10 = 0;
      FUN_0801adde(local_2c,auStack_30,param_3 << 2);
      iVar12 = 0;
      local_40[0] = (int)local_2c[0] >> 2;
      local_50[0] = 0;
      while (iVar7 = FUN_0801add0(local_2c), iVar7 != 0) {
        iVar7 = 0;
        do {
          FUN_0801adec(local_2c,auStack_30);
          iVar6 = FUN_0801add0(local_2c);
          if (iVar6 == 0) break;
          local_40[iVar7] = (int)local_2c[0] >> 2;
          local_50[iVar7] = local_2c[0] & 3;
          iVar7 = iVar7 + 1;
        } while (iVar7 < 4);
        if (iVar7 != 4) break;
        cVar2 = (char)local_50[0];
        cVar3 = (char)local_50[1];
        cVar1 = (char)local_50[2];
        cVar4 = (char)local_50[3];
        if (local_40[0] == local_40[3]) {
          *(int *)(iVar8 + iVar12 * 4) = local_40[0];
          *(int *)(iVar15 + iVar12 * 4) = iVar10;
          cVar1 = cVar2 + cVar3 + cVar1 + cVar4;
          *(char *)(piVar14[1] + iVar12) = cVar1;
          *(char *)(piVar14[1] + iVar12) = cVar1 * '\x10';
          iVar12 = iVar12 + 1;
        }
        else if (local_40[0] == local_40[1]) {
          if (local_40[0] == local_40[2]) {
            *(int *)(iVar8 + iVar12 * 4) = local_40[0];
            *(int *)(iVar15 + iVar12 * 4) = iVar10;
            cVar1 = cVar2 + cVar3 + cVar1;
            *(char *)(piVar14[1] + iVar12) = cVar1;
            *(char *)(piVar14[1] + iVar12) = cVar1 * '\x10';
            iVar7 = iVar12 + 1;
            *(int *)(iVar8 + iVar7 * 4) = local_40[0] + -1;
            *(int *)(iVar15 + iVar7 * 4) = iVar10;
            *(char *)(piVar14[1] + iVar7) = cVar4 + '\f';
            *(char *)(piVar14[1] + iVar7) = (cVar4 + '\f') * '\x10';
            iVar12 = iVar12 + 2;
          }
          else {
            *(int *)(iVar8 + iVar12 * 4) = local_40[0];
            *(int *)(iVar15 + iVar12 * 4) = iVar10;
            *(char *)(piVar14[1] + iVar12) = cVar2 + cVar3;
            *(char *)(piVar14[1] + iVar12) = (cVar2 + cVar3) * '\x10';
            iVar7 = iVar12 + 1;
            *(int *)(iVar8 + iVar7 * 4) = local_40[0] + -1;
            *(int *)(iVar15 + iVar7 * 4) = iVar10;
            cVar1 = cVar1 + cVar4 + '\b';
            *(char *)(piVar14[1] + iVar7) = cVar1;
            *(char *)(piVar14[1] + iVar7) = cVar1 * '\x10';
            iVar12 = iVar12 + 2;
          }
        }
        else {
          *(int *)(iVar8 + iVar12 * 4) = local_40[0];
          *(int *)(iVar15 + iVar12 * 4) = iVar10;
          *(char *)(piVar14[1] + iVar12) = cVar2;
          *(char *)(piVar14[1] + iVar12) = (char)(local_50[0] << 4);
          iVar7 = iVar12 + 1;
          *(int *)(iVar8 + iVar7 * 4) = local_40[0] + -1;
          *(int *)(iVar15 + iVar7 * 4) = iVar10;
          cVar1 = cVar4 + '\x04' + cVar1 + cVar3;
          *(char *)(piVar14[1] + iVar7) = cVar1;
          *(char *)(piVar14[1] + iVar7) = cVar1 * '\x10';
          iVar12 = iVar12 + 2;
        }
        iVar10 = iVar10 + 1;
      }
      iVar10 = param_3 * 0x2d3 >> 10;
      if ((*(int *)(iVar8 + (iVar12 + -1) * 4) != iVar10) ||
         (*(int *)(iVar15 + (iVar12 + -1) * 4) != iVar10)) {
        iVar7 = param_3 * 0x2d3 + iVar10 * -0x400;
        if (iVar7 < 0x201) {
          iVar7 = iVar7 * iVar7 * 2 >> 0x10;
        }
        else {
          iVar7 = 0xf - ((0x400 - iVar7) * (0x400 - iVar7) * 2 >> 0x10);
        }
        *(int *)(iVar8 + iVar12 * 4) = iVar10;
        *(int *)(iVar15 + iVar12 * 4) = iVar10;
        *(char *)(piVar14[1] + iVar12) = (char)iVar7;
        *(char *)(piVar14[1] + iVar12) = (char)(iVar7 << 4);
        iVar12 = iVar12 + 1;
      }
      for (iVar10 = iVar12 + -2; -1 < iVar10; iVar10 = iVar10 + -1) {
        *(undefined4 *)(iVar8 + iVar12 * 4) = *(undefined4 *)(iVar15 + iVar10 * 4);
        *(undefined4 *)(iVar15 + iVar12 * 4) = *(undefined4 *)(iVar8 + iVar10 * 4);
        *(undefined1 *)(piVar14[1] + iVar12) = *(undefined1 *)(piVar14[1] + iVar10);
        iVar12 = iVar12 + 1;
      }
      iVar7 = 0;
      iVar10 = 0;
      *(undefined2 *)piVar14[3] = 0;
      while (iVar10 < iVar12) {
        *(short *)(piVar14[3] + iVar7 * 2) = (short)iVar10;
        *(undefined2 *)(piVar14[2] + iVar7 * 2) = *(undefined2 *)(iVar8 + iVar10 * 4);
        for (; (*(int *)(iVar15 + iVar10 * 4) == iVar7 && (iVar10 < iVar12)); iVar10 = iVar10 + 1) {
          uVar13 = *(uint *)(iVar8 + iVar10 * 4);
          uVar9 = (uint)*(ushort *)(piVar14[2] + iVar7 * 2);
          if ((int)uVar13 <= (int)uVar9) {
            uVar9 = uVar13;
          }
          *(short *)(piVar14[2] + iVar7 * 2) = (short)uVar9;
        }
        iVar7 = iVar7 + 1;
      }
      FUN_0802d6c0(iVar8);
    }
  }
  return;
}

