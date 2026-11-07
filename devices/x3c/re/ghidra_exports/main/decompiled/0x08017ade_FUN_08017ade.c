// 0x08017ade FUN_08017ade size=340

undefined8 FUN_08017ade(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  short sVar8;
  int iVar9;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  
  local_30 = param_1;
  local_2c = param_2;
  if ((*(int *)(param_1 + 0x24) == 0) && (*(int *)(param_1 + 0x28) == 0)) goto LAB_08017c2e;
  iVar9 = *(int *)(param_1 + 0x4c);
  local_28 = param_3;
  local_24 = param_4;
  if (iVar9 == 0) {
    iVar9 = FUN_08020f96(param_1);
    if (iVar9 == 0) goto LAB_08017c2e;
    FUN_08024f38(param_1);
    FUN_0802ad74(iVar9,9,0);
    if ((int)((uint)*(byte *)(param_1 + 1) << 0x1e) < 0) goto LAB_08017c2e;
  }
  sVar8 = 0;
  iVar7 = 0x100;
  for (iVar3 = iVar9; iVar3 != 0; iVar3 = FUN_0802ee0a(iVar3)) {
    sVar1 = FUN_0802fab8(iVar3,0);
    sVar8 = sVar1 + sVar8;
    iVar2 = FUN_0802fad6(iVar3,0);
    iVar7 = (int)(short)((short)iVar7 * (short)(iVar2 + ((uint)(iVar2 >> 0x1f) >> 0x18) >> 8));
  }
  if ((sVar8 != 0) || (iVar7 != 0x100)) {
    local_30 = iVar3;
    local_2c = iVar3;
    FUN_080318c8(param_1 + 0x24,(int)-sVar8,(int)(short)(0x10000 / iVar7),&local_30);
  }
  local_28 = iVar3;
  local_24 = iVar3;
  if ((*(byte *)(param_1 + 0x6c) & 0xf) == 3) {
    uVar4 = FUN_0802efdc(iVar9);
    uVar5 = FUN_0802ef1c(iVar9);
    local_30 = 3;
    local_24 = FUN_0801fd5a(iVar9,*(undefined4 *)(param_1 + 0x24),uVar5,uVar4);
  }
  else {
    uVar4 = FUN_0802f0bc(iVar9);
    uVar5 = FUN_0802ee28(iVar9);
    local_30 = 0xc;
    local_28 = FUN_0801fd5a(iVar9,*(undefined4 *)(param_1 + 0x28),uVar4,uVar5);
  }
  uVar6 = FUN_0802eec8(iVar9);
  if (((uVar6 & 1) == 0) && (0 < local_24)) {
    local_24 = iVar3;
  }
  if ((-1 < (int)(uVar6 << 0x1e)) && (local_24 < 0)) {
    local_24 = iVar3;
  }
  if ((-1 < (int)(uVar6 << 0x1d)) && (0 < local_28)) {
    local_28 = iVar3;
  }
  if ((-1 < (int)(uVar6 << 0x1c)) && (local_28 < 0)) {
    local_28 = iVar3;
  }
  FUN_08037110(param_1,&local_24,&local_28);
  FUN_080180e2(iVar9,local_24,local_28);
  if (-1 < (int)((uint)*(byte *)(param_1 + 1) << 0x1e)) {
    *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x2c) + local_24;
    *(int *)(param_1 + 0x30) = *(int *)(param_1 + 0x30) + local_28;
  }
LAB_08017c2e:
  return CONCAT44(local_2c,local_30);
}

