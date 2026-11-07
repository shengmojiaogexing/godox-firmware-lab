// 0x0802ad74 FUN_0802ad74 size=68

undefined4 FUN_0802ad74(int param_1,undefined1 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  int local_24;
  int local_20;
  undefined1 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  byte local_c;
  
  iVar1 = DAT_0802adb8;
  if (param_1 == 0) {
    uVar2 = 1;
  }
  else {
    local_18 = 0;
    local_c = local_c & 0xf8;
    local_10 = *(undefined4 *)(DAT_0802adb8 + 4);
    *(int **)(DAT_0802adb8 + 4) = &local_24;
    local_24 = param_1;
    local_20 = param_1;
    local_1c = param_2;
    local_14 = param_3;
    uVar2 = FUN_0802081c();
    *(undefined4 *)(iVar1 + 4) = local_10;
  }
  return uVar2;
}

