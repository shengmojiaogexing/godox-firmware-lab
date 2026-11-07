// 0x0802da9c FUN_0802da9c size=168

void FUN_0802da9c(int param_1,undefined4 param_2,uint param_3)

{
  ushort uVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  
  FUN_08039f20(param_1,param_3,0xffff,0);
  uVar8 = 0;
  uVar1 = *(ushort *)(param_1 + 0x2a);
  while ((uVar8 < (uVar1 & 0x1ff) >> 3 &&
         ((iVar6 = *(int *)(*(int *)(param_1 + 0xc) + uVar8 * 8 + 4), iVar6 << 6 < 0 ||
          (iVar6 << 7 < 0))))) {
    uVar8 = uVar8 + 1;
  }
  uVar2 = ((uint)(uVar1 >> 9) | (uint)uVar1 << 0x17) + 0x4000000;
  *(ushort *)(param_1 + 0x2a) = (ushort)(uVar2 >> 0x17) | (uVar1 >> 9) << 9;
  uVar3 = FUN_0802d72c(*(int *)(param_1 + 0xc),uVar2 >> 0x17 & 0xfffffff8);
  *(undefined4 *)(param_1 + 0xc) = uVar3;
  uVar2 = (*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3;
  while (uVar7 = uVar2 - 1, uVar8 < uVar7) {
    puVar5 = (undefined4 *)(*(int *)(param_1 + 0xc) + uVar7 * 8);
    puVar4 = (undefined4 *)(*(int *)(param_1 + 0xc) + (uVar2 - 2) * 8);
    uVar3 = puVar4[1];
    *puVar5 = *puVar4;
    puVar5[1] = uVar3;
    uVar2 = uVar7;
  }
  FUN_0802d8d6(*(int *)(param_1 + 0xc) + uVar8 * 8,8);
  *(undefined4 *)(*(int *)(param_1 + 0xc) + uVar8 * 8) = param_2;
  iVar6 = uVar8 * 8 + 4;
  *(uint *)(*(int *)(param_1 + 0xc) + iVar6) =
       *(uint *)(*(int *)(param_1 + 0xc) + iVar6) & 0xff000000 | param_3 & 0xffffff;
  FUN_08030ca4(param_1,param_3,0xffff);
  return;
}

