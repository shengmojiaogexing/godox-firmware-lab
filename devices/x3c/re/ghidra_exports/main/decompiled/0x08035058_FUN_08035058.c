// 0x08035058 FUN_08035058 size=286

void FUN_08035058(int param_1)

{
  int iVar1;
  undefined2 uVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  undefined8 uVar7;
  undefined4 local_30;
  int local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  int local_1c;
  undefined4 local_18;
  undefined4 local_14;
  
  iVar3 = FUN_0802acf4(param_1);
  uVar4 = FUN_0802ad70(param_1);
  iVar1 = DAT_08035160;
  if (iVar3 == 0x12) {
    if ((*(char *)(param_1 + 8) == '\x12') &&
       (uVar7 = FUN_0802ad6c(param_1,2), (uint)*(byte *)uVar7 < (uint)((ulonglong)uVar7 >> 0x20))) {
      *(byte *)uVar7 = (byte)((ulonglong)uVar7 >> 0x20);
    }
    return;
  }
  if (iVar3 == 0x14) {
    iVar3 = FUN_0802f9bc(uVar4,0,0x57);
    FUN_0802f9bc(uVar4,0,0x59);
    iVar6 = *(int *)(iVar3 + 8);
    FUN_0802ed04(uVar4,&local_20);
    local_30 = local_20;
    local_28 = local_18;
    local_2c = local_1c;
    iVar3 = FUN_0802ed88(uVar4);
    local_24 = local_1c + (iVar3 - iVar6) / 2;
    uVar4 = FUN_0802d624(0x24);
    FUN_08029324(uVar4,&local_30,0,local_2c,0xff,local_24);
    uVar2 = FUN_08028e78(uVar4,0);
    *(undefined2 *)(iVar1 + 6) = uVar2;
    local_2c = local_24 + iVar6 + -1;
    local_24 = local_14;
    uVar4 = FUN_0802d624(0x24);
    FUN_08029324(uVar4,&local_30,0xff,local_2c,0,local_24);
    uVar2 = FUN_08028e78(uVar4,0);
    *(undefined2 *)(iVar1 + 8) = uVar2;
  }
  else if (iVar3 == 0x19) {
    uVar4 = FUN_080299f4((int)*(short *)(DAT_08035160 + 6));
    uVar5 = FUN_080299f4((int)*(short *)(iVar1 + 8));
    FUN_08029358(uVar4);
    FUN_08029358(uVar5);
    FUN_0802d6c0(uVar4);
    FUN_0802d6c0(uVar5);
    *(undefined2 *)(iVar1 + 6) = 0xffff;
    *(undefined2 *)(iVar1 + 8) = 0xffff;
  }
  return;
}

