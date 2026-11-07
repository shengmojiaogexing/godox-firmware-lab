// 0x0802bc60 FUN_0802bc60 size=324

undefined4 FUN_0802bc60(int param_1,uint param_2,int param_3,int param_4,int param_5)

{
  undefined2 uVar1;
  byte bVar2;
  uint uVar3;
  byte *pbVar4;
  undefined4 uVar5;
  uint uVar6;
  byte *pbVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  
  bVar2 = FUN_0802b904(*(byte *)(param_1 + 0x14) & 0x1f);
  uVar11 = 0;
  uVar9 = 0;
  iVar10 = 0;
  uVar3 = *(uint *)(param_1 + 0x14);
  uVar6 = uVar3 & 0x1f;
  if (uVar6 == 7) {
    uVar11 = ((uVar3 & 0x1fffff) >> 10) + 7 >> 3;
    iVar10 = uVar11 * param_3 + ((int)param_2 >> 3) + 8;
    uVar9 = 7 - (param_2 & 7);
  }
  else if (uVar6 == 8) {
    uVar11 = ((uVar3 & 0x1fffff) >> 10) + 3 >> 2;
    iVar10 = uVar11 * param_3 + ((int)param_2 >> 2) + 0x10;
    uVar9 = (param_2 & 3) * -2 + 6;
  }
  else if (uVar6 == 9) {
    uVar11 = ((uVar3 & 0x1fffff) >> 10) + 1 >> 1;
    iVar10 = uVar11 * param_3 + ((int)param_2 >> 1) + 0x40;
    uVar9 = (param_2 & 1) * -4 + 4;
  }
  else if (uVar6 == 10) {
    uVar11 = (uVar3 & 0x1fffff) >> 10;
    iVar10 = uVar11 * param_3 + param_2 + 0x400;
  }
  iVar12 = *(int *)(param_1 + 0x24);
  pbVar4 = (byte *)FUN_0802d624(uVar11);
  if (pbVar4 == (byte *)0x0) {
    uVar5 = 0;
  }
  else {
    if (*(char *)(param_1 + 0x10) == '\0') {
      pbVar7 = (byte *)(*(int *)(*(int *)(param_1 + 4) + 8) + iVar10);
    }
    else {
      FUN_0802b290(iVar12,iVar10 + 4,0);
      FUN_0802b10a(iVar12,pbVar4,uVar11,0);
      pbVar7 = pbVar4;
    }
    for (iVar10 = 0; iVar10 < param_4; iVar10 = iVar10 + 1) {
      uVar3 = (uint)(*pbVar7 >> (uVar9 & 0xff)) & (1 << (uint)bVar2) - 1U & 0xffff;
      uVar1 = *(undefined2 *)(*(int *)(iVar12 + 0xc) + uVar3 * 2);
      *(char *)(param_5 + iVar10 * 3) = (char)uVar1;
      iVar8 = iVar10 * 3 + param_5;
      *(char *)(iVar8 + 1) = (char)((ushort)uVar1 >> 8);
      *(undefined1 *)(iVar8 + 2) = *(undefined1 *)(*(int *)(iVar12 + 0x10) + uVar3);
      uVar9 = (uint)(char)((char)uVar9 - bVar2);
      if ((int)uVar9 < 0) {
        uVar9 = (uint)(char)('\b' - bVar2);
        pbVar7 = pbVar7 + 1;
      }
    }
    FUN_0802d6c0(pbVar4);
    uVar5 = 1;
  }
  return uVar5;
}

