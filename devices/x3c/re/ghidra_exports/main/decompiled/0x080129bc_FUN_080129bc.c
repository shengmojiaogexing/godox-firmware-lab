// 0x080129bc FUN_080129bc size=216

undefined8 FUN_080129bc(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int iVar2;
  ushort uVar3;
  uint uVar4;
  undefined4 local_28;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  undefined4 local_20;
  char local_1c;
  undefined3 uStack_1b;
  
  uVar1 = DAT_08012a98;
  local_24 = (undefined1)param_2;
  local_23 = (undefined1)((uint)param_2 >> 8);
  local_22 = (undefined1)((uint)param_2 >> 0x10);
  local_21 = (undefined1)((uint)param_2 >> 0x18);
  local_20 = *(undefined4 *)(DAT_08012a94 + 8);
  _local_1c = CONCAT31((int3)((uint)param_4 >> 8),
                       (char)local_20 + (char)((uint)local_20 >> 8) +
                       (char)((uint)local_20 >> 0x10) + (char)((uint)local_20 >> 0x18) + -0x30);
  local_28 = param_1;
  FUN_08038400(DAT_08012a98,0);
  FUN_08038128(&local_28);
  local_28 = 0;
  local_24 = 0;
  local_23 = 1;
  local_22 = 1;
  local_21 = 1;
  FUN_080382c8(uVar1,&local_28);
  FUN_08038224(uVar1,0x40,0);
  uVar4 = 0;
  do {
    uVar3 = 0;
    FUN_080381d6(uVar1,*(undefined1 *)((int)&local_20 + uVar4));
    do {
      iVar2 = FUN_08038216(uVar1,2);
      if (iVar2 != 0) break;
      uVar3 = uVar3 + 1;
    } while (uVar3 < 2000);
    uVar4 = uVar4 + 1;
    if (4 < uVar4) {
      FUN_0801c118(0x28);
      FUN_080380c4();
      FUN_08035c78(0x28,1,0);
      FUN_08038400(uVar1,1);
      return CONCAT17(local_21,CONCAT16(local_22,CONCAT15(local_23,CONCAT14(local_24,local_28))));
    }
  } while( true );
}

