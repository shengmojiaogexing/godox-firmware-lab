// 0x08036844 FUN_08036844 size=298

undefined4 FUN_08036844(uint param_1,int param_2,int param_3,uint param_4)

{
  undefined4 *puVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  uint uVar12;
  uint uVar13;
  uint local_30;
  int local_2c;
  int local_28;
  uint local_24;
  
  local_30 = param_1;
  local_2c = param_2;
  local_28 = param_3;
  local_24 = param_4;
  iVar3 = FUN_080223b4(param_1);
  if (iVar3 == 0) {
    uVar4 = 1;
  }
  else {
    uVar4 = FUN_0802c5fc();
    iVar5 = FUN_0802c69e(uVar4);
    if (((iVar5 == 4) || (iVar5 == 2)) &&
       (*(undefined2 *)(param_1 + 0x30) = *(undefined2 *)(param_1 + 0x2e), iVar5 == 4)) {
      uVar6 = FUN_0802ed7a(param_1);
      iVar5 = FUN_0802b744(uVar6);
      if (iVar5 != 0) {
        FUN_0802b8b2(uVar6,0);
      }
    }
    iVar5 = FUN_0802c69e(uVar4);
    if ((iVar5 == 1) || (iVar5 = FUN_0802c69e(uVar4), iVar5 == 3)) {
      if (*(int *)(param_1 + 0x30) << 0xe < 0) {
        iVar5 = FUN_0802faa0(param_1,0);
        iVar8 = FUN_0802fab2(param_1,0);
        iVar10 = *(int *)(iVar5 + 8);
        iVar11 = *(int *)(param_1 + 0x20);
        iVar5 = *(int *)(param_1 + 0x18);
        iVar9 = FUN_0802c860(uVar4,0xc);
        iVar3 = ((iVar5 + (iVar11 - iVar5) / 2) - (iVar9 + *(int *)(iVar3 + 0x18))) /
                (iVar10 + iVar8);
        if (iVar3 < 0) {
          iVar3 = 0;
        }
        if ((int)(uint)*(ushort *)(param_1 + 0x2c) <= iVar3) {
          iVar3 = *(ushort *)(param_1 + 0x2c) - 1;
        }
        uVar12 = (uint)(short)iVar3;
      }
      else {
        uVar12 = 0;
        FUN_0802c634(uVar4,&local_2c);
        local_28 = local_28 - *(int *)(iVar3 + 0x18);
        local_2c = local_2c - *(int *)(iVar3 + 0x14);
        uVar7 = FUN_0802cb88(iVar3,&local_2c);
        uVar4 = FUN_0802cd72(iVar3);
        puVar1 = DAT_08036970;
        local_30 = 0;
        for (uVar13 = 0; uVar2 = local_30, uVar13 < uVar7; uVar13 = uVar13 + 1) {
          iVar3 = (*(code *)*puVar1)(uVar4,&local_30);
          if ((iVar3 == 10) && (uVar2 != uVar7)) {
            uVar12 = (uint)(short)((short)uVar12 + 1);
          }
        }
      }
      if (-1 < (int)uVar12) {
        FUN_08031fba(param_1,uVar12 & 0xffff,1);
      }
    }
    local_24 = (uint)*(ushort *)(param_1 + 0x2e);
    uVar4 = FUN_0802ad74(param_1,0x1c,&local_24);
  }
  return uVar4;
}

