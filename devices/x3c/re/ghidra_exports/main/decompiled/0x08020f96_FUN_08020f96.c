// 0x08020f96 FUN_08020f96 size=602

int FUN_08020f96(int param_1)

{
  byte bVar1;
  short sVar2;
  int *piVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  short sVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  int local_40;
  int local_3c;
  undefined4 local_38;
  undefined4 local_34;
  int local_30;
  undefined4 local_2c;
  int local_28;
  
  local_28 = 0;
  local_2c = 0;
  piVar3 = (int *)FUN_0802c5fc();
  uVar8 = (uint)*(byte *)(*piVar3 + 0x18);
  local_30 = *(int *)(param_1 + 0x44);
  uVar9 = 0;
  uVar14 = 0;
  *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x2c) + *(int *)(param_1 + 0x24);
  *(int *)(param_1 + 0x30) = *(int *)(param_1 + 0x30) + *(int *)(param_1 + 0x28);
LAB_080211c4:
  do {
    if (local_30 == 0) {
LAB_080211cc:
      if (local_28 != 0) {
        *(byte *)(param_1 + 0x6c) = *(byte *)(param_1 + 0x6c) & 0xf0 | (byte)local_2c & 0xf;
        *(int *)(param_1 + 0x4c) = local_28;
        *(undefined4 *)(param_1 + 0x2c) = 0;
        *(undefined4 *)(param_1 + 0x30) = 0;
      }
      return local_28;
    }
    sVar12 = 0;
    iVar10 = 0x100;
    local_38 = 0;
    local_34 = 0;
    for (iVar4 = local_30; iVar4 != 0; iVar4 = FUN_0802ee0a(iVar4)) {
      sVar2 = FUN_0802fab8(iVar4,0);
      sVar12 = sVar2 + sVar12;
      iVar6 = FUN_0802fad6(iVar4,0);
      iVar10 = (int)(short)((short)iVar10 * (short)(iVar6 + ((uint)(iVar6 >> 0x1f) >> 0x18) >> 8));
    }
    local_40 = *(int *)(param_1 + 0x2c);
    local_3c = *(int *)(param_1 + 0x30);
    if ((sVar12 != 0) || (iVar10 != 0x100)) {
      FUN_080318c8(&local_40,(int)-sVar12,(int)(short)(0x10000 / iVar10),&local_38);
    }
    iVar4 = local_40;
    if (local_40 < 1) {
      iVar4 = -local_40;
    }
    if (local_3c < 1) {
      iVar10 = -local_3c;
    }
    else {
      iVar10 = local_3c;
    }
    if (iVar10 < iVar4) {
      uVar9 = 1;
    }
    else {
      uVar14 = 1;
    }
    iVar4 = FUN_0802fc9e(local_30,0x10);
    if (iVar4 == 0) {
      uVar15 = FUN_0802fc9e(local_30,0x100);
      if (((uVar9 & ~uVar15) != 0) ||
         (uVar15 = FUN_0802fc9e(local_30,0x200), (uVar14 & ~uVar15) != 0)) goto LAB_080211cc;
      local_30 = FUN_0802ee0a(local_30);
      goto LAB_080211c4;
    }
    uVar5 = FUN_0802eec8(local_30);
    uVar15 = uVar9;
    if ((uVar5 & 1) == 0) {
      uVar15 = 0;
    }
    uVar16 = uVar9;
    if (-1 < (int)(uVar5 << 0x1e)) {
      uVar16 = 0;
    }
    uVar11 = uVar14;
    if (-1 < (int)(uVar5 << 0x1d)) {
      uVar11 = 0;
    }
    uVar13 = uVar14;
    if (-1 < (int)(uVar5 << 0x1c)) {
      uVar13 = 0;
    }
    iVar4 = FUN_0802f0bc(local_30);
    iVar10 = FUN_0802ee28(local_30);
    iVar6 = FUN_0802ef1c(local_30);
    iVar7 = FUN_0802efdc(local_30);
    if (((0 < iVar4) || (0 < iVar10)) &&
       (((uVar11 != 0 && ((int)uVar8 <= local_3c)) || ((uVar13 != 0 && (local_3c <= (int)-uVar8)))))
       ) {
      local_28 = local_30;
      local_2c = 0xc;
    }
    if (((0 < iVar6) || (0 < iVar7)) &&
       (((uVar15 != 0 && ((int)uVar8 <= local_40)) || ((uVar16 != 0 && (local_40 <= (int)-uVar8)))))
       ) {
      local_28 = local_30;
      local_2c = 3;
    }
    if (iVar4 < 1) {
      uVar11 = 0;
    }
    if (iVar10 < 1) {
      uVar13 = 0;
    }
    if (iVar6 < 1) {
      uVar15 = 0;
    }
    if (iVar7 < 1) {
      uVar16 = 0;
    }
    if ((((uVar15 != 0) && ((int)uVar8 <= local_40)) || ((uVar16 != 0 && (local_40 <= (int)-uVar8)))
        ) || (((uVar11 != 0 && ((int)uVar8 <= local_3c)) ||
              ((uVar13 != 0 && (local_3c <= (int)-uVar8)))))) {
      if (uVar9 == 0) {
        bVar1 = 0xc;
      }
      else {
        bVar1 = 3;
      }
      *(byte *)(param_1 + 0x6c) = *(byte *)(param_1 + 0x6c) & 0xf0 | bVar1;
      goto LAB_080211cc;
    }
    uVar15 = FUN_0802fc9e(local_30,0x100);
    if (((uVar9 & ~uVar15) != 0) || (uVar15 = FUN_0802fc9e(local_30,0x200), (uVar14 & ~uVar15) != 0)
       ) goto LAB_080211cc;
    local_30 = FUN_0802ee0a(local_30);
  } while( true );
}

