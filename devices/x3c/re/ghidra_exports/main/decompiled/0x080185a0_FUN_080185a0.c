// 0x080185a0 FUN_080185a0 size=102

int FUN_080185a0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  undefined4 local_20;
  
  iVar4 = 0;
  local_20 = param_4;
  uVar1 = (*(code *)*DAT_08018608)(param_1);
  local_20 = 0;
  for (uVar5 = 0; uVar5 < uVar1; uVar5 = uVar5 + 1) {
    uVar2 = (*(code *)*DAT_0801860c)(param_1,&local_20);
    iVar3 = FUN_08026b2c(uVar2 & 0xffff);
    if (iVar3 != -1) {
      uVar2 = (uint)*(ushort *)(DAT_08018610 + iVar3 * 10 + 2);
    }
    if (uVar2 < 0x80) {
      iVar4 = iVar4 + 1;
    }
    else if (uVar2 < 0x800) {
      iVar4 = iVar4 + 2;
    }
    else if (uVar2 < 0x10000) {
      iVar4 = iVar4 + 3;
    }
    else {
      iVar4 = iVar4 + 4;
    }
  }
  return iVar4 + 1;
}

