// 0x08018180 FUN_08018180 size=304

void FUN_08018180(int param_1,int param_2,undefined2 param_3,undefined2 param_4,ushort *param_5)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  int *local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  uint local_40;
  int local_3c;
  byte local_2a;
  uint local_28;
  uint local_24;
  
  *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) | 4;
  *(undefined2 *)(param_1 + 0x28) = param_3;
  uVar1 = FUN_0802f9bc(param_1,param_2,param_5[4]);
  *(undefined2 *)(param_1 + 0x28) = param_4;
  local_24 = uVar1;
  uVar2 = FUN_0802f9bc(param_1,param_2,param_5[4]);
  *(ushort *)(param_1 + 0x2a) = *(ushort *)(param_1 + 0x2a) & 0xfffb;
  if ((uVar1 != uVar2) || ((local_24 & 0xffff) != (uVar2 & 0xffff))) {
    *(undefined2 *)(param_1 + 0x28) = param_3;
    local_28 = uVar2;
    iVar3 = FUN_0802f9bc(param_1,param_2,param_5[4]);
    *(undefined2 *)(param_1 + 0x28) = param_4;
    local_24 = iVar3;
    puVar4 = (undefined4 *)FUN_080227c6(param_1,param_2);
    FUN_08032ff8(*puVar4,param_5[4],iVar3);
    if ((param_5[4] == 0xb) && ((iVar3 == 0x7fff || (uVar2 == 0x7fff)))) {
      iVar5 = FUN_0802fc5e(param_1);
      iVar5 = iVar5 / 2;
      iVar3 = FUN_0802ed88(param_1);
      iVar3 = iVar3 / 2;
      if ((local_24 == 0x7fff) && (local_24 = iVar5 + 1, iVar3 + 1 <= (int)local_24)) {
        local_24 = iVar3 + 1;
      }
      if (uVar2 == 0x7fff) {
        if (iVar5 + 1 < iVar3 + 1) {
          iVar3 = iVar5;
        }
        local_28 = iVar3 + 1;
      }
    }
    local_6c = (int *)FUN_08017f8c(DAT_080182b0);
    if (local_6c == (int *)0x0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    local_6c[3] = local_24;
    local_6c[4] = local_28;
    *local_6c = param_1;
    *(ushort *)(local_6c + 1) = param_5[4];
    local_6c[2] = param_2;
    FUN_080269c8(&local_6c);
    local_68 = DAT_080182b4;
    local_64 = DAT_080182b8;
    local_60 = DAT_080182bc;
    local_4c = 0;
    local_48 = 0;
    local_44 = 0xff;
    local_40 = (uint)*param_5;
    local_3c = -(uint)param_5[1];
    local_50 = *(undefined4 *)(param_5 + 6);
    local_2a = local_2a & 0xfe;
    local_54 = *(undefined4 *)(param_5 + 8);
    FUN_08026aac(&local_6c);
  }
  return;
}

