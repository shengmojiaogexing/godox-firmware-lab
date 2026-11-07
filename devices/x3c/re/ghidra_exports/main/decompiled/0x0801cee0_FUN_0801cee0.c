// 0x0801cee0 FUN_0801cee0 size=458

void FUN_0801cee0(int *param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int *piVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  int *piVar11;
  int local_28;
  
  piVar6 = (int *)**(undefined4 **)(DAT_0801d0ac + 4);
  uVar1 = piVar6[7];
  if ((-1 < (int)(uVar1 << 0x1e)) || (-1 < (int)(uVar1 << 0x1d))) {
    uVar1 = (uVar1 & 0x3f) >> 4;
    if (uVar1 == 2) {
      FUN_0801d0c4(piVar6,param_1,param_2);
      FUN_0801a19c(piVar6,param_1,param_2);
      return;
    }
    if ((uVar1 == 1) || (uVar1 == 3)) {
      iVar10 = 0;
      piVar11 = param_1;
      iVar2 = FUN_08028242(*(undefined4 **)(DAT_0801d0ac + 4));
      iVar3 = *param_1;
      uVar1 = (param_1[2] - iVar3) + 1;
      iVar8 = param_1[1];
      uVar9 = (param_1[3] - iVar8) + 1;
      if ((int)(0x1400 / (ulonglong)uVar1) < (int)uVar9) {
        uVar5 = (uint)(0x1400 / (ulonglong)uVar1);
      }
      else {
        uVar5 = uVar9;
      }
      if ((*(byte *)(piVar6 + 7) & 0x3f) >> 4 == 1) {
        iVar3 = (piVar6[1] - iVar3) + -1;
        param_1[3] = iVar3;
        param_1[1] = (iVar3 - uVar1) + 1;
      }
      else {
        param_1[1] = iVar3;
        param_1[3] = iVar3 + (param_1[2] - iVar3);
      }
      iVar3 = 0;
      local_28 = param_2;
      while (iVar3 < (int)uVar9) {
        uVar7 = uVar9 - iVar3;
        if ((int)uVar5 < (int)(uVar9 - iVar3)) {
          uVar7 = uVar5;
        }
        *(undefined4 *)(iVar2 + 0x10) = 1;
        if ((iVar3 == 0) && ((int)uVar1 <= (int)uVar9)) {
          FUN_0801d174((*(byte *)(piVar6 + 7) & 0x3f) >> 4 == 3,uVar1,local_28);
          uVar7 = uVar1;
          if ((*(byte *)(piVar6 + 7) & 0x3f) >> 4 == 1) {
            *param_1 = iVar8;
            param_1[2] = iVar8 + uVar1 + -1;
          }
          else {
            iVar4 = (*piVar6 - iVar8) + -1;
            param_1[2] = iVar4;
            *param_1 = (iVar4 - uVar1) + 1;
          }
        }
        else {
          if (iVar10 == 0) {
            iVar10 = FUN_0802d624(0x2800);
          }
          FUN_0801d126((*(byte *)(piVar6 + 7) & 0x3f) >> 4 == 3,uVar1,uVar7,local_28,iVar10,uVar5,
                       iVar10,piVar11);
          if ((*(byte *)(piVar6 + 7) & 0x3f) >> 4 == 1) {
            *param_1 = iVar8 + iVar3;
            param_1[2] = iVar8 + iVar3 + uVar7 + -1;
          }
          else {
            iVar4 = ((*piVar6 - iVar8) - iVar3) + -1;
            param_1[2] = iVar4;
            *param_1 = (iVar4 - uVar7) + 1;
          }
        }
        iVar3 = iVar3 + uVar7;
        if (((iVar3 < (int)uVar9) || ((*(uint *)(iVar2 + 0x18) & 1) == 0)) ||
           (-1 < *(int *)(iVar2 + 0x18) << 0x1e)) {
          *(undefined4 *)(iVar2 + 0x14) = 0;
        }
        else {
          *(undefined4 *)(iVar2 + 0x14) = 1;
        }
        if (iVar10 == 0) {
          iVar4 = local_28;
        }
        else {
          iVar4 = iVar10;
        }
        FUN_0801a19c(piVar6,param_1,iVar4);
        while (*(int *)(iVar2 + 0x10) != 0) {
          if ((code *)piVar6[0xd] != (code *)0x0) {
            (*(code *)piVar6[0xd])(piVar6);
          }
        }
        local_28 = local_28 + uVar1 * uVar7 * 2;
      }
      if (iVar10 != 0) {
        FUN_0802d6c0(iVar10);
        return;
      }
    }
  }
  return;
}

