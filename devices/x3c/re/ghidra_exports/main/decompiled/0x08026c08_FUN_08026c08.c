// 0x08026c08 FUN_08026c08 size=430

void FUN_08026c08(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  undefined1 auStack_ec [76];
  undefined1 auStack_a0 [16];
  undefined1 auStack_90 [4];
  uint local_8c;
  undefined1 auStack_7c [4];
  undefined4 local_78;
  undefined4 local_74;
  undefined1 *local_70;
  undefined1 *local_6c;
  undefined1 *local_5c;
  undefined1 *local_58;
  undefined4 local_48;
  uint local_40;
  uint local_34;
  undefined1 auStack_30 [8];
  undefined4 local_28;
  
  iVar2 = FUN_0802ad70(param_1);
  local_28 = FUN_0802ad00(param_1);
  FUN_08021f30(iVar2,auStack_30,&local_34);
  FUN_0802e788(auStack_7c,local_28);
  uVar1 = DAT_08026db8;
  if (0 < (int)local_34) {
    FUN_0802835e(auStack_90);
    FUN_0802fd28(iVar2,0,auStack_90);
    local_48 = 0;
    local_78 = uVar1;
    local_74 = 0;
    local_40 = local_34;
    local_5c = auStack_90;
    local_58 = auStack_30;
    FUN_0802ad74(iVar2,0x1a,auStack_7c);
    FUN_0802833e(local_28,auStack_90,auStack_30,local_40 & 0xffff,
                 *(short *)(iVar2 + 0x2c) + *(short *)(iVar2 + 0x32),
                 *(short *)(iVar2 + 0x34) + *(short *)(iVar2 + 0x2c));
    FUN_0802ad74(iVar2,0x1b,auStack_7c);
  }
  iVar3 = FUN_0802f944(iVar2,0x20000);
  iVar4 = FUN_0802f96e(iVar2,0x20000);
  iVar5 = FUN_0802f9a4(iVar2,0x20000);
  iVar6 = FUN_0802f90e(iVar2,0x20000);
  if (iVar4 < iVar3) {
    iVar7 = iVar3;
  }
  else {
    iVar7 = iVar4;
  }
  if (iVar6 < iVar5) {
    iVar8 = iVar5;
  }
  else {
    iVar8 = iVar6;
  }
  if (iVar8 < iVar7) {
    if (iVar4 < iVar3) {
      iVar6 = iVar3;
    }
    else {
      iVar6 = iVar4;
    }
  }
  else if (iVar6 < iVar5) {
    iVar6 = iVar5;
  }
  uVar9 = local_34 - iVar6;
  if (0 < (int)uVar9) {
    FUN_0802835e(auStack_90);
    FUN_0802fd28(iVar2,0x20000,auStack_90);
    local_48 = 0x20000;
    local_78 = uVar1;
    local_74 = 1;
    local_5c = auStack_90;
    local_58 = auStack_30;
    local_40 = uVar9;
    FUN_0802ad74(iVar2,0x1a,auStack_7c);
    if ((int)local_40 < (int)local_8c) {
      local_8c = local_40;
    }
    FUN_0802833e(local_28,auStack_90,auStack_30,local_40 & 0xffff,
                 *(short *)(iVar2 + 0x2c) + *(short *)(iVar2 + 0x2e),
                 *(short *)(iVar2 + 0x30) + *(short *)(iVar2 + 0x2c));
    FUN_0802ad74(iVar2,0x1b,auStack_7c);
  }
  FUN_0802230c(iVar2,auStack_30,local_34,auStack_a0);
  FUN_08029a38(auStack_ec);
  FUN_0802fff4(iVar2,0x30000,auStack_ec);
  local_48 = 0x30000;
  local_78 = uVar1;
  local_74 = 2;
  local_70 = auStack_a0;
  local_6c = auStack_ec;
  FUN_0802ad74(iVar2,0x1a,auStack_7c);
  FUN_08029a14(local_28,auStack_ec,auStack_a0);
  FUN_0802ad74(iVar2,0x1b,auStack_7c);
  return;
}

