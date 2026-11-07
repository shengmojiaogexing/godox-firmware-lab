// 0x0802e7a8 FUN_0802e7a8 size=1624

void FUN_0802e7a8(undefined4 param_1,undefined4 param_2)

{
  char cVar1;
  bool bVar2;
  undefined4 uVar3;
  int iVar4;
  char *pcVar5;
  uint uVar6;
  undefined4 uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  uint uVar12;
  undefined1 auStack_c4 [4];
  undefined4 local_c0;
  undefined4 local_bc;
  int *local_b8;
  int *local_b4;
  undefined4 local_90;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  undefined1 local_67;
  int local_58;
  undefined1 local_4e;
  undefined1 local_44;
  byte local_43;
  undefined1 local_38 [8];
  int local_30;
  int local_2c;
  int local_28;
  uint local_24;
  int local_1c;
  
  iVar8 = FUN_0802acf4(param_2);
  iVar9 = FUN_0802acfc(param_2);
  if (iVar8 == 1) {
    FUN_0802da8e(iVar9,0x20);
    return;
  }
  if (iVar8 == 8) {
    FUN_0802e34c(iVar9,0x20);
    FUN_0802ad6c(param_2);
    iVar8 = FUN_0802c682();
    if ((iVar8 == 0) && (iVar8 = FUN_0802fc9e(iVar9,8), iVar8 != 0)) {
      uVar6 = FUN_0802f7b8(iVar9);
      if ((uVar6 & 1) == 0) {
        FUN_0802da8e(iVar9,1);
      }
      else {
        FUN_0802e34c(iVar9,1);
      }
      FUN_0802ad74(iVar9,0x1c,0);
      return;
    }
  }
  else {
    if (iVar8 == 3) {
      FUN_0802e34c(iVar9,0x20);
      return;
    }
    if (iVar8 == 0x2a) {
      uVar6 = FUN_0802ebf4(iVar9);
      for (uVar12 = 0; uVar12 < uVar6; uVar12 = uVar12 + 1) {
        FUN_08030436(*(undefined4 *)(**(int **)(iVar9 + 8) + uVar12 * 4));
      }
    }
    else if (iVar8 == 0xd) {
      iVar8 = FUN_0802fc9e(iVar9,8);
      if (iVar8 == 0) {
        iVar8 = FUN_0802fc9e(iVar9,0x810);
        if ((iVar8 != 0) && (iVar8 = FUN_0803033e(iVar9), iVar8 == 0)) {
          iVar8 = FUN_0802ef1c(iVar9);
          iVar4 = FUN_0802efdc(iVar9);
          pcVar5 = (char *)FUN_0802ad6c(param_2);
          cVar1 = *pcVar5;
          if (cVar1 == '\x12') {
            iVar8 = FUN_0802f0dc(iVar9);
            iVar4 = FUN_0802ed88(iVar9);
            FUN_0803121c(iVar9,iVar8 + ((int)(iVar4 + ((uint)(iVar4 >> 0x1f) >> 0x1e)) >> 2),0);
            return;
          }
          if (cVar1 == '\x11') {
            iVar8 = FUN_0802f0dc(iVar9);
            iVar4 = FUN_0802ed88(iVar9);
            FUN_0803121c(iVar9,iVar8 - ((int)(iVar4 + ((uint)(iVar4 >> 0x1f) >> 0x1e)) >> 2),0);
            return;
          }
          if (cVar1 == '\x13') {
            uVar6 = FUN_0802eec8(iVar9);
            if (((uVar6 & 3) == 0) || ((iVar8 < 1 && (iVar4 < 1)))) {
              iVar8 = FUN_0802f0dc(iVar9);
              iVar4 = FUN_0802ed88(iVar9);
              FUN_0803121c(iVar9,iVar8 + ((int)(iVar4 + ((uint)(iVar4 >> 0x1f) >> 0x1e)) >> 2),0);
              return;
            }
            iVar8 = FUN_0802f0cc(iVar9);
            iVar4 = FUN_0802fc5e(iVar9);
            FUN_080311f0(iVar9,iVar8 + ((int)(iVar4 + ((uint)(iVar4 >> 0x1f) >> 0x1e)) >> 2),0);
            return;
          }
          if (cVar1 == '\x14') {
            uVar6 = FUN_0802eec8(iVar9);
            if (((uVar6 & 3) == 0) || ((iVar8 < 1 && (iVar4 < 1)))) {
              iVar8 = FUN_0802f0dc(iVar9);
              iVar4 = FUN_0802ed88(iVar9);
              FUN_0803121c(iVar9,iVar8 - ((int)(iVar4 + ((uint)(iVar4 >> 0x1f) >> 0x1e)) >> 2),0);
              return;
            }
            iVar8 = FUN_0802f0cc(iVar9);
            iVar4 = FUN_0802fc5e(iVar9);
            FUN_080311f0(iVar9,iVar8 - ((int)(iVar4 + ((uint)(iVar4 >> 0x1f) >> 0x1e)) >> 2),0);
            return;
          }
        }
      }
      else {
        pcVar5 = (char *)FUN_0802ad6c(param_2);
        cVar1 = *pcVar5;
        if ((cVar1 == '\x13') || (cVar1 == '\x11')) {
          FUN_0802da8e(iVar9,1);
        }
        else if ((cVar1 == '\x14') || (cVar1 == '\x12')) {
          FUN_0802e34c(iVar9,1);
        }
        if (cVar1 != '\n') {
          FUN_0802ad74(iVar9,0x1c,0);
          return;
        }
      }
    }
    else {
      if (iVar8 == 0xe) {
        iVar8 = FUN_0802fc9e(iVar9,0x400);
        if (iVar8 != 0) {
          FUN_080311b0(iVar9,1);
        }
        FUN_0802ed7a(iVar9);
        iVar8 = FUN_0802b744();
        uVar6 = 2;
        iVar4 = FUN_0802c5fc();
        if (iVar4 == 0) {
          FUN_0802ad22(param_2);
        }
        iVar4 = FUN_0802c69e();
        if ((iVar4 == 2) || (iVar4 == 4)) {
          uVar6 = 6;
        }
        if (iVar8 == 0) {
          FUN_0802da8e(iVar9,uVar6);
          FUN_0802e34c(iVar9,8);
          return;
        }
        FUN_0802da8e(iVar9,uVar6 | 8);
        return;
      }
      if (iVar8 == 9) {
        FUN_0802da8e(iVar9,0x40);
        return;
      }
      if (iVar8 == 10) {
        FUN_0802e34c(iVar9,0x40);
        iVar8 = FUN_0802f76a(iVar9);
        if (iVar8 == 2) {
          FUN_0802f0ec(iVar9,&local_28,local_38);
          FUN_08030308(iVar9,&local_28);
          FUN_08030308(iVar9,local_38);
        }
      }
      else {
        if (iVar8 == 0xf) {
          FUN_0802e34c(iVar9,0xe);
          return;
        }
        if (iVar8 == 0x29) {
          iVar8 = FUN_0802f7bc(iVar9,0);
          iVar4 = FUN_0802f8ae(iVar9,0);
          if (iVar4 != 0 || iVar8 != 0) {
            FUN_08030436(iVar9);
          }
          uVar6 = FUN_0802ebf4(iVar9);
          for (uVar12 = 0; uVar12 < uVar6; uVar12 = uVar12 + 1) {
            FUN_08030436(*(undefined4 *)(**(int **)(iVar9 + 8) + uVar12 * 4));
          }
        }
        else if (iVar8 == 0x22) {
          iVar8 = FUN_0802f9bc(iVar9,0,1);
          local_1c = iVar8;
          iVar4 = FUN_0802f9bc(iVar9,0,4);
          local_1c = iVar4;
          iVar10 = FUN_0802f7bc(iVar9,0);
          iVar11 = FUN_0802f8ae(iVar9,0);
          if (((iVar11 != 0 || iVar10 != 0) || (iVar8 == DAT_0802eb84)) || (iVar4 == DAT_0802eb84))
          {
            FUN_08030436(iVar9);
            return;
          }
        }
        else {
          if (iVar8 == 0x13) {
            uVar7 = FUN_0802e024(iVar9,0);
            FUN_0802add2(param_2,uVar7);
            return;
          }
          if (((iVar8 == 0x15) || (iVar8 == 0x18)) || (iVar8 == 0x12)) {
            iVar8 = FUN_0802acf4(param_2);
            iVar9 = FUN_0802ad70(param_2);
            uVar7 = DAT_0802e784;
            if (iVar8 == 0x12) {
              pcVar5 = (char *)FUN_0802ad6c(param_2);
              if (*pcVar5 != '\x02') {
                iVar8 = FUN_0802f876(iVar9,0);
                if (iVar8 == 0) {
                  uVar7 = FUN_0802fa4c(iVar9,0);
                  iVar8 = FUN_0802faca(iVar9,0);
                  iVar4 = FUN_0802fabe(iVar9,0);
                  FUN_080273e8(&local_30,iVar9 + 0x14);
                  local_30 = local_30 - iVar8;
                  local_28 = local_28 + iVar8;
                  local_2c = local_2c - iVar4;
                  local_24 = local_24 + iVar4;
                  iVar8 = FUN_080174c4(*(undefined4 *)(pcVar5 + 4),&local_30,uVar7);
                  if (iVar8 == 0) {
                    *pcVar5 = '\x01';
                  }
                  else {
                    uVar6 = FUN_0802f83a(iVar9,0);
                    if (uVar6 < 0xfd) {
                      *pcVar5 = '\x01';
                    }
                    else {
                      *pcVar5 = '\0';
                    }
                  }
                }
                else {
                  *pcVar5 = '\x02';
                }
              }
            }
            else if (iVar8 == 0x15) {
              uVar3 = FUN_0802ad00(param_2);
              FUN_08029a38(&local_6c);
              iVar8 = FUN_0802f852(iVar9,0);
              if (iVar8 != 0) {
                local_43 = local_43 | 1;
              }
              FUN_0802fff4(iVar9,0,&local_6c);
              iVar8 = FUN_0802faca(iVar9,0);
              iVar4 = FUN_0802fabe(iVar9,0);
              FUN_080273e8(&local_7c);
              local_7c = local_7c - iVar8;
              local_74 = local_74 + iVar8;
              local_78 = local_78 - iVar4;
              local_70 = local_70 + iVar4;
              FUN_0802e788(auStack_c4,uVar3);
              local_c0 = uVar7;
              local_bc = 0;
              local_b4 = &local_6c;
              local_b8 = &local_7c;
              local_90 = 0;
              FUN_0802ad74(iVar9,0x1a,auStack_c4);
              iVar4 = FUN_0802f876(iVar9,0);
              iVar8 = local_58;
              if ((iVar4 == 0) || (local_6c == 0)) {
                bVar2 = false;
              }
              else {
                bVar2 = true;
              }
              if (bVar2) {
                local_58 = 0;
              }
              FUN_08029a14(uVar3,&local_6c,&local_7c);
              if (bVar2) {
                uVar7 = FUN_0802d624(0x24);
                FUN_080298a0(uVar7,iVar9 + 0x14,local_6c,0);
                FUN_08028e78(uVar7,iVar9 + 0x160);
                if (iVar8 != 0) {
                  local_67 = 0;
                  local_44 = 0;
                  local_38[0] = 0;
                  local_24 = local_24 & 0xffffff00;
                  local_58 = iVar8;
                  FUN_08029a14(uVar3,&local_6c,&local_7c);
                }
              }
              FUN_0802ad74(iVar9,0x1b,auStack_c4);
            }
            else if (iVar8 == 0x18) {
              uVar3 = FUN_0802ad00(param_2);
              FUN_0801f26c(iVar9,uVar3);
              iVar8 = FUN_0802f876(iVar9,0);
              if ((iVar8 != 0) && (iVar8 = FUN_080299c4(iVar9 + 0x160), iVar8 != 0)) {
                FUN_08029358(iVar8);
                FUN_0802d6c0(iVar8);
              }
              iVar8 = FUN_0802f852(iVar9,0);
              if (iVar8 != 0) {
                FUN_08029a38(&local_6c);
                local_67 = 0;
                local_4e = 0;
                local_38[0] = 0;
                local_24 = local_24 & 0xffffff00;
                FUN_0802fff4(iVar9,0,&local_6c);
                iVar8 = FUN_0802faca(iVar9,0);
                iVar4 = FUN_0802fabe(iVar9,0);
                FUN_080273e8(&local_7c,iVar9 + 0x14);
                local_7c = local_7c - iVar8;
                local_74 = local_74 + iVar8;
                local_78 = local_78 - iVar4;
                local_70 = local_70 + iVar4;
                FUN_0802e788(auStack_c4,uVar3);
                local_c0 = uVar7;
                local_bc = 1;
                local_b4 = &local_6c;
                local_b8 = &local_7c;
                local_90 = 0;
                FUN_0802ad74(iVar9,0x1a,auStack_c4);
                FUN_08029a14(uVar3,&local_6c,&local_7c);
                FUN_0802ad74(iVar9,0x1b,auStack_c4);
              }
            }
            return;
          }
        }
      }
    }
  }
  return;
}

