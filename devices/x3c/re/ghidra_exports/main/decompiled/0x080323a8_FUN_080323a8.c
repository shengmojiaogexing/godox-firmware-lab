// 0x080323a8 FUN_080323a8 size=2196

void FUN_080323a8(undefined4 param_1,undefined4 param_2)

{
  bool bVar1;
  char cVar2;
  byte bVar3;
  bool bVar4;
  undefined1 uVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  undefined4 *puVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int *piVar18;
  char *pcVar19;
  int iVar20;
  int *piVar21;
  undefined1 auStack_110 [76];
  undefined1 auStack_c4 [4];
  undefined4 local_c0;
  undefined4 local_bc;
  int local_b8;
  undefined1 *local_b4;
  undefined4 local_90;
  undefined4 local_8c;
  undefined1 auStack_7c [64];
  undefined1 auStack_3c [8];
  int local_34;
  int local_30;
  undefined4 local_2c;
  undefined4 local_28;
  
  local_2c = param_1;
  local_28 = param_2;
  iVar9 = FUN_0802eb88(DAT_080327a8,param_2);
  if (iVar9 == 1) {
    iVar9 = FUN_0802acf4(local_28);
    iVar10 = FUN_0802ad70(local_28);
    iVar11 = FUN_08032a9a(iVar10);
    uVar6 = local_28;
    piVar18 = DAT_080327ac;
    if (iVar9 == 0x11) {
      puVar12 = (undefined4 *)FUN_0802ad6c(local_28);
      if (*(int *)(iVar10 + 8) == 0) {
        uVar6 = 0;
      }
      else {
        uVar6 = *(undefined4 *)(*(int *)(iVar10 + 8) + 0x18);
      }
      FUN_08027466(auStack_3c,iVar10 + 0x80);
      FUN_080274e4(auStack_3c,uVar6,uVar6);
      iVar9 = FUN_0801761a(auStack_3c,*puVar12,0);
      *(char *)(puVar12 + 1) = (char)iVar9;
      if ((iVar9 == 0) && (iVar11 == 2)) {
        FUN_08027466(auStack_3c,iVar10 + 0x70);
        FUN_080274e4(auStack_3c,uVar6,uVar6);
        uVar5 = FUN_0801761a(auStack_3c,*puVar12,0);
        *(undefined1 *)(puVar12 + 1) = uVar5;
      }
    }
    else if (iVar9 == 1) {
      *(undefined1 *)(iVar10 + 0x95) = 0;
      iVar9 = FUN_080334d8();
      *piVar18 = iVar9;
      uVar6 = FUN_0802c5fc();
      FUN_0802c634(uVar6,DAT_080327ac + 1);
      FUN_080302d0(iVar10);
      *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) | 1;
      if ((iVar11 == 0) || (iVar11 == 1)) {
        *(int *)(iVar10 + 0x90) = iVar10 + 0x2c;
      }
      else if (iVar11 == 2) {
        uVar6 = FUN_0802c5fc();
        FUN_0802c634(uVar6,&local_34);
        iVar9 = FUN_0802fc5e(iVar10);
        iVar11 = FUN_0802ed88(iVar10);
        iVar7 = FUN_0802f810(iVar10,0);
        if (iVar9 < iVar11) {
          iVar9 = *(int *)(iVar10 + 0x84);
          if (local_30 < iVar9) {
            *(int *)(iVar10 + 0x90) = iVar10 + 0x2c;
          }
          else if (*(int *)(iVar10 + 0x7c) < local_30) {
            *(int *)(iVar10 + 0x90) = iVar10 + 0x38;
          }
          else {
            iVar7 = *(int *)(iVar10 + 0x74) +
                    (*(int *)(iVar10 + 0x7c) - *(int *)(iVar10 + 0x74)) / 2;
            iVar11 = iVar7 - local_30;
            if (iVar11 < 1) {
              iVar11 = local_30 - iVar7;
            }
            iVar9 = iVar9 + (*(int *)(iVar10 + 0x8c) - iVar9) / 2;
            iVar7 = iVar9 - local_30;
            if (iVar7 < 1) {
              iVar7 = local_30 - iVar9;
            }
            if (iVar7 < iVar11) {
              *(int *)(iVar10 + 0x90) = iVar10 + 0x2c;
              *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) & 0xfd;
            }
            else {
              *(int *)(iVar10 + 0x90) = iVar10 + 0x38;
              *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) | 2;
            }
          }
        }
        else if (((iVar7 == 1) || (local_34 <= *(int *)(iVar10 + 0x88))) &&
                ((iVar7 != 1 || (*(int *)(iVar10 + 0x80) <= local_34)))) {
          if (((iVar7 == 1) || (*(int *)(iVar10 + 0x70) <= local_34)) &&
             ((iVar7 != 1 || (local_34 <= *(int *)(iVar10 + 0x78))))) {
            iVar11 = *(int *)(iVar10 + 0x70) +
                     (*(int *)(iVar10 + 0x78) - *(int *)(iVar10 + 0x70)) / 2;
            iVar9 = iVar11 - local_34;
            if (iVar9 < 1) {
              iVar9 = local_34 - iVar11;
            }
            iVar7 = *(int *)(iVar10 + 0x80) +
                    (*(int *)(iVar10 + 0x88) - *(int *)(iVar10 + 0x80)) / 2;
            iVar11 = iVar7 - local_34;
            if (iVar11 < 1) {
              iVar11 = local_34 - iVar7;
            }
            if (iVar11 < iVar9) {
              *(int *)(iVar10 + 0x90) = iVar10 + 0x2c;
              *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) & 0xfd;
            }
            else {
              *(int *)(iVar10 + 0x90) = iVar10 + 0x38;
              *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) | 2;
            }
          }
          else {
            *(int *)(iVar10 + 0x90) = iVar10 + 0x38;
          }
        }
        else {
          *(int *)(iVar10 + 0x90) = iVar10 + 0x2c;
        }
      }
    }
    else {
      piVar21 = DAT_080327ac + 1;
      if ((iVar9 == 2) && (*(int *)(iVar10 + 0x90) != 0)) {
        if ((int)((uint)*(byte *)(iVar10 + 0x94) << 0x19) < 0) {
          iVar9 = FUN_080334d8();
          if (((-1 < (int)((uint)*(byte *)(iVar10 + 0x94) << 0x18)) &&
              (-1 < (int)((uint)*(byte *)(iVar10 + 0x94) << 0x1d))) &&
             ((uint)(iVar9 - *piVar18) < 300)) {
            uVar6 = FUN_0802c5fc();
            FUN_0802c634(uVar6,&local_34);
            if ((piVar18[2] + 0x14 < local_30) || (local_30 < piVar18[2] + -0x14)) {
              *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) | 4;
            }
            iVar11 = *piVar21;
            if ((iVar11 + 0x14 < local_34) || (local_34 < iVar11 + -0x14)) {
              *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) | 0x80;
            }
          }
          if (((uint)(iVar9 - *piVar18) < 300) &&
             (-1 < (int)((uint)*(byte *)(iVar10 + 0x94) << 0x18))) {
            return;
          }
          if ((int)((uint)*(byte *)(iVar10 + 0x94) << 0x1d) < 0) {
            return;
          }
        }
        *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) | 0x20;
        FUN_0802e2d2(iVar10,0x200);
        uVar6 = FUN_0802c5fc();
        iVar9 = FUN_0802c69e(uVar6);
        if (iVar9 == 1) {
          FUN_0802c634(uVar6,&local_34);
          iVar9 = FUN_0802f810(iVar10,0);
          iVar11 = FUN_0802fc5e(iVar10);
          iVar7 = FUN_0802ed88(iVar10);
          iVar8 = FUN_0802f950(iVar10,0);
          iVar14 = FUN_0802f97a(iVar10,0);
          iVar15 = FUN_0802f9b0(iVar10,0);
          iVar16 = FUN_0802f91a(iVar10,0);
          iVar17 = *(int *)(iVar10 + 0x30);
          iVar20 = *(int *)(iVar10 + 0x34);
          if (iVar11 < iVar7) {
            iVar9 = ((iVar20 - iVar17) * -(local_30 - (iVar16 + *(int *)(iVar10 + 0x20)))) /
                    ((iVar7 - iVar16) - iVar15);
          }
          else {
            if (iVar9 == 1) {
              local_34 = (*(int *)(iVar10 + 0x1c) - iVar14) - local_34;
            }
            else {
              local_34 = local_34 - (*(int *)(iVar10 + 0x14) + iVar8);
            }
            iVar9 = ((iVar20 - iVar17) * local_34) / ((iVar11 - iVar8) - iVar14);
          }
          iVar9 = iVar9 + iVar17;
          piVar18 = *(int **)(iVar10 + 0x90);
          if (piVar18 == (int *)(iVar10 + 0x38)) {
            iVar20 = *(int *)(iVar10 + 0x2c);
          }
          else {
            iVar17 = *(int *)(iVar10 + 0x38);
          }
          if (iVar9 < iVar17) {
            iVar9 = iVar17;
          }
          else if (iVar20 < iVar9) {
            iVar9 = iVar20;
          }
          if (*piVar18 != iVar9) {
            *piVar18 = iVar9;
            FUN_080302d0(iVar10);
            FUN_0802ad74(iVar10,0x1c,0);
          }
        }
      }
      else if ((iVar9 == 8) || (iVar9 == 3)) {
        *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) & 0xfe;
        *(undefined4 *)(iVar10 + 0x90) = 0;
        FUN_080302d0(iVar10);
        iVar9 = FUN_0802fc5e(iVar10);
        FUN_0802ed88(iVar10);
        if ((int)((uint)*(byte *)(iVar10 + 0x94) << 0x19) < 0) {
          FUN_0802e2d2(iVar10,0x300);
          uVar13 = (uint)*(byte *)(iVar10 + 0x94);
          if ((((-1 < (int)(uVar13 << 0x1c)) && (-1 < (int)(uVar13 << 0x1d))) &&
              (-1 < (int)(uVar13 << 0x1a))) && (-1 < (int)(uVar13 << 0x18))) {
            iVar11 = *piVar21;
            iVar7 = *(int *)(iVar10 + 0x14);
            if ((iVar7 < iVar11) && (iVar11 < (iVar9 * 3) / 10 + iVar7)) {
              if ((int)(uVar13 << 0x1e) < 0) {
                iVar9 = thunk_FUN_08027756(iVar10);
                thunk_FUN_080277e2(iVar10,iVar9 + -1,1);
              }
              else {
                iVar9 = thunk_FUN_08027774(iVar10);
                thunk_FUN_0802782e(iVar10,iVar9 + -1,1);
              }
              FUN_0802ad74(iVar10,0x1c,0);
            }
            else if ((iVar7 + (iVar9 * 7) / 10 < iVar11) && (iVar11 < *(int *)(iVar10 + 0x1c))) {
              if ((int)(uVar13 << 0x1e) < 0) {
                iVar9 = thunk_FUN_08027756(iVar10);
                thunk_FUN_080277e2(iVar10,iVar9 + 1,1);
              }
              else {
                iVar9 = thunk_FUN_08027774(iVar10);
                thunk_FUN_0802782e(iVar10,iVar9 + 1,1);
              }
              FUN_0802ad74(iVar10,0x1c,0);
            }
            *(undefined1 *)(iVar10 + 0x95) = 1;
          }
          *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) & 0x7f;
        }
        *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) & 0xdb;
        FUN_0802da22(iVar10,0x200);
        uVar6 = FUN_0802ed7a(iVar10);
        iVar9 = FUN_0802b744(uVar6);
        FUN_0802c5fc();
        iVar11 = FUN_0802c69e();
        if ((iVar11 == 4) && (iVar9 != 0)) {
          iVar9 = FUN_08032a9a(iVar10);
          if (iVar9 != 2) {
            FUN_0802b8b2(uVar6,0);
            return;
          }
          bVar3 = *(byte *)(iVar10 + 0x94);
          if ((int)((uint)bVar3 << 0x1e) < 0) {
            *(byte *)(iVar10 + 0x94) = bVar3 & 0xfd;
            FUN_0802b8b2(uVar6,0);
            return;
          }
          *(byte *)(iVar10 + 0x94) = bVar3 | 2;
        }
      }
      else if (iVar9 == 0xe) {
        FUN_0802c5fc();
        iVar9 = FUN_0802c69e();
        if ((iVar9 == 4) || (iVar9 == 2)) {
          *(byte *)(iVar10 + 0x94) = *(byte *)(iVar10 + 0x94) & 0xfd;
        }
      }
      else {
        if (iVar9 == 0x29) {
          FUN_08030c20(iVar10);
          return;
        }
        if (iVar9 == 0x13) {
          iVar11 = FUN_0802f950(iVar10,0x30000);
          iVar7 = FUN_0802f97a(iVar10,0x30000);
          iVar9 = FUN_0802f9b0(iVar10,0x30000);
          iVar8 = FUN_0802f91a(iVar10,0x30000);
          iVar14 = FUN_0802f9bc(iVar10,0x30000,0x6c);
          local_34 = iVar14;
          iVar15 = FUN_0802fad0(iVar10,0x30000);
          local_30 = FUN_0802fac4(iVar10,0x30000);
          iVar16 = FUN_0802fc5e(iVar10);
          iVar17 = FUN_0802ed88(iVar10);
          if (iVar16 + iVar15 * 2 < iVar17 + local_30 * 2) {
            iVar16 = FUN_0802fc5e(iVar10);
            iVar16 = iVar16 + iVar15 * 2;
          }
          else {
            iVar16 = FUN_0802ed88(iVar10);
            iVar16 = iVar16 + local_30 * 2;
          }
          if (iVar7 < iVar11) {
            iVar15 = iVar11;
          }
          else {
            iVar15 = iVar7;
          }
          if (iVar9 < iVar8) {
            iVar17 = iVar8;
          }
          else {
            iVar17 = iVar9;
          }
          if (iVar17 < iVar15) {
            if (iVar7 < iVar11) {
              iVar9 = iVar11;
            }
            else {
              iVar9 = iVar7;
            }
          }
          else if (iVar9 < iVar8) {
            iVar9 = iVar8;
          }
          iVar10 = FUN_0802e024(iVar10,0x30000);
          iVar10 = iVar10 + iVar9 + ((iVar16 >> 1) * iVar14 >> 8) + 2;
          piVar18 = (int *)FUN_0802ad6c(local_28);
          if (iVar10 < *piVar18) {
            iVar10 = *piVar18;
          }
          *piVar18 = iVar10;
        }
        else if (iVar9 == 0xd) {
          pcVar19 = (char *)FUN_0802ad6c(local_28);
          cVar2 = *pcVar19;
          if ((cVar2 == '\x13') || (cVar2 == '\x11')) {
            if ((int)((uint)*(byte *)(iVar10 + 0x94) << 0x1e) < 0) {
              iVar9 = thunk_FUN_08027756(iVar10);
              thunk_FUN_080277e2(iVar10,iVar9 + 1,1);
            }
            else {
              iVar9 = thunk_FUN_08027774(iVar10);
              thunk_FUN_0802782e(iVar10,iVar9 + 1,1);
            }
            FUN_0802ad74(iVar10,0x1c,0);
            return;
          }
          if ((cVar2 == '\x14') || (cVar2 == '\x12')) {
            if ((int)((uint)*(byte *)(iVar10 + 0x94) << 0x1e) < 0) {
              iVar9 = thunk_FUN_08027756(iVar10);
              thunk_FUN_080277e2(iVar10,iVar9 + -1,1);
            }
            else {
              iVar9 = thunk_FUN_08027774(iVar10);
              thunk_FUN_0802782e(iVar10,iVar9 + -1,1);
            }
            FUN_0802ad74(iVar10,0x1c,0);
            return;
          }
        }
        else if (iVar9 == 0x15) {
          iVar10 = FUN_0802ad70(local_28);
          uVar6 = FUN_0802ad00(uVar6);
          iVar11 = FUN_0802f810(iVar10,0);
          iVar7 = FUN_0802fc5e(iVar10);
          iVar9 = FUN_0802ed88(iVar10);
          bVar1 = iVar9 <= iVar7;
          if (!bVar1) {
            iVar9 = iVar7;
          }
          bVar4 = false;
          if ((((*(byte *)(iVar10 + 0x6c) & 3) == 1) && (*(int *)(iVar10 + 0x30) < 0)) &&
             (0 < *(int *)(iVar10 + 0x34))) {
            bVar4 = true;
          }
          if (bVar1) {
            if (bVar4) {
              if (*(int *)(iVar10 + 0x2c) < 0) {
                if (iVar11 == 1) {
                  local_30 = *(int *)(iVar10 + 0x44);
                }
                else {
                  local_30 = *(int *)(iVar10 + 0x3c);
                }
              }
              else if (iVar11 == 1) {
                local_30 = *(int *)(iVar10 + 0x3c);
              }
              else {
                local_30 = *(int *)(iVar10 + 0x44);
              }
            }
            else if (iVar11 == 1) {
              local_30 = *(int *)(iVar10 + 0x3c);
            }
            else {
              local_30 = *(int *)(iVar10 + 0x44);
            }
          }
          else if (bVar4) {
            if (*(int *)(iVar10 + 0x2c) < 0) {
              local_2c = *(undefined4 *)(iVar10 + 0x48);
            }
            else {
              local_2c = *(undefined4 *)(iVar10 + 0x40);
            }
          }
          else {
            local_2c = *(undefined4 *)(iVar10 + 0x40);
          }
          FUN_08029a38(auStack_7c);
          FUN_0802fff4(iVar10,0x30000,auStack_7c);
          FUN_08035f30(iVar10,&local_30,iVar9,bVar1);
          iVar7 = iVar10 + 0x80;
          FUN_08027466(iVar7,&local_30);
          FUN_0802e788(auStack_c4,uVar6);
          local_90 = 0x30000;
          local_c0 = DAT_0801dc44;
          local_bc = 0;
          local_8c = 0;
          local_b8 = iVar7;
          local_b4 = auStack_7c;
          iVar8 = FUN_08032a9a(iVar10);
          if (iVar8 == 2) {
            FUN_0802d764(auStack_110,auStack_7c,0x4c);
            FUN_0802ad74(iVar10,0x1a,auStack_c4);
            FUN_08029a14(uVar6,auStack_7c,iVar7);
            FUN_0802ad74(iVar10,0x1b,auStack_c4);
            if (bVar1) {
              if (iVar11 == 1) {
                local_30 = *(int *)(iVar10 + 0x44);
              }
              else {
                local_30 = *(int *)(iVar10 + 0x3c);
              }
            }
            else {
              local_2c = *(undefined4 *)(iVar10 + 0x48);
            }
            FUN_08035f30(iVar10,&local_30,iVar9,bVar1);
            iVar9 = iVar10 + 0x70;
            FUN_08027466(iVar9,&local_30);
            FUN_0802d764(auStack_7c,auStack_110,0x4c);
            local_bc = 1;
            local_8c = 1;
            local_b8 = iVar9;
            local_b4 = auStack_7c;
            FUN_0802ad74(iVar10,0x1a,auStack_c4);
            FUN_08029a14(uVar6,auStack_7c,iVar9);
            FUN_0802ad74(iVar10,0x1b,auStack_c4);
          }
          else {
            FUN_0802ad74(iVar10,0x1a,auStack_c4);
            FUN_08029a14(uVar6,auStack_7c,iVar7);
            FUN_0802ad74(iVar10,0x1b,auStack_c4);
          }
          return;
        }
      }
    }
  }
  return;
}

