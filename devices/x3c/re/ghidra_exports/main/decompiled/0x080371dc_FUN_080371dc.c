// 0x080371dc FUN_080371dc size=274

undefined4 FUN_080371dc(undefined4 param_1,undefined4 *param_2)

{
  undefined1 uVar1;
  byte bVar2;
  undefined2 uVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  uint uVar7;
  
  FUN_08029a38(param_2);
  uVar4 = FUN_0802f83a(param_1,0x10000);
  *(char *)((int)param_2 + 5) = (char)uVar4;
  if (2 < uVar4) {
    uVar3 = FUN_0802f9bc(param_1,0x10000,0x20);
    *(undefined2 *)((int)param_2 + 6) = uVar3;
  }
  bVar2 = FUN_0802f9bc(param_1,0x10000,0x31);
  *(byte *)(param_2 + 10) = bVar2;
  if (2 < bVar2) {
    iVar5 = FUN_0802f9bc(param_1,0x10000,0x32);
    param_2[9] = iVar5;
    if (iVar5 < 1) {
      *(undefined1 *)(param_2 + 10) = 0;
    }
    else {
      uVar3 = FUN_0802f9bc(param_1,0x10000,0x30);
      *(undefined2 *)((int)param_2 + 0x22) = uVar3;
    }
  }
  bVar2 = FUN_0802f9bc(param_1,0x10000,0x45);
  *(byte *)(param_2 + 0x12) = bVar2;
  if (2 < bVar2) {
    iVar5 = FUN_0802f9bc(param_1,0x10000,0x40);
    param_2[0xe] = iVar5;
    if (iVar5 < 1) {
      *(undefined1 *)(param_2 + 0x12) = 0;
    }
    else {
      uVar6 = FUN_0802f9bc(param_1,0x10000,0x43);
      param_2[0x11] = uVar6;
      uVar3 = FUN_0802f9bc(param_1,0x10000,0x44);
      *(undefined2 *)((int)param_2 + 0x36) = uVar3;
    }
  }
  uVar4 = FUN_0802f9bc(param_1,0x10000,0x60);
  uVar4 = uVar4 & 0xff;
  if (uVar4 < 0xfd) {
    uVar7 = uVar4 * *(byte *)((int)param_2 + 5);
    *(char *)((int)param_2 + 5) = (char)(uVar7 >> 8);
    uVar1 = (undefined1)(uVar4 * (uVar7 >> 8) >> 8);
    *(undefined1 *)(param_2 + 10) = uVar1;
    *(undefined1 *)(param_2 + 0x12) = uVar1;
  }
  if (((*(char *)((int)param_2 + 5) == '\0') && (*(char *)(param_2 + 10) == '\0')) &&
     (*(char *)(param_2 + 0x12) == '\0')) {
    uVar6 = 0;
  }
  else {
    uVar6 = FUN_0802fa4c(param_1,0x10000);
    *param_2 = uVar6;
    uVar6 = 1;
  }
  return uVar6;
}

