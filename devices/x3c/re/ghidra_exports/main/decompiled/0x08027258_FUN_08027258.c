// 0x08027258 FUN_08027258 size=114

void FUN_08027258(int param_1,uint param_2)

{
  short sVar1;
  short sVar2;
  short sVar3;
  undefined2 uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  undefined4 unaff_r4;
  undefined4 unaff_r5;
  uint uVar8;
  undefined4 unaff_r6;
  
  if (0x168 < param_2) {
    param_2 = param_2 - 0x168 & 0xffff;
  }
  sVar1 = *(short *)(param_1 + 0x32);
  sVar2 = *(short *)(param_1 + 0x34) - sVar1;
  iVar5 = (int)sVar2;
  sVar3 = *(short *)(param_1 + 0x34) - (short)param_2;
  iVar7 = (int)sVar3;
  if (iVar5 < 0) {
    iVar5 = (int)(short)(sVar2 + 0x168);
  }
  if (iVar7 < 0) {
    iVar7 = (int)(short)(sVar3 + 0x168);
  }
  if ((iVar7 - iVar5) + 0xb4U < 0x169) {
    if (iVar7 < iVar5) {
      FUN_080250f4(param_1,sVar1,param_2,0);
    }
    else if (iVar5 < iVar7) {
      FUN_080250f4(param_1,param_2,sVar1,0);
    }
  }
  else {
    FUN_080302d0(param_1);
  }
  *(short *)(param_1 + 0x32) = (short)param_2;
  iVar5 = (int)*(short *)(param_1 + 0x36);
  if (iVar5 != -0x8000) {
    sVar1 = *(short *)(param_1 + 0x34);
    uVar8 = (uint)*(ushort *)(param_1 + 0x32);
    if (*(ushort *)(param_1 + 0x34) < uVar8) {
      sVar1 = *(ushort *)(param_1 + 0x34) + 0x168;
    }
    iVar7 = (int)sVar1;
    uVar6 = (*(byte *)(param_1 + 0x3c) & 7) >> 1;
    if (uVar6 == 0) {
      uVar4 = FUN_0802d580(iVar5,(int)*(short *)(param_1 + 0x38),(int)*(short *)(param_1 + 0x3a),
                           uVar8,iVar7,unaff_r4,unaff_r5,unaff_r6);
      FUN_080271b2(param_1,*(undefined2 *)(param_1 + 0x32),uVar4);
    }
    else if (uVar6 == 1) {
      uVar6 = (int)(uVar8 + iVar7) / 2;
      iVar7 = (int)(short)(((int)*(short *)(param_1 + 0x38) + (int)*(short *)(param_1 + 0x3a)) / 2);
      if (iVar5 < iVar7) {
        uVar4 = FUN_0802d580(iVar5,(int)*(short *)(param_1 + 0x38),iVar7,uVar8,(int)(short)uVar6,
                             unaff_r4,unaff_r5,unaff_r6);
        FUN_08027348(param_1,uVar4);
        FUN_080272ca(param_1,uVar6 & 0xffff);
      }
      else {
        uVar4 = FUN_0802d580();
        FUN_08027348(param_1,uVar6 & 0xffff);
        FUN_080272ca(param_1,uVar4);
      }
    }
    else {
      if (uVar6 != 2) {
        return;
      }
      uVar4 = FUN_0802d580(iVar5,(int)*(short *)(param_1 + 0x38),(int)*(short *)(param_1 + 0x3a),
                           iVar7,uVar8,unaff_r4,unaff_r5,unaff_r6);
      FUN_080271b2(param_1,uVar4,*(undefined2 *)(param_1 + 0x34));
    }
    *(undefined2 *)(param_1 + 0x44) = uVar4;
  }
  return;
}

