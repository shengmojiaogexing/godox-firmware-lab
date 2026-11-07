// 0x0803c1c0 FUN_0803c1c0 size=316

ulonglong FUN_0803c1c0(uint param_1,uint param_2,undefined4 param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  bool bVar8;
  
  uVar3 = param_1 >> 7 & 0xff0000;
  bVar8 = uVar3 == 0;
  if (!bVar8) {
    param_4 = param_2 >> 7 & 0xff0000;
    bVar8 = param_4 == 0;
  }
  if (!bVar8) {
    bVar8 = uVar3 == 0xff0000;
  }
  if (!bVar8) {
    bVar8 = param_4 == 0xff0000;
  }
  if (bVar8) {
    uVar6 = param_2 >> 7 & 0xff0000;
    if ((int)(param_1 ^ param_2) < 0) {
      uVar3 = uVar3 | 0x100;
    }
    if (uVar3 < 0xff0000 && uVar6 < 0xff0000) {
      if (uVar6 != 0) {
        return CONCAT44(param_2,param_1 ^ param_2) & 0xffffffff80000000;
      }
      if ((uVar3 & 0xff0000) == 0) {
        return CONCAT44(param_2,DAT_0803c340);
      }
      param_1 = param_1 ^ param_2;
    }
    else {
      param_1 = FUN_0803c3aa();
      param_1 = param_1 & 0x80000000;
    }
  }
  else {
    if ((int)(param_1 ^ param_2) < 0) {
      uVar3 = uVar3 | 0x100;
    }
    uVar2 = param_1 & 0xffffff | 0x800000;
    uVar1 = param_2 & 0xffffff | 0x800000;
    bVar8 = uVar1 <= uVar2;
    uVar6 = -uVar1;
    uVar1 = (uint)*(byte *)(0x803c308 - (uVar1 >> 0x11));
    iVar7 = uVar1 * 2;
    if (!bVar8) {
      uVar2 = uVar2 << 1;
    }
    iVar4 = (uVar3 - param_4) + 0x7d0000;
    iVar7 = uVar1 * 0x100 + (((int)(uVar6 * iVar7) >> 4) * iVar7 >> 0x15);
    uVar1 = iVar4 + (iVar4 >> 0x10) + (uint)bVar8;
    uVar5 = iVar7 * (uVar2 >> 8) >> 0x14;
    uVar3 = uVar6 * uVar5 + uVar2 * 0x800;
    uVar2 = iVar7 * (uVar3 >> 8) >> 0x13;
    uVar3 = uVar6 * uVar2 + uVar3 * 0x1000;
    bVar8 = CARRY4(uVar3,uVar6);
    if (bVar8) {
      uVar3 = uVar3 + uVar6;
    }
    iVar7 = uVar6 + uVar3 * 2;
    param_1 = uVar2 + uVar5 * 0x1000 + (uint)bVar8 +
              uVar1 * 0x800000 + (uint)CARRY4(uVar6,uVar3 * 2);
    if (uVar1 < 0xfc0000) {
      return CONCAT44(iVar7,param_1);
    }
    if ((int)(uVar1 - 0xfc0000) < 0) {
      if ((uVar1 & 0xf0) != 0) {
        param_1 = param_1 + 0x60000000 & 0x80000000;
      }
      return CONCAT44(iVar7,param_1);
    }
    if (0xfdffffff < param_1 * 2 + 0x1000000) {
      return CONCAT44(iVar7,param_1);
    }
    param_1 = param_1 + 0xa0000000;
  }
  return CONCAT44(0xff,(param_1 >> 0x17 | 0xff) << 0x17);
}

