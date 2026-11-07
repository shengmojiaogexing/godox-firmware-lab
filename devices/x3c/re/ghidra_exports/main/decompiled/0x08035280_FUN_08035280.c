// 0x08035280 FUN_08035280 size=280

void FUN_08035280(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_18 = param_2;
  local_14 = param_3;
  local_10 = param_4;
  FUN_0801bd04(DAT_08035398,1);
  FUN_08035c78(0x12,1,0);
  FUN_08018be0(&local_14);
  local_14 = 0x200;
  local_10 = CONCAT13((char)((uint)local_10 >> 0x18),0x100);
  FUN_08018b68(&local_14);
  FUN_08018b28(&local_18);
  uVar1 = DAT_0803539c;
  local_18 = 1;
  FUN_08018afc(DAT_0803539c,&local_18);
  FUN_08018e50(uVar1,0);
  FUN_08018c3c(uVar1,0);
  FUN_08018bf2(uVar1,0);
  FUN_08018bfc(uVar1,0);
  FUN_08018cb0(uVar1,2);
  FUN_08018cbc(uVar1,1,1,5);
  FUN_08018cbc(uVar1,0x11,2,5);
  FUN_08018e06(uVar1,0);
  FUN_08018ca6(uVar1,0);
  FUN_08018c24(uVar1,0x80,1);
  FUN_08018c92(uVar1,2,3);
  FUN_08018c7c(uVar1,0);
  FUN_08018c70(uVar1,0);
  FUN_08018e3a(uVar1,1);
  FUN_08018c06(uVar1,1);
  do {
    iVar2 = FUN_08018c16(uVar1,0x40);
  } while (iVar2 == 0);
  FUN_08018b36(uVar1);
  do {
    iVar2 = FUN_08018b40(uVar1);
  } while (iVar2 != 0);
  FUN_08018b4e(uVar1);
  do {
    iVar2 = FUN_08018b58(uVar1);
  } while (iVar2 != 0);
  return;
}

