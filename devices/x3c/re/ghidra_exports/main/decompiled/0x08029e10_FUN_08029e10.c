// 0x08029e10 FUN_08029e10 size=438

void FUN_08029e10(int *param_1,int *param_2)

{
  char cVar1;
  undefined1 uVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int local_38;
  int local_34;
  int local_28;
  
  iVar8 = param_2[3];
  if ((iVar8 == 0) || ((char)param_2[4] != '\0')) {
    if ((char)param_2[4] == '\x01') {
      iVar8 = 0;
    }
    local_28 = FUN_080274da(param_1[1]);
    iVar3 = FUN_08017484(&local_38,*param_2,param_1[2]);
    if (iVar3 != 0) {
      piVar4 = (int *)FUN_08018528();
      iVar3 = *param_1;
      if (*(int *)(*piVar4 + 0x28) == 0) {
        if ((int)((uint)*(byte *)(*piVar4 + 0x1c) << 0x19) < 0) {
          iVar3 = (local_38 - *(int *)param_1[1]) * 3 +
                  local_28 * (local_34 - ((int *)param_1[1])[1]) * 3 + iVar3;
        }
        else {
          iVar3 = iVar3 + (local_28 * (local_34 - ((int *)param_1[1])[1]) +
                          (local_38 - *(int *)param_1[1])) * 2;
        }
      }
      iVar9 = param_2[1];
      if (iVar9 == 0) {
        iVar5 = 0;
        iVar9 = 0;
      }
      else {
        iVar5 = FUN_080274da(*param_2);
        iVar9 = iVar9 + (iVar5 * (local_34 - ((int *)*param_2)[1]) + (local_38 - *(int *)*param_2))
                        * 2;
      }
      if (iVar8 == 0) {
        iVar6 = 0;
        iVar8 = 0;
      }
      else {
        if (-1 < (int)((uint)*(byte *)(*piVar4 + 0x1c) << 0x1c)) {
          iVar6 = FUN_080274b0(param_2[5]);
          for (iVar7 = 0; iVar7 < iVar6; iVar7 = iVar7 + 1) {
            if (*(byte *)(iVar8 + iVar7) < 0x81) {
              uVar2 = 0;
            }
            else {
              uVar2 = 0xff;
            }
            *(undefined1 *)(iVar8 + iVar7) = uVar2;
          }
        }
        iVar6 = FUN_080274da(param_2[5]);
        iVar8 = iVar6 * (local_34 - ((int *)param_2[5])[1]) +
                (local_38 - *(int *)param_2[5]) + iVar8;
      }
      FUN_080274fe(&local_38,-*(int *)param_1[1],-((int *)param_1[1])[1]);
      if (*(int *)(*piVar4 + 0x28) == 0) {
        cVar1 = *(char *)((int)param_2 + 0x19);
        if (cVar1 == '\0') {
          if (param_2[1] == 0) {
            FUN_08020b88(iVar3,&local_38,local_28,param_2[2],(char)param_2[6],iVar8,iVar6);
          }
          else {
            FUN_08034c2c(iVar3,&local_38,local_28,iVar9,iVar5,(char)param_2[6],iVar8,iVar6);
          }
        }
        else if (param_2[1] == 0) {
          FUN_08020a58(iVar3,&local_38,local_28,param_2[2],(char)param_2[6],iVar8,iVar6,cVar1);
        }
        else {
          FUN_08034ab8(iVar3,&local_38,local_28,iVar9,iVar5,(char)param_2[6],iVar8,iVar6,cVar1);
        }
      }
      else if (param_2[1] == 0) {
        FUN_08020ec8(iVar3,&local_38,local_28,param_2[2],(char)param_2[6],iVar8,iVar6);
      }
      else {
        FUN_08034f36(iVar3,&local_38,local_28,iVar9,iVar5,(char)param_2[6],iVar8,iVar6);
      }
    }
  }
  return;
}

