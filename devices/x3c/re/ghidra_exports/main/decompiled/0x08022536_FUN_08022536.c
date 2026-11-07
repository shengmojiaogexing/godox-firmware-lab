// 0x08022536 FUN_08022536 size=290

bool FUN_08022536(int param_1,int param_2,undefined4 param_3,undefined4 *param_4)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int extraout_r1;
  undefined4 *extraout_r2;
  int extraout_r3;
  uint uVar7;
  uint uVar8;
  undefined8 uVar9;
  undefined4 local_3c;
  uint local_38;
  int iStack_34;
  int iStack_30;
  undefined4 local_2c;
  undefined4 *puStack_28;
  
  iStack_34 = param_1;
  iStack_30 = param_2;
  local_2c = param_3;
  puStack_28 = param_4;
  uVar3 = FUN_08018536(param_3);
  uVar8 = 1 << (uVar3 & 0xff) & 0xff;
  uVar3 = 0xffffffff;
  uVar2 = *(ushort *)(param_1 + 0x28);
  local_38 = ~(uint)uVar2 & 0xffff;
  bVar1 = *(byte *)(param_1 + 0x2a);
  uVar7 = 0;
  do {
    if (((*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3 <= uVar7) ||
       (-1 < *(int *)(*(int *)(param_1 + 0xc) + uVar7 * 8 + 4) << 6)) {
      do {
        if ((*(ushort *)(param_1 + 0x2a) & 0x1ff) >> 3 <= uVar7) {
          if (-1 < (int)uVar3) {
            *param_4 = local_3c;
          }
          return -1 < (int)uVar3;
        }
        iVar4 = *(int *)(param_1 + 0xc);
        if ((*(byte *)(*(int *)(iVar4 + uVar7 * 8) + 6) & uVar8) != 0) {
          iVar6 = uVar7 * 8 + 4;
          FUN_08031782(*(uint *)(iVar4 + iVar6) & 0xffffff,iVar6,iVar4 + uVar7 * 8);
          uVar5 = FUN_08031788(*(uint *)(*(int *)(param_1 + 0xc) + extraout_r1) & 0xffffff);
          if (((extraout_r3 == param_2) && ((uVar5 & local_38) == 0)) && ((int)uVar3 < (int)uVar5))
          {
            iVar4 = thunk_FUN_08032b92(*extraout_r2,local_2c,&local_3c);
            if (iVar4 == 1) {
              if (uVar5 == uVar2) {
                *param_4 = local_3c;
                return true;
              }
              if ((int)uVar3 < (int)uVar5) {
                *param_4 = local_3c;
                uVar3 = uVar5;
              }
            }
            else if (iVar4 == 2) {
              return (bool)2;
            }
          }
        }
        uVar7 = uVar7 + 1;
      } while( true );
    }
    if (-1 < (int)((uint)bVar1 << 0x1d)) {
      uVar9 = FUN_08031782(*(uint *)(*(int *)(param_1 + 0xc) + uVar7 * 8 + 4) & 0xffffff);
      if (((int)uVar9 == param_2) &&
         (iVar4 = *(int *)((ulonglong)uVar9 >> 0x20), (*(byte *)(iVar4 + 6) & uVar8) != 0)) {
        iVar4 = thunk_FUN_08032b92(iVar4,local_2c,&local_3c);
        if (iVar4 == 1) {
          *param_4 = local_3c;
          return true;
        }
        if (iVar4 == 2) {
          return (bool)2;
        }
      }
    }
    uVar7 = uVar7 + 1;
  } while( true );
}

