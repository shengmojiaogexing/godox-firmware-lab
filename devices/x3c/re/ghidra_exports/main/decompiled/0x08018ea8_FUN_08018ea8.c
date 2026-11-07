// 0x08018ea8 FUN_08018ea8 size=238

undefined4 * FUN_08018ea8(int param_1,int param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  undefined4 *puVar7;
  uint uVar8;
  undefined4 local_28;
  
  piVar2 = DAT_08018f9c;
  iVar1 = DAT_08018f98;
  if ((*(byte *)(param_1 + 9) & 7) == 2) {
    iVar6 = param_2;
  }
  else {
    iVar6 = param_3;
  }
  if (param_3 < param_2) {
    param_3 = param_2;
  }
  uVar8 = (param_3 * 2 + 3U & 0xfffffffc) + 0x14;
  puVar7 = *(undefined4 **)(DAT_08018f98 + 8);
  iVar3 = (int)puVar7 - *DAT_08018f9c;
  local_28 = param_4;
  if (uVar8 + iVar3 < *(uint *)(DAT_08018f98 + 4)) {
    puVar7[1] = puVar7[1] & 0x7fffffff;
  }
  else if (*(uint *)(DAT_08018f98 + 4) < uVar8) {
    puVar7 = (undefined4 *)FUN_0802d5c4(uVar8);
    if (puVar7 == (undefined4 *)0x0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    puVar7[1] = puVar7[1] | 0x80000000;
  }
  else {
    while (*(uint *)(iVar1 + 4) < iVar3 + uVar8) {
      local_28 = 0xffffffff;
      FUN_080254c0(DAT_08018fa0,&local_28,0);
      FUN_080254c0(DAT_08018fa4,&local_28,0);
      iVar3 = *(int *)(iVar1 + 8) - *piVar2;
    }
    puVar7 = *(undefined4 **)(iVar1 + 8);
    puVar7[1] = puVar7[1] & 0x7fffffff;
  }
  uVar4 = FUN_0801b5bc(param_1,iVar6,param_2);
  *puVar7 = uVar4;
  uVar5 = (puVar7[1] & 0xc0000000) + 1 & 0xbfffffff;
  puVar7[1] = uVar5;
  puVar7[3] = param_3;
  puVar7[4] = iVar6;
  if ((int)uVar5 < 0) {
    puVar7[2] = puVar7 + 5;
  }
  else {
    iVar6 = *(int *)(iVar1 + 8);
    puVar7[2] = iVar6 + 0x14;
    *(uint *)(iVar1 + 8) = iVar6 + uVar8;
  }
  return puVar7;
}

