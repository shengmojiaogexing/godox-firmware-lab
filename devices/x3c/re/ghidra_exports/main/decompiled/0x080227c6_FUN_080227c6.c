// 0x080227c6 FUN_080227c6 size=164

int FUN_080227c6(int param_1,uint param_2)

{
  ushort uVar1;
  uint uVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  undefined4 *puVar6;
  int iVar7;
  undefined4 *puVar8;
  
  uVar2 = 0;
  uVar1 = *(ushort *)(param_1 + 0x2a);
  uVar4 = (uVar1 & 0x1ff) >> 3;
  iVar7 = *(int *)(param_1 + 0xc);
  while ((uVar2 < uVar4 &&
         ((uVar5 = *(uint *)(iVar7 + uVar2 * 8 + 4), -1 < (int)(uVar5 << 6) ||
          ((uVar5 & 0xffffff) != param_2))))) {
    uVar2 = uVar2 + 1;
  }
  if (uVar4 == uVar2) {
    uVar2 = ((uint)(uVar1 >> 9) | (uint)uVar1 << 0x17) + 0x4000000;
    *(ushort *)(param_1 + 0x2a) = (ushort)(uVar2 >> 0x17) | (uVar1 >> 9) << 9;
    uVar3 = FUN_0802d72c(iVar7,uVar2 >> 0x17 & 0xfffffff8);
    *(undefined4 *)(param_1 + 0xc) = uVar3;
    uVar2 = (*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3;
    while (uVar4 = uVar2 - 1, uVar4 != 0) {
      puVar8 = (undefined4 *)(*(int *)(param_1 + 0xc) + uVar4 * 8);
      puVar6 = (undefined4 *)(*(int *)(param_1 + 0xc) + (uVar2 - 2) * 8);
      uVar3 = puVar6[1];
      *puVar8 = *puVar6;
      puVar8[1] = uVar3;
      uVar2 = uVar4;
    }
    FUN_0802d8d6(*(undefined4 *)(param_1 + 0xc),8);
    uVar3 = FUN_0802d5c4(8);
    **(undefined4 **)(param_1 + 0xc) = uVar3;
    FUN_08032c54();
    uVar2 = *(uint *)(*(int *)(param_1 + 0xc) + 4);
    *(uint *)(*(int *)(param_1 + 0xc) + 4) = uVar2 | 0x2000000;
    *(uint *)(*(int *)(param_1 + 0xc) + 4) = uVar2 & 0xff000000 | 0x2000000 | param_2 & 0xffffff;
    iVar7 = *(int *)(param_1 + 0xc);
  }
  else {
    iVar7 = iVar7 + uVar2 * 8;
  }
  return iVar7;
}

