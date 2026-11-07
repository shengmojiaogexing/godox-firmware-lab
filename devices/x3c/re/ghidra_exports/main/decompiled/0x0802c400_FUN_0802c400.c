// 0x0802c400 FUN_0802c400 size=346

void FUN_0802c400(int param_1,int param_2)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int iVar7;
  uint local_28;
  int local_24;
  uint local_20;
  
  FUN_080302d0(param_1);
  iVar2 = FUN_0802c55a(param_2);
  if (iVar2 == 3) {
    bVar1 = *(byte *)(param_1 + 0x4e) & 3;
    if ((bVar1 == 2) || (bVar1 == 1)) {
      FUN_0802d6e4(*(undefined4 *)(param_1 + 0x2c));
    }
    *(undefined4 *)(param_1 + 0x2c) = 0;
    *(byte *)(param_1 + 0x4e) = *(byte *)(param_1 + 0x4e) | 3;
  }
  else {
    FUN_0802c05c(param_2,&local_20);
    if (iVar2 == 0) {
      bVar1 = *(byte *)(param_1 + 0x4e) & 3;
      if ((bVar1 == 1) || (bVar1 == 2)) {
        FUN_0802d6e4(*(undefined4 *)(param_1 + 0x2c));
      }
      *(int *)(param_1 + 0x2c) = param_2;
    }
    else if (((iVar2 == 1) || (iVar2 == 2)) && (*(int *)(param_1 + 0x2c) != param_2)) {
      bVar1 = *(byte *)(param_1 + 0x4e) & 3;
      if ((bVar1 == 1) || (iVar7 = 0, bVar1 == 2)) {
        iVar7 = *(int *)(param_1 + 0x2c);
      }
      iVar3 = FUN_08004df8(param_2);
      iVar3 = FUN_0802d5c4(iVar3 + 1);
      if (iVar3 == 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      FUN_08004db0(iVar3,param_2);
      *(int *)(param_1 + 0x2c) = iVar3;
      if (iVar7 != 0) {
        FUN_0802d6e4(iVar7);
      }
    }
    if (iVar2 == 2) {
      uVar4 = FUN_0802f9bc(param_1,0,0x57);
      uVar5 = FUN_0802f9bc(param_1,0,0x58);
      uVar6 = FUN_0802f9bc(param_1,0,0x59);
      FUN_08033a84(&local_28,param_2,uVar4,uVar5,uVar6,0x1fffffff,0,uVar6);
      local_20 = local_20 & 0x3ff | (local_28 & 0x7ff) << 10 | local_24 << 0x15;
    }
    *(byte *)(param_1 + 0x4e) = *(byte *)(param_1 + 0x4e) & 0xfc | (byte)iVar2 & 3;
    *(uint *)(param_1 + 0x38) = (local_20 & 0x1fffff) >> 10;
    *(uint *)(param_1 + 0x3c) = local_20 >> 0x15;
    *(byte *)(param_1 + 0x4e) = *(byte *)(param_1 + 0x4e) & 0x83 | (byte)((local_20 & 0x1f) << 2);
    *(uint *)(param_1 + 0x44) = (local_20 & 0x1fffff) >> 0xb;
    *(uint *)(param_1 + 0x48) = local_20 >> 0x16;
    FUN_08030c70(param_1);
    if ((*(short *)(param_1 + 0x40) != 0) || (*(short *)(param_1 + 0x4c) != 0x100)) {
      FUN_08030c20(param_1);
    }
    FUN_080302d0(param_1);
  }
  return;
}

