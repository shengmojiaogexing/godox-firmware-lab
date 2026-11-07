// 0x080273c0 FUN_080273c0 size=246

void FUN_080273c0(int param_1,int param_2)

{
  short sVar1;
  undefined2 uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  
  if (*(short *)(param_1 + 0x36) != param_2) {
    if (*(short *)(param_1 + 0x3a) < param_2) {
      param_2 = (int)*(short *)(param_1 + 0x3a);
    }
    if (param_2 < *(short *)(param_1 + 0x38)) {
      param_2 = (int)*(short *)(param_1 + 0x38);
    }
    if (*(short *)(param_1 + 0x36) != param_2) {
      *(short *)(param_1 + 0x36) = (short)param_2;
      iVar3 = (int)*(short *)(param_1 + 0x36);
      if (iVar3 != -0x8000) {
        sVar1 = *(short *)(param_1 + 0x34);
        uVar6 = (uint)*(ushort *)(param_1 + 0x32);
        if (*(ushort *)(param_1 + 0x34) < uVar6) {
          sVar1 = *(ushort *)(param_1 + 0x34) + 0x168;
        }
        iVar5 = (int)sVar1;
        uVar4 = (*(byte *)(param_1 + 0x3c) & 7) >> 1;
        if (uVar4 == 0) {
          uVar2 = FUN_0802d580(iVar3,(int)*(short *)(param_1 + 0x38),(int)*(short *)(param_1 + 0x3a)
                               ,uVar6,iVar5);
          FUN_080271b2(param_1,*(undefined2 *)(param_1 + 0x32),uVar2);
        }
        else if (uVar4 == 1) {
          uVar4 = (int)(uVar6 + iVar5) / 2;
          iVar5 = (int)(short)(((int)*(short *)(param_1 + 0x38) + (int)*(short *)(param_1 + 0x3a)) /
                              2);
          if (iVar3 < iVar5) {
            uVar2 = FUN_0802d580(iVar3,(int)*(short *)(param_1 + 0x38),iVar5,uVar6,(int)(short)uVar4
                                );
            FUN_08027348(param_1,uVar2);
            FUN_080272ca(param_1,uVar4 & 0xffff);
          }
          else {
            uVar2 = FUN_0802d580();
            FUN_08027348(param_1,uVar4 & 0xffff);
            FUN_080272ca(param_1,uVar2);
          }
        }
        else {
          if (uVar4 != 2) {
            return;
          }
          uVar2 = FUN_0802d580(iVar3,(int)*(short *)(param_1 + 0x38),(int)*(short *)(param_1 + 0x3a)
                               ,iVar5,uVar6);
          FUN_080271b2(param_1,uVar2,*(undefined2 *)(param_1 + 0x34));
        }
        *(undefined2 *)(param_1 + 0x44) = uVar2;
      }
      return;
    }
  }
  return;
}

