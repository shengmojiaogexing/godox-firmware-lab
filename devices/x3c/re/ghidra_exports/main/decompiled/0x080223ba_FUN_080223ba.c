// 0x080223ba FUN_080223ba size=196

undefined4 FUN_080223ba(int param_1,uint param_2)

{
  ushort uVar1;
  uint uVar2;
  undefined4 *puVar3;
  uint uVar4;
  undefined4 *puVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  
  uVar1 = *(ushort *)(param_1 + 0x2a);
  iVar6 = *(int *)(param_1 + 0xc);
  for (uVar2 = 0; uVar2 < (uVar1 & 0x1ff) >> 3; uVar2 = uVar2 + 1) {
    uVar4 = *(uint *)(iVar6 + uVar2 * 8 + 4);
    if (((int)(uVar4 << 7) < 0) && ((uVar4 & 0xffffff) == param_2)) {
      return *(undefined4 *)(iVar6 + uVar2 * 8);
    }
  }
  uVar2 = ((uint)(uVar1 >> 9) | (uint)uVar1 << 0x17) + 0x4000000;
  *(ushort *)(param_1 + 0x2a) = (ushort)(uVar2 >> 0x17) | (uVar1 >> 9) << 9;
  iVar6 = FUN_0802d72c(iVar6,uVar2 >> 0x17 & 0xfffffff8);
  *(int *)(param_1 + 0xc) = iVar6;
  if (iVar6 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  uVar2 = (*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3;
  while (uVar4 = uVar2 - 1, uVar4 != 0) {
    iVar6 = *(int *)(param_1 + 0xc);
    iVar7 = *(int *)(iVar6 + (uVar2 - 2) * 8 + 4);
    if ((iVar7 << 7 < 0) || (iVar7 << 6 < 0)) break;
    puVar5 = (undefined4 *)(iVar6 + uVar4 * 8);
    puVar3 = (undefined4 *)(iVar6 + (uVar2 - 2) * 8);
    uVar8 = puVar3[1];
    *puVar5 = *puVar3;
    puVar5[1] = uVar8;
    uVar2 = uVar4;
  }
  FUN_0802d8d6(*(int *)(param_1 + 0xc) + uVar4 * 8,8);
  uVar8 = FUN_0802d5c4(8);
  *(undefined4 *)(*(int *)(param_1 + 0xc) + uVar4 * 8) = uVar8;
  FUN_08032c54();
  iVar6 = uVar4 * 8 + 4;
  uVar2 = *(uint *)(*(int *)(param_1 + 0xc) + iVar6);
  *(uint *)(*(int *)(param_1 + 0xc) + iVar6) = uVar2 | 0x1000000;
  *(uint *)(*(int *)(param_1 + 0xc) + iVar6) = uVar2 & 0xff000000 | 0x1000000 | param_2 & 0xffffff;
  return *(undefined4 *)(*(int *)(param_1 + 0xc) + uVar4 * 8);
}

