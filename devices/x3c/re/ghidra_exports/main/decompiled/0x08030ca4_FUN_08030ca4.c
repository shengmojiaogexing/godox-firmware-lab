// 0x08030ca4 FUN_08030ca4 size=286

void FUN_08030ca4(int param_1,undefined4 param_2,int param_3)

{
  byte *pbVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined4 extraout_r1;
  int local_30;
  
  if (*DAT_08030dc4 != '\0') {
    FUN_080302d0(param_1);
    iVar2 = FUN_08031782(param_2);
    uVar3 = FUN_08032d50(param_3,4);
    uVar4 = FUN_08032d50(param_3,2);
    uVar5 = FUN_08032d50(param_3,1);
    iVar6 = FUN_08032d50(param_3,0x10);
    local_30 = param_3;
    if ((uVar3 != 0) &&
       ((((iVar2 == 0xf0000 || (iVar2 == 0)) ||
         (local_30 = FUN_0802f9bc(param_1,0,4), iVar7 = DAT_08030dc8, local_30 == DAT_08030dc8)) ||
        (local_30 = FUN_0802f9bc(param_1,0,1), local_30 == iVar7)))) {
      FUN_0802ad74(param_1,0x2a,0);
      FUN_08030436(param_1);
    }
    if ((((iVar2 == 0xf0000) || (iVar2 == 0)) && ((param_3 == 0xffff || (uVar3 != 0)))) &&
       (iVar7 = FUN_0802ee0a(param_1), iVar7 != 0)) {
      FUN_08030436();
    }
    if (((iVar2 == 0xf0000) || (iVar2 == 0)) && (iVar6 != 0)) {
      uVar8 = FUN_0801a148(param_1);
      if (*(int *)(param_1 + 8) == 0) {
        if (uVar8 != 0) {
          FUN_0802dee0(param_1);
          pbVar1 = (byte *)(*(int *)(param_1 + 8) + 0x22);
          *pbVar1 = *pbVar1 & 0x3f | (byte)((uVar8 & 3) << 6);
        }
      }
      else {
        pbVar1 = (byte *)(*(int *)(param_1 + 8) + 0x22);
        *pbVar1 = *pbVar1 & 0x3f | (byte)((uVar8 & 3) << 6);
      }
    }
    if ((param_3 == 0xffff) || (uVar4 != 0)) {
      FUN_08030c20(param_1);
    }
    FUN_080302d0(param_1);
    if (((param_3 == 0xffff) || (((uVar4 | uVar3) & uVar5) != 0)) && (iVar2 != 0x10000)) {
      FUN_08036804(param_1,extraout_r1,local_30,uVar5);
      return;
    }
  }
  return;
}

