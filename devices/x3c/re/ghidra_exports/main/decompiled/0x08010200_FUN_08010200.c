// 0x08010200 FUN_08010200 size=252

void FUN_08010200(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  byte bVar1;
  int iVar2;
  byte *pbVar3;
  byte *pbVar4;
  char cVar5;
  short sVar6;
  undefined2 uVar7;
  undefined4 uVar8;
  int iVar9;
  uint uVar10;
  
  pbVar4 = DAT_08010308;
  pbVar3 = DAT_08010304;
  iVar2 = DAT_08010300;
  iVar9 = DAT_080102fc;
  cVar5 = *(char *)(param_1 + 8);
  if (cVar5 == '\x1c') {
    if (*DAT_0801030c == '\0') {
      if (*(short *)(DAT_08010300 + 4) == 0) {
        if ((*(char *)(DAT_08010300 + 2) == '\0') ||
           (iVar9 = FUN_0802fcb6(*(undefined4 *)(param_1 + 4),0x20,param_3,param_4,param_4),
           iVar9 != 0)) {
          uVar7 = FUN_08027774(*(undefined4 *)(param_1 + 4));
          *(undefined2 *)(iVar2 + 4) = uVar7;
        }
        else {
          *(undefined1 *)(iVar2 + 2) = 0;
          cVar5 = FUN_08027774(*(undefined4 *)(param_1 + 4));
          *pbVar4 = *pbVar3 + cVar5 * -10;
        }
      }
      else {
        uVar8 = FUN_08027ce0(0xfd,0x9f,10);
        FUN_0803157c(*(undefined4 *)(iVar9 + 4),uVar8,0);
        sVar6 = FUN_08027774(*(undefined4 *)(param_1 + 4));
        iVar9 = (int)(short)(sVar6 - *(short *)(iVar2 + 4));
        *(short *)(iVar2 + 4) = sVar6;
        if (iVar9 < 0) {
          bVar1 = *pbVar3;
          if (((uint)*pbVar4 != (uint)bVar1) &&
             (uVar10 = (uint)*pbVar4 + iVar9 * -10, *pbVar4 = (byte)uVar10,
             (uint)bVar1 < (uVar10 & 0xff))) {
            *pbVar4 = bVar1;
          }
        }
        else if ((*pbVar4 != 0x14) &&
                (uVar10 = (uint)*pbVar4 + iVar9 * -10, *pbVar4 = (byte)uVar10,
                (uVar10 & 0xff) < 0x14)) {
          *pbVar4 = 0x14;
        }
      }
    }
  }
  else {
    if (cVar5 == '\b') {
      FUN_0802782e(*(undefined4 *)(param_1 + 4),
                   (int)((uint)*DAT_08010304 - (uint)*DAT_08010308) / 10 & 0xff,1);
      *(undefined2 *)(iVar2 + 4) = 0;
      uVar8 = FUN_08027f90();
      FUN_0803157c(*(undefined4 *)(iVar9 + 4),uVar8,0,uVar8);
      return;
    }
    if (cVar5 == '\x01') {
      *(undefined1 *)(DAT_08010300 + 2) = 1;
    }
  }
  return;
}

