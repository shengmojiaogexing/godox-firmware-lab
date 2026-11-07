// 0x080360a0 FUN_080360a0 size=326

void FUN_080360a0(int *param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined1 auStack_44 [16];
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  
  iVar3 = DAT_080361e8;
  piVar1 = *(int **)(DAT_080361e8 + 4);
  iVar4 = *piVar1;
  puVar10 = *(undefined4 **)(iVar4 + 0x48);
  *puVar10 = *(undefined4 *)(*(int *)(iVar4 + 0x18) + 8);
  uVar5 = *(uint *)(*piVar1 + 0x1c);
  if (((int)(uVar5 << 0x1e) < 0) || ((uVar5 & 1) != 0)) {
    iVar4 = FUN_080282e6();
    iVar2 = FUN_08028248(*(undefined4 *)(iVar3 + 4));
    FUN_08027518(auStack_44,0,0,iVar2 + -1,iVar4 + -1);
    puVar10[1] = auStack_44;
    iVar3 = **(int **)(iVar3 + 4);
    if ((int)((uint)*(byte *)(iVar3 + 0x1c) << 0x1e) < 0) {
      *(uint *)(*(int *)(iVar3 + 0x18) + 0x18) = *(uint *)(*(int *)(iVar3 + 0x18) + 0x18) | 2;
      puVar10[2] = auStack_44;
      FUN_080361ec(puVar10);
    }
    else {
      iVar3 = *(int *)(iVar3 + 0x18);
      *(uint *)(iVar3 + 0x18) =
           *(uint *)(iVar3 + 0x18) & 0xfffffffd | (*(uint *)(iVar3 + 0x18) & 1) << 1;
      puVar10[2] = param_1;
      FUN_080361ec(puVar10);
    }
  }
  else {
    iVar8 = *param_1;
    iVar6 = param_1[2];
    iVar9 = param_1[1];
    iVar7 = param_1[3];
    iVar4 = FUN_080282e6();
    iVar2 = param_1[3];
    if (iVar4 <= iVar2) {
      iVar2 = FUN_080282e6(*(undefined4 *)(iVar3 + 4));
      iVar2 = iVar2 + -1;
    }
    iVar6 = FUN_08022480(*(undefined4 *)(iVar3 + 4),(iVar6 - iVar8) + 1,(iVar7 - iVar9) + 1);
    iVar4 = 0;
    local_30 = param_1[1];
    while( true ) {
      iVar7 = local_30 + iVar6;
      local_28 = iVar7 + -1;
      if (iVar2 < local_28) break;
      local_34 = *param_1;
      local_2c = param_1[2];
      puVar10[1] = &local_34;
      puVar10[2] = &local_34;
      piVar1 = *(int **)(iVar3 + 4);
      *puVar10 = *(undefined4 *)(*(int *)(*piVar1 + 0x18) + 8);
      if (iVar2 < local_28) {
        local_28 = iVar2;
      }
      iVar4 = local_28;
      if (iVar2 == local_28) {
        iVar8 = *(int *)(*piVar1 + 0x18);
        *(uint *)(iVar8 + 0x18) = *(uint *)(iVar8 + 0x18) | 2;
      }
      FUN_080361ec(puVar10);
      local_30 = iVar7;
    }
    if (iVar2 != iVar4) {
      local_34 = *param_1;
      local_2c = param_1[2];
      puVar10[1] = &local_34;
      puVar10[2] = &local_34;
      piVar1 = *(int **)(iVar3 + 4);
      *puVar10 = *(undefined4 *)(*(int *)(*piVar1 + 0x18) + 8);
      iVar3 = *(int *)(*piVar1 + 0x18);
      *(uint *)(iVar3 + 0x18) = *(uint *)(iVar3 + 0x18) | 2;
      local_28 = iVar2;
      FUN_080361ec(puVar10);
    }
  }
  return;
}

