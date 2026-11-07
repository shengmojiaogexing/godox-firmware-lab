// 0x0802bacc FUN_0802bacc size=380

undefined4 FUN_0802bacc(int param_1,uint param_2,int param_3,int param_4,int param_5)

{
  byte bVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  uint uVar6;
  byte *pbVar7;
  uint uVar8;
  uint uVar9;
  undefined4 *puVar10;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  undefined4 local_38;
  int iStack_34;
  uint uStack_30;
  int local_2c;
  int local_28;
  
  local_38 = DAT_0802bc48;
  local_3c = DAT_0802bc4c;
  local_4c = DAT_0802bc50;
  uStack_48 = DAT_0802bc54;
  local_44 = DAT_0802bc58;
  uStack_40 = DAT_0802bc5c;
  uVar2 = *(undefined4 *)(param_1 + 8);
  for (iVar3 = 0; iVar3 < param_4; iVar3 = iVar3 + 1) {
    *(char *)(param_5 + iVar3 * 3) = (char)uVar2;
    *(char *)(iVar3 * 3 + param_5 + 1) = (char)((uint)uVar2 >> 8);
  }
  puVar10 = (undefined4 *)0x0;
  iStack_34 = param_1;
  uStack_30 = param_2;
  local_2c = param_3;
  local_28 = param_4;
  bVar1 = FUN_0802b904(*(byte *)(param_1 + 0x14) & 0x1f);
  uVar9 = 0;
  iVar3 = 0;
  uVar8 = 0;
  uVar4 = *(uint *)(param_1 + 0x14);
  uVar6 = uVar4 & 0x1f;
  if (uVar6 == 0xb) {
    uVar9 = ((uVar4 & 0x1fffff) >> 10) + 7 >> 3;
    iVar3 = uVar9 * local_2c + ((int)param_2 >> 3);
    uVar8 = 7 - (param_2 & 7);
    puVar10 = &local_38;
  }
  else if (uVar6 == 0xc) {
    uVar9 = ((uVar4 & 0x1fffff) >> 10) + 3 >> 2;
    iVar3 = uVar9 * local_2c + ((int)param_2 >> 2);
    uVar8 = (param_2 & 3) * -2 + 6;
    puVar10 = &local_3c;
  }
  else if (uVar6 == 0xd) {
    uVar9 = ((uVar4 & 0x1fffff) >> 10) + 1 >> 1;
    iVar3 = uVar9 * local_2c + ((int)param_2 >> 1);
    uVar8 = (param_2 & 1) * -4 + 4;
    puVar10 = &local_4c;
  }
  else if (uVar6 == 0xe) {
    uVar9 = (uVar4 & 0x1fffff) >> 10;
    iVar3 = uVar9 * local_2c + param_2;
  }
  uVar2 = *(undefined4 *)(param_1 + 0x24);
  pbVar5 = (byte *)FUN_0802d624(uVar9);
  if (pbVar5 == (byte *)0x0) {
    uVar2 = 0;
  }
  else {
    if (*(char *)(param_1 + 0x10) == '\0') {
      pbVar7 = (byte *)(*(int *)(*(int *)(param_1 + 4) + 8) + iVar3);
    }
    else {
      FUN_0802b290(uVar2,iVar3 + 4,0);
      FUN_0802b10a(uVar2,pbVar5,uVar9,0);
      pbVar7 = pbVar5;
    }
    for (iVar3 = 0; iVar3 < local_28; iVar3 = iVar3 + 1) {
      uVar4 = (uint)(*pbVar7 >> (uVar8 & 0xff)) & (1 << (uint)bVar1) - 1U & 0xffff;
      if ((*(byte *)(param_1 + 0x14) & 0x1f) != 0xe) {
        uVar4 = (uint)*(byte *)((int)puVar10 + uVar4);
      }
      *(char *)(iVar3 * 3 + param_5 + 2) = (char)uVar4;
      uVar8 = (uint)(char)((char)uVar8 - bVar1);
      if ((int)uVar8 < 0) {
        uVar8 = (uint)(char)('\b' - bVar1);
        pbVar7 = pbVar7 + 1;
      }
    }
    FUN_0802d6c0(pbVar5);
    uVar2 = 1;
  }
  return uVar2;
}

