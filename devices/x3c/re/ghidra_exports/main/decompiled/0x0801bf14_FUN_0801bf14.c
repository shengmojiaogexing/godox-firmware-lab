// 0x0801bf14 FUN_0801bf14 size=490

undefined4 FUN_0801bf14(int param_1,short *param_2,int *param_3,undefined4 param_4)

{
  byte bVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  short *apsStack_5c [4];
  int local_4c;
  undefined1 auStack_48 [4];
  int local_44 [2];
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  
  if (*(byte *)((int)param_2 + 0xf) < 3) {
    uVar2 = 1;
  }
  else {
    iVar3 = FUN_080179fc(param_4,*(undefined4 *)(param_2 + 6),*(undefined4 *)(param_2 + 10));
    if (iVar3 == 0) {
      uVar2 = 0;
    }
    else {
      iVar4 = FUN_0802b958(*(byte *)(iVar3 + 0x14) & 0x1f);
      if (iVar4 == 0) {
        bVar1 = *(byte *)(iVar3 + 0x14) & 0x1f;
        if (bVar1 == 0xe) {
          iVar4 = 0xe;
        }
        else if (bVar1 == 0x14) {
          iVar4 = 0x14;
        }
        else {
          iVar4 = FUN_0802b938();
          if (iVar4 == 0) {
            iVar4 = 4;
          }
          else {
            iVar4 = 5;
          }
        }
      }
      else {
        iVar4 = 6;
      }
      if ((iVar4 == 0xe) && ((*param_2 != 0 || (param_2[1] != 0x100)))) {
        iVar4 = 5;
        *(undefined4 *)(iVar3 + 0x18) = 0;
      }
      if (*(int *)(iVar3 + 0x20) == 0) {
        if (*(int *)(iVar3 + 0x18) == 0) {
          iVar5 = FUN_08017484(&local_34,*(undefined4 *)(param_1 + 8),param_3);
          if (iVar5 == 0) {
            thunk_FUN_0802c016(iVar3);
            return 1;
          }
          local_38 = (local_2c - local_34) + 1;
          uVar2 = FUN_0802d624(local_38 * 3);
          uVar7 = *(undefined4 *)(param_1 + 8);
          FUN_0802741e(auStack_48,&local_34);
          FUN_08027526(auStack_48,1);
          local_4c = local_34 - *param_3;
          iVar8 = local_30 - param_3[1];
          for (iVar5 = local_30; iVar5 <= local_28; iVar5 = iVar5 + 1) {
            iVar6 = FUN_08017484(apsStack_5c,uVar7,auStack_48);
            if (iVar6 != 0) {
              iVar6 = FUN_0802c184(iVar3,local_4c,iVar8,local_38,uVar2);
              if (iVar6 != 1) {
                FUN_0802c016(iVar3);
                FUN_0802d6c0(uVar2);
                thunk_FUN_0802c016(iVar3);
                *(undefined4 *)(param_1 + 8) = uVar7;
                return 0;
              }
              *(short ***)(param_1 + 8) = apsStack_5c;
              FUN_080287bc(param_1,param_2,auStack_48,uVar2,iVar4);
              local_44[0] = local_44[0] + 1;
              local_3c = local_3c + 1;
              iVar8 = iVar8 + 1;
            }
          }
          *(undefined4 *)(param_1 + 8) = uVar7;
          FUN_0802d6c0(uVar2);
        }
        else {
          FUN_0802741e(&local_34,param_3);
          if ((*param_2 != 0) || (param_2[1] != 0x100)) {
            apsStack_5c[0] = param_2 + 2;
            FUN_08017880(&local_34,(param_3[2] - *param_3) + 1,(param_3[3] - param_3[1]) + 1,
                         (int)*param_2,param_2[1]);
            local_34 = local_34 + *param_3;
            local_30 = local_30 + param_3[1];
            local_2c = local_2c + *param_3;
            local_28 = local_28 + param_3[1];
          }
          iVar5 = FUN_08017484(local_44,*(undefined4 *)(param_1 + 8),&local_34);
          if (iVar5 == 0) {
            thunk_FUN_0802c016(iVar3);
            return 1;
          }
          uVar2 = *(undefined4 *)(param_1 + 8);
          *(int **)(param_1 + 8) = local_44;
          FUN_080287bc(param_1,param_2,param_3,*(undefined4 *)(iVar3 + 0x18),iVar4);
          *(undefined4 *)(param_1 + 8) = uVar2;
        }
      }
      else {
        FUN_08037ff4(param_1,param_3);
      }
      thunk_FUN_0802c016(iVar3);
      uVar2 = 1;
    }
  }
  return uVar2;
}

