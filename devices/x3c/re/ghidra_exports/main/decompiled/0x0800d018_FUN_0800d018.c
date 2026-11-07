// 0x0800d018 FUN_0800d018 size=322

undefined8 FUN_0800d018(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  byte bVar1;
  short sVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  iVar5 = DAT_0800d15c + param_1 * 0x74;
  uStack_1c = param_2;
  uStack_18 = param_3;
  uStack_14 = param_4;
  iVar3 = FUN_080303b8(*(undefined4 *)(iVar5 + 0x30));
  local_20 = param_1;
  if ((((iVar3 != 0) && (iVar3 = FUN_080303b8(*(undefined4 *)(iVar5 + 0x60)), iVar3 != 0)) &&
      (iVar3 = FUN_080303b8(*(undefined4 *)(iVar5 + 0x2c)), iVar3 != 0)) &&
     ((iVar3 = FUN_080303b8(*(undefined4 *)(iVar5 + 0x34)), iVar3 != 0 &&
      (iVar3 = FUN_080303b8(*(undefined4 *)(iVar5 + 0x38)), iVar3 != 0)))) {
    if ((*DAT_0800d160 == '\0') ||
       (iVar3 = DAT_0800d164 + param_1 * 6, -1 < (int)((uint)*(byte *)(iVar3 + 4) << 0x18))) {
      FUN_0802da22(*(undefined4 *)(iVar5 + 0x2c),1);
      FUN_0802da22(*(undefined4 *)(iVar5 + 0x60),1);
      FUN_080316cc(*(undefined4 *)(iVar5 + 0x30),DAT_0800d170,0);
      FUN_0802d33c(*(undefined4 *)(iVar5 + 0x30),&DAT_0800d174);
      uVar4 = FUN_08027d64(100,100);
      FUN_080316b4(*(undefined4 *)(iVar5 + 0x30),uVar4,0);
      return CONCAT44(uStack_1c,uVar4);
    }
    if ((*(byte *)(iVar3 + 5) & 1) == 0) {
      FUN_080316cc(*(undefined4 *)(iVar5 + 0x30),DAT_0800d170,0);
      FUN_0802e2d2(*(undefined4 *)(iVar5 + 0x60),1);
      uVar4 = FUN_08027fcc();
      FUN_080316b4(*(undefined4 *)(iVar5 + 0x30),uVar4,0);
      FUN_0802d33c(*(undefined4 *)(iVar5 + 0x30),&DAT_0800d174);
      FUN_08004c48(&uStack_1c,10,&DAT_0800d178,*(byte *)(iVar3 + 4) & 0x7f);
      FUN_0802d33c(*(undefined4 *)(iVar5 + 0x3c),&uStack_1c);
      FUN_0802782e(*(undefined4 *)(iVar5 + 0x34),(*(byte *)(iVar3 + 4) & 0x7f) - 10,0);
      bVar1 = *(byte *)(iVar3 + 4);
      sVar2 = FUN_0802fc5e(*(undefined4 *)(iVar5 + 0x34));
      if (sVar2 == 0) {
        sVar2 = 0xdc;
      }
      FUN_080316fe(*(undefined4 *)(iVar5 + 0x38),
                   (int)((short)(((bVar1 & 0x7f) - 10) * sVar2) / 0x5a));
      return CONCAT44(uStack_1c,uVar4);
    }
    FUN_080316cc(*(undefined4 *)(iVar5 + 0x30),DAT_0800d168,0);
    FUN_0802da22(*(undefined4 *)(iVar5 + 0x60),1);
    local_20 = FUN_08027fcc();
    FUN_080316b4(*(undefined4 *)(iVar5 + 0x30),local_20,0);
    FUN_0802d33c(*(undefined4 *)(iVar5 + 0x30),&DAT_0800d16c);
  }
  return CONCAT44(uStack_1c,local_20);
}

