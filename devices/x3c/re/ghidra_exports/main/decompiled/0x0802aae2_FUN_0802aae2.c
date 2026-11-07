// 0x0802aae2 FUN_0802aae2 size=464

void FUN_0802aae2(undefined4 param_1,int *param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,short *param_7,int param_8,int param_9,
                 int param_10)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int local_80;
  int local_7c;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  int local_68;
  undefined1 auStack_64 [16];
  int local_54;
  int local_50;
  undefined4 local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  undefined4 uStack_34;
  int *piStack_30;
  undefined4 local_2c;
  undefined4 uStack_28;
  
  local_48 = -(int)*param_7;
  local_4c = (undefined4)(0x10000 / (ulonglong)(ushort)param_7[1]);
  local_3c = *(int *)(param_7 + 2);
  local_38 = *(int *)(param_7 + 4);
  sVar1 = (short)(local_48 / 10);
  iVar8 = local_48 % 10;
  uStack_34 = param_1;
  piStack_30 = param_2;
  local_2c = param_3;
  uStack_28 = param_4;
  local_68 = FUN_080338d4((int)sVar1);
  local_6c = FUN_080338d4((int)(short)(sVar1 + 1));
  iVar2 = FUN_080338d4((int)(short)(sVar1 + 0x5a));
  iVar3 = FUN_080338d4((int)(short)(sVar1 + 0x5b));
  local_54 = (local_6c * iVar8 + (10 - iVar8) * local_68) / 10 >> 5;
  local_50 = (iVar3 * iVar8 + (10 - iVar8) * iVar2) / 10 >> 5;
  local_44 = local_3c << 8;
  local_40 = local_38 << 8;
  iVar2 = param_2[2] - *param_2;
  iVar3 = iVar2 + 1;
  local_70 = (param_2[3] - param_2[1]) + 1;
  iVar8 = 0;
  do {
    if (local_70 <= iVar8) {
      return;
    }
    FUN_0803a0b0(auStack_64,*param_2,param_2[1] + iVar8,&local_74,&local_78);
    FUN_0803a0b0(auStack_64,param_2[2],param_2[1] + iVar8,&local_7c,&local_80);
    iVar4 = 0;
    iVar5 = 0;
    if (1 < iVar3) {
      iVar4 = ((local_7c - local_74) * 0x100) / iVar2;
      iVar5 = ((local_80 - local_78) * 0x100) / iVar2;
    }
    iVar6 = local_74 + 0x80;
    iVar7 = local_78 + 0x80;
    if ((*(byte *)(param_7 + 0xc) & 1) == 0) {
      if (param_8 == 4) {
LAB_0802ac94:
        FUN_08036b66(local_2c,uStack_28,param_5,param_6,iVar6,iVar7,iVar4,iVar5,iVar3,param_9,
                     param_10,param_8);
      }
      else if (param_8 == 5) {
        FUN_080194aa(local_2c,uStack_28,param_5,param_6,iVar6,iVar7,iVar4,iVar5,iVar3,param_9,
                     param_10);
      }
      else {
        if (param_8 == 6) goto LAB_0802ac94;
        if (param_8 == 0x14) {
          FUN_08036af4(local_2c,uStack_28,param_5,param_6,iVar6,iVar7,iVar4,iVar5,iVar3,param_9,
                       param_10);
        }
      }
    }
    else {
      FUN_08019190(local_2c,uStack_28,param_5,param_6,iVar6,iVar7,iVar4,iVar5,iVar3,param_9,param_10
                   ,param_8);
    }
    param_9 = param_9 + iVar3 * 2;
    param_10 = param_10 + iVar3;
    iVar8 = iVar8 + 1;
  } while( true );
}

